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

#ifndef CPFW_BASE_INCLUDE_CONDITION_H_
#define CPFW_BASE_INCLUDE_CONDITION_H_

#include <string>

#include "ExpressionPool.h"

namespace cpfw {

class Condition {
 public:
    Condition();

    Condition(const std::string &name, uint32_t elementId, uint32_t widgetId,
              ExpressionEnum expression, int32_t left, int32_t right);

    ~Condition();

    const std::string& getName() const;

    const ExpressionEnum getExpression() const;

    const uint32_t getWidgetId() const;

    const uint32_t getElementId() const;

    int32_t getDefault() const;

    int32_t getLeft() const;

    int32_t getRight() const;

 private:
    const std::string mName;
    const ExpressionEnum mExpression;
    const uint32_t mWidgetId;
    const uint32_t mElementId;
    int32_t mLeft;
    int32_t mRight;
};

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_CONDITION_H_

