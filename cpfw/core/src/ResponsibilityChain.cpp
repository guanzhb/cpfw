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
#define LOG_TAG "ResponsibilityChain"

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
        LOGE("no widget for id:%d", widgetId);
        return -EINVAL;
    }

    int32_t ret = 0;

    auto preChain = mStore->getPreChain(widgetId);
    if (preChain != DataStore::EMPTY_INVOKE_CHAIN) {
        for (auto preId : preChain) {
            ret = invokeChain(preId);
            if (0 != ret) {
                LOGE("pre action for id:%d, errno:%d", preId, ret);
            }
        }
    }

    ret = widget->get()->process();

    auto postChain = mStore->getPostChain(widgetId);
    if (postChain != DataStore::EMPTY_INVOKE_CHAIN) {
        for (auto postId : postChain) {
            ret = invokeChain(postId);
            if (0 != ret) {
                LOGE("post action for id:%d, errno:%d", postId, ret);
            }
        }
    }

    LOGI("finish to invokeWidget:%s with error: %d", widget->get()->getName().c_str(), ret);

    return ret;
}

}  // namespace cpfw

