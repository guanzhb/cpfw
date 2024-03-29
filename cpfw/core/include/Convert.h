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
 * WITHOUT WARRANTIES OR ConvertS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef CPFW_CORE_INCLUDE_CONVERT_H_
#define CPFW_CORE_INCLUDE_CONVERT_H_

#include <string>

#include "ExpressionPool.h"

namespace cpfw {

struct Convert {
    Convert() : Convert("", ExpressionEnum::EMPTY, 0.0f, 0U, 0U) {
    }

    Convert(const std::string &name, ExpressionEnum expression, float factor)
        : Convert(name, expression, factor, 0U, 0U) {
    }

    Convert(const std::string &name, ExpressionEnum expression,
            uint32_t widgetId, uint32_t elementId)
        : Convert(name, expression, 0.0f, widgetId, elementId) {
    }

    Convert(const std::string &name_, ExpressionEnum expression_, float factor_,
            uint32_t widgetId_, uint32_t elementId_)
        : name(name_), expression(expression_), factor(factor_),
          widgetId(widgetId_), elementId(elementId_) {
    }

    ~Convert() {
    }

    const std::string name;
    const ExpressionEnum expression;
    const float factor;
    const uint32_t widgetId;
    const uint32_t elementId;
};

}  // namespace cpfw

#endif  // CPFW_CORE_INCLUDE_CONVERT_H_

