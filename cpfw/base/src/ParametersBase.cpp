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
#include "ParametersBase.h"

#include <algorithm>
#include <iterator>

#include "MapUtils.hpp"

namespace cpfw {

ParametersBase::ParametersBase() {
}

ParametersBase::~ParametersBase() {
}

std::pair<std::string/*key*/, std::string/*values*/>
        ParametersBase::parseKv(const std::string &kv) {
    std::string::size_type pos = kv.find(DELIM_KEY_VALUES);
    if (pos == std::string::npos) {
        return { kv, "" };
    }
    return { kv.substr(0, pos), kv.substr(pos+1) };
}

uint8_t ParametersBase::calculateValueCount(const std::string &values) {
    uint8_t count = std::count(values.begin(), values.end(), DELIM_VALUES);
    if (!values.empty()) {
        ++count;
    }
    return count;
}

int32_t ParametersBase::setParameters(const std::string &kv) {
    auto [ key, values ] = parseKv(kv);
    return setParameters(key, values);
}

int32_t ParametersBase::setParameters(const std::string &key, const std::string &values) {
    if (auto itor = getOptionalFromMap(mSetTable, key); itor) {
        uint8_t count = calculateValueCount(values);
        if (std::find(itor->first.begin(), itor->first.end(), count) != end(itor->first)) {
            return std::invoke(itor->second, values);
        }
    }
    return -EINVAL;
}

int32_t ParametersBase::getParameters(const std::string &kv, std::string &result) {
    auto [ key, values ] = parseKv(kv);
    return getParameters(key, values, result);
}

int32_t ParametersBase::getParameters(
        const std::string &key, const std::string &values, std::string &result) {
    if (auto itor = getOptionalFromMap(mGetTable, key); itor) {
        uint8_t count = calculateValueCount(values);
        if (std::find(itor->first.begin(), itor->first.end(), count) != end(itor->first)) {
            return std::invoke(itor->second, values, result);
        }
    }
    return -EINVAL;
}

}  // namespace cpfw

