/**
 * Copyright (C) 2020 The Cross Platform Framework Project
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

#include <string>
#include <functional>
#include <memory>
#include <iostream>
#include <algorithm>

#include "cpfw/base/include/ResponsibilityChain.h"

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
    std::shared_ptr<Widget> widget = mStore->getWidget(widgetName);
    if (widget == nullptr) {
        return EINVAL;  // TODO(guanzhb) LOGE
    }

    int32_t ret = 0;
    if (ret = widget->check() != 0) {
        return ret;  // TODO(guanzhb) LOGE
    }
    if (ret = widget->action() != 0) {
        return ret;  // TODO(guanzhb) LOGE
    }
    widget->swipe();

    auto childrenChain = mStore->getChain(widgetName);
    std::for_each(childrenChain.begin(), childrenChain.end(),
        [&] (auto &childwidgetName) -> void {
            ret = invokeChain(childwidgetName);
            if (0 != ret) {
                //  TODO(guanzhb) LOGE
            }
        });

    return ret;
}

}  // namespace cpfw

