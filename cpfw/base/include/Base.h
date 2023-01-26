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

#ifndef CPFW_BASE_INCLUDE_BASE_H_
#define CPFW_BASE_INCLUDE_BASE_H_

#include <functional>
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

namespace cpfw {

enum DataType : uint8_t {
    INT8,
    UINT8,
    INT16,
    UINT16,
    INT32,
    UINT32,
    INT64,
    UINT64,

    STRING,
    CHAR,
};

// why not enum class? may have multi vaild type, avoid static_cast
enum ElementType : uint32_t {
    PUBLIC = 0x01,
    NEED_CONVERT = 0x02,
};

struct Element {
    int32_t min;
    int32_t max;
    int32_t current;
    int32_t backup;
    uint32_t type;  // relate to ElementType
    bool flag;  // change or not
};

struct Profile {
    std::map<uint32_t, Element> elements;
    std::map<uint32_t, Profile> profile;  // TODO profile in profile
};

const std::string KEY_WIDGET = "widget";
const std::string KEY_ELEMENT = "element";
const std::string KEY_VALUE = "value";

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_BASE_H_

