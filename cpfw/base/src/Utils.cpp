/**
 * Copyright (C) 2022 The Cross Platform Framework Project
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

#include "Utils.h"

#include <sys/time.h>

#include "Base.h"
#include "StrategyCalculate.h"

namespace cpfw {

std::vector<int32_t> parseProfile(
        const Profile &profile, uint32_t type,
        uint32_t widgetId, std::shared_ptr<DataStore> store) {
    std::vector<int32_t> ret;
    if (!store) {
        return ret;
    }
    auto &elements = profile.elements;
    std::for_each(elements.begin(), elements.end(),
        [&](auto &data) -> void {
            auto &d = data.second;
            if (0 == (type & ElementType::PUBLIC)
                    || (0 == (d.type & ElementType::PUBLIC))) {
                return;
            }
            int32_t current = d.current;
            if (0 != (d.type & ElementType::NEED_CONVERT)) {
                current = store->getConvertedData(widgetId, current);
                auto& converts = store->getConvertTable(widgetId);
                for (auto &c : converts) {
                    current = StrategyCalculatePool::getStrategy(
                                    c.getExpression())->handle(
                                        widgetId, current, c, store);
                }
            }
            ret.push_back(current);
    });
    return ret;
}

uint64_t getCurrentTimeMs() {
    struct timespec stTimeSpec;
    clock_gettime(CLOCK_MONOTONIC, &stTimeSpec);
    return stTimeSpec.tv_sec * 1000 + stTimeSpec.tv_nsec / 1000000;
}

int32_t invoke(const std::any &func, std::vector<int32_t> &values) {
    int32_t ret = 0;
    switch (values.size()) {
    case 0:
        ret = std::invoke(std::any_cast<FUNCTION_0INT>(func));
        break;
    case 1:
        ret = std::invoke(std::any_cast<FUNCTION_1INT>(func), values[0]);
        break;
    case 2:
        ret = std::invoke(std::any_cast<FUNCTION_2INT>(func),
                          values[0], values[1]);
        break;
    case 3:
        ret = std::invoke(std::any_cast<FUNCTION_3INT>(func),
                          values[0], values[1], values[2]);
        break;
    case 4:
        ret = std::invoke(std::any_cast<FUNCTION_4INT>(func),
                          values[0], values[1], values[2], values[3]);
        break;
    case 5:
        ret = std::invoke(std::any_cast<FUNCTION_5INT>(func),
                          values[0], values[1], values[2], values[3], values[4]);
        break;
    default:
        ret = std::invoke(std::any_cast<FUNCTION_VINT>(func), values);
        break;
    }
    return ret;
}

}  // namespace cpfw
