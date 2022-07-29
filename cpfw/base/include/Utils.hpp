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

#ifndef CPFW_BASE_INCLUDE_UTILS_HPP_
#define CPFW_BASE_INCLUDE_UTILS_HPP_

#include <vector>
#include <algorithm>

#include "cpfw/base/include/Base.h"
#include "cpfw/base/include/DataStore.hpp"

namespace cpfw {


/**
 * @brief parse data from prifile.
 *
 * @param profile where to parse
 * @param type how to parse @see ElementType
 * @param context get context
 * @param store data store, used when convert data
 *
 * @return data parsed from profil
 */
inline std::vector<int32_t> parseProfile(
        const Profile *profile, uint32_t type,
        std::string context, std::shared_ptr<DataStore> store) {
    std::vector<int32_t> ret;
    auto &elements = profile->elements;
    std::for_each(elements.begin(), elements.end(),
        [&](auto &data) -> void {
            int32_t current = 0;
            if (0 != (type & static_cast<uint32_t>(ElementType::PUBLIC))
                    && (0 != (data.second.type & static_cast<uint32_t>(ElementType::PUBLIC)))) {
                current = data.second.current;
            }
            if (store
                    && 0 != (type & static_cast<uint32_t>(ElementType::NEED_CONVERT))
                    && 0 != (data.second.type & static_cast<uint32_t>(ElementType::NEED_CONVERT))) {
                current = store->getConvertedData(context, current);
            }
            ret.push_back(current);
    });
    return ret;
}

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_UTILS_HPP_

