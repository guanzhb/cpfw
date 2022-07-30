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

#ifndef CPFW_BASE_INCLUDE_BASE_H__
#define CPFW_BASE_INCLUDE_BASE_H__

#include <stdint.h>
#include <string>
#include <vector>

#include "cpfw/base/include/DataStore.h"

namespace cpfw {

class DataStore;

enum class ElementType : uint32_t {
    PUBLIC = 0x01,
    NEED_CONVERT = 0x02,
};

struct Element {
    int32_t min;
    int32_t max;
    int32_t current;
    int32_t backup;
    uint32_t type;  // relate to ElementType
};

struct Profile {
    std::map<std::string, Element> elements;
    std::map<std::string, Profile> profile;
};

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_BASE_H__

