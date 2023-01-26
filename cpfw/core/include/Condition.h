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
#ifndef CPFW_CORE_INCLUDE_CONDITION_H_
#define CPFW_CORE_INCLUDE_CONDITION_H_

#include <string>

#include "ExpressionPool.h"

namespace cpfw {

struct Condition {
    Condition() : Condition("", 0U, 0U, ExpressionEnum::EMPTY, 0, 0) {
    }

    Condition(const std::string &name_, uint32_t elementId_, uint32_t widgetId_,
              ExpressionEnum expression_, int32_t left_, int32_t right_)
        : name(name_), expression(expression_), widgetId(widgetId_),
          elementId(elementId_), left(left_), right(right_), value(left_) {
    }

    ~Condition() {
    }

    const std::string name;
    const ExpressionEnum expression;
    const uint32_t widgetId;
    const uint32_t elementId;
    const int32_t left;
    const int32_t right;
    const int32_t value;
};

}  // namespace cpfw

#endif  // CPFW_CORE_INCLUDE_CONDITION_H_

