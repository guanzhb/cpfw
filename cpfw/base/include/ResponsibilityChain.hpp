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

#ifndef CPFW_BASE_INCLUDE_RESPONSIBILITYCHAIN_HPP_
#define CPFW_BASE_INCLUDE_RESPONSIBILITYCHAIN_HPP_

#include <string>
#include <functional>
#include <memory>
#include <algorithm>

#include "cpfw/base/include/DataStore.hpp"

namespace cpfw {

class ResponsibilityChain {
 public:
    explicit ResponsibilityChain(std::shared_ptr<DataStore> store)
        : mStore(store) {
    }

    ~ResponsibilityChain() {
    }

    /**
     * @brief invoke interface chain with name
     *
     * @param interfaceName
     * @return int32_t 0 if success, else errno
     */
    int32_t invokeChain(std::string interfaceName) const {
        int32_t ret = invokeInterface(interfaceName);
        if (ret == 0) {
            return ret;
        }
        ret = invokeWidget(interfaceName);
        return ret;
    }

 private:
    int32_t invokeInterface(std::string interfaceName) const {
        TINTERFACE interface = mStore->getInterface(interfaceName);
        if (interface == nullptr) {
            return EINVAL;  // TODO(guanzhb) LOGE
        }

        int32_t ret = std::invoke(interface);

        auto childrenChain = mStore->getChain(interfaceName);
        std::for_each(childrenChain.begin(), childrenChain.end(),
            [&] (auto &childInterfaceName) -> void {
                ret = invokeChain(childInterfaceName);
            });

        return ret;
    }

    int32_t invokeWidget(std::string interfaceName) const {
        std::shared_ptr<WidgetBase> widget = mStore->getWidget(interfaceName);
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

        auto childrenChain = mStore->getChain(interfaceName);
        std::for_each(childrenChain.begin(), childrenChain.end(),
            [&] (auto &childInterfaceName) -> void {
                ret = invokeChain(childInterfaceName);
                if (0 != ret) {
                    //  TODO(guanzhb) LOGE
                }
            });

        return ret;
    }

 private:
    std::shared_ptr<DataStore> mStore;
};

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_RESPONSIBILITYCHAIN_HPP_

