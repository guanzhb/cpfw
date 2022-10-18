/** * Copyright (C) 2022 The Cross Platform Framework Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define TAG "ResponsibilityChain"

#include "ResponsibilityChain.h"

#include <algorithm>
#include <functional>
#include <memory>
#include <string>

#include "Log.hpp"

namespace cpfw {

ResponsibilityChain::ResponsibilityChain() {
}

ResponsibilityChain::ResponsibilityChain(std::shared_ptr<DataStore> store)
        : mStore(store) {
}

ResponsibilityChain::~ResponsibilityChain() {
}

int32_t ResponsibilityChain::invokeChain(const uint32_t widgetId) const {
    return invokeWidget(widgetId);
}

int32_t ResponsibilityChain::invokeWidget(const uint32_t widgetId) const {
    std::optional<std::shared_ptr<Widget>> widget = mStore->getWidget(widgetId);
    if (!widget) {
        LOGE("no widget for id " + std::to_string(widgetId));
        return EINVAL;
    }

    int32_t ret = 0;
    if ((ret = widget->get()->check()) != 0) {
        LOGE("check for name " + widget->get()->getName() + ", errno:" + std::to_string(ret));
        return ret;
    }
    if ((ret = widget->get()->action()) != 0) {
        widget->get()->reset();
        LOGE("action for name " + widget->get()->getName() + ", errno:" + std::to_string(ret));
        return ret;
    }
    widget->get()->swipe();

    auto childrenChain = mStore->getChain(widgetId);
    if (childrenChain == DataStore::EMPTY_INVOKE_CHAIN) {
        return ret;
    }
    std::for_each(childrenChain.begin(), childrenChain.end(),
        [&] (auto &childWidgetId) -> void {
            ret = invokeChain(childWidgetId);
            if (0 != ret) {
                LOGE("action for name " + std::to_string(childWidgetId)
                     + ", errno:" + std::to_string(ret));
            }
        });
    LOGI("invokeWidget -> " + widget->get()->getName() + " success");
    return ret;
}

}  // namespace cpfw

