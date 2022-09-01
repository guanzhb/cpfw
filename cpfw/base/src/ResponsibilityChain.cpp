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

#include "cpfw/base/include/ResponsibilityChain.h"

#include <string>
#include <functional>
#include <memory>
#include <iostream>
#include <algorithm>

#include "cpfw/base/include/Log.hpp"

namespace cpfw {

ResponsibilityChain::ResponsibilityChain() {
}

ResponsibilityChain::ResponsibilityChain(std::shared_ptr<DataStore> store)
        : mStore(store) {
}

ResponsibilityChain::~ResponsibilityChain() {
}

int32_t ResponsibilityChain::invokeChain(std::string widgetName) const {
    return invokeWidget(widgetName);
}

int32_t ResponsibilityChain::invokeWidget(std::string widgetName) const {
    std::optional<std::shared_ptr<Widget>> widget = mStore->getWidget(widgetName);
    if (!widget) {
        LOGE("no widget for name " + widgetName);
    }

    int32_t ret = 0;
    if ((ret = widget->get()->check()) != 0) {
        LOGE("check for name " + widgetName + ", errno:" + std::to_string(ret));
    }
    if ((ret = widget->get()->action()) != 0) {
        widget->get()->reset();
        LOGE("action for name " + widgetName + ", errno:" + std::to_string(ret));
        return ret;
    }
    widget->get()->swipe();

    auto childrenChain = mStore->getChain(widgetName);
    if (childrenChain == DataStore::EMPTY_INVOKE_CHAIN) {
        return ret;
    }
    std::for_each(childrenChain.begin(), childrenChain.end(),
        [&] (auto &childWidgetName) -> void {
            ret = invokeChain(childWidgetName);
            if (0 != ret) {
                LOGE("action for name " + childWidgetName + ", errno:" + std::to_string(ret));
            }
        });
    LOGI("invokeWidget -> " + widgetName + " success");
    return ret;
}

}  // namespace cpfw

