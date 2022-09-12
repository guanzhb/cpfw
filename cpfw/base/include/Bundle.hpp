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
    bool get(const std::string &key, TVALUE &value) {
        if (const auto &tmp = mTable.find(key); tmp != mTable.end()) {
            value = std::any_cast<TVALUE&>(tmp->second);
            return true;
        }
        return false;
    }

    void erase(const std::string &key) {
        mTable.erase(key);
    }

    void clear() {
        mTable.clear();
    }

 private:
    std::map<std::string, std::any> mTable;
};

#define INITIALIZE_TEMPLATE(TYPE) \
    template void Bundle::set<TYPE>(const std::string &, const TYPE &); \
    template bool Bundle::get<TYPE>(const std::string &, TYPE &)

INITIALIZE_TEMPLATE(std::string);
INITIALIZE_TEMPLATE(int64_t);
INITIALIZE_TEMPLATE(uint64_t);
INITIALIZE_TEMPLATE(int32_t);
INITIALIZE_TEMPLATE(uint32_t);
INITIALIZE_TEMPLATE(int16_t);
INITIALIZE_TEMPLATE(uint16_t);
INITIALIZE_TEMPLATE(int8_t);
INITIALIZE_TEMPLATE(uint8_t);


#endif  // CPFW_BASE_INCLUDE_BUNDLE_HPP__

