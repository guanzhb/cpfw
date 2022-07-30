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

#ifndef CPFW_BASE_INCLUDE_RESPONSIBILITYCHAIN_H_
#define CPFW_BASE_INCLUDE_RESPONSIBILITYCHAIN_H_

#include <string>
#include <functional>
#include <memory>
#include <algorithm>

#include "cpfw/base/include/DataStore.h"

namespace cpfw {

class ResponsibilityChain {
 public:
    ResponsibilityChain();
    explicit ResponsibilityChain(std::shared_ptr<DataStore> store);

    ~ResponsibilityChain();

    /**
     * @brief invoke widget chain with name
     *
     * @param widgetName
     * @return int32_t 0 if success, else errno
     */
    int32_t invokeChain(std::string widgetName) const;

 private:
    int32_t invokeWidget(std::string widgetName) const;

 private:
    std::shared_ptr<DataStore> mStore;
};

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_RESPONSIBILITYCHAIN_H_

