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

#ifndef CPFW_BASE_INCLUDE_BUNDLE_HPP_
#define CPFW_BASE_INCLUDE_BUNDLE_HPP_

#include <any>
#include <map>
#include <string>
#include <typeindex>

namespace cpfw {

namespace {
    using TTable = std::map<std::string, std::any>;
    using iterator = TTable::iterator;
    using reverse_iterator = TTable::reverse_iterator;
    using const_iterator = TTable::const_iterator;
    using const_reverse_iterator = TTable::const_reverse_iterator;
}  // namespace

class Bundle {
 public:

    iterator begin() noexcept {
        return mTable.begin();
    }

    reverse_iterator rbegin() noexcept {
        return mTable.rbegin();
    }

    const_iterator cbegin() const noexcept {
        return mTable.cbegin();
    }

    const_reverse_iterator crbegin() const noexcept {
        return mTable.crbegin();
    }

    iterator end() noexcept {
        return mTable.end();
    }

    reverse_iterator rend() noexcept {
        return mTable.rend();
    }

    const_iterator cend() const noexcept {
        return mTable.cend();
    }

    const_reverse_iterator crend() const noexcept {
        return mTable.crend();
    }

    template<typename TVALUE>
    void set(const std::string &key, const TVALUE &value) {
        mTable[key] = value;
    }

    // no check, need user to ensure it's safety. perf
    template<typename TVALUE>
    void get(const std::string &key, TVALUE &value) {
        const auto &tmp = mTable.find(key);
        value = std::any_cast<TVALUE&>(tmp->second);
    }

    // not perf, but safe
    template<typename TVALUE>
    [[nodiscard]] bool getSafe(const std::string &key, TVALUE &value) {
        if (const auto &tmp = mTable.find(key); tmp != mTable.end()) {
            if (tmp->second.type() == std::type_index(typeid(TVALUE))) {
                value = std::any_cast<TVALUE&>(tmp->second);
                return true;
            }
        }
        return false;
    }

    void erase(const std::string &key) {
        mTable.erase(key);
    }

    void clear() {
        mTable.clear();
    }

    [[nodiscard]] bool empty() {
        return mTable.empty();
    }

 private:

    TTable mTable;
};

#define INITIALIZE_TEMPLATE(TYPE) \
    template void Bundle::set(const std::string &, const TYPE &); \
    template void Bundle::get(const std::string &, TYPE &); \
    template bool Bundle::getSafe(const std::string &, TYPE &)

INITIALIZE_TEMPLATE(std::string);
INITIALIZE_TEMPLATE(int64_t);
INITIALIZE_TEMPLATE(uint64_t);
INITIALIZE_TEMPLATE(int32_t);
INITIALIZE_TEMPLATE(uint32_t);
INITIALIZE_TEMPLATE(int16_t);
INITIALIZE_TEMPLATE(uint16_t);
INITIALIZE_TEMPLATE(int8_t);
INITIALIZE_TEMPLATE(uint8_t);

} // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_BUNDLE_HPP_

