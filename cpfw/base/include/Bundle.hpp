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

#ifndef CPFW_BASE_INCLUDE_BUNDLE_HPP__
#define CPFW_BASE_INCLUDE_BUNDLE_HPP__

#include <any>
#include <map>
#include <string>

class Bundle {
 public:
    Bundle() {}
    ~Bundle() {}

    template<typename TVALUE>
    void set(const std::string &key, const TVALUE &value) {
        mTable[key] = value;
    }

    template<typename TVALUE>
    TVALUE get(const std::string &key, const TVALUE &defaultValue = TVALUE()) {
        auto &value = mTable[key];
        return value.has_value() ? std::any_cast<TVALUE>(value) : defaultValue;
    }

    void erase(const std::string &key) {
        mTable.erase(key);
    }

    void clean() {
        mTable.clear();
    }

 private:
    std::map<std::string, std::any> mTable;
};

#endif  // CPFW_BASE_INCLUDE_BUNDLE_H__

