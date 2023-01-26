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

#ifndef CPFW_CORE_INCLUDE_UTILS_HPP_
#define CPFW_CORE_INCLUDE_UTILS_HPP_

#include <algorithm>
#include <any>
#include <optional>
#include <vector>

#include "Base.h"
#include "Bundle.hpp"
#include "DataStore.h"

namespace cpfw {

Bundle parseProfile(const Profile &profile, uint32_t type,
        uint32_t widgetId, std::shared_ptr<DataStore> store);

uint64_t getCurrentTimeMs();

void dumpBytes(const uint8_t *data, const std::size_t bytes,
               const std::size_t dataInLine, const char *header);

template<typename TTable, typename TKey>
std::optional<typename TTable::mapped_type> getOptionalFromMap(
        const TTable &table, const TKey &key) {
    std::optional<typename TTable::mapped_type> ret = std::nullopt;
    if (const auto &itor = table.find(key); itor != table.end()) {
         ret = itor->second;
    }
    return ret;
}

template<typename TTable, typename TKey, typename TValue>
TValue& getOrDefaultFromMap(TTable &table, const TKey &key, TValue &value) {
    if (auto itor = table.find(key); itor != table.end()) {
         return itor->second;
    }
    return value;
}

}  // namespace cpfw

#endif  // CPFW_CORE_INCLUDE_UTILS_HPP_
