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

#include "Condition.h"

namespace cpfw {

Condition::Condition() : Condition("", 0, 0, ExpressionEnum::EMPTY, 0, 0) {
}

Condition::Condition(
        const std::string &name, uint32_t elementId, uint32_t widgetId,
        ExpressionEnum expression, int32_t left, int32_t right)
    : mName(name), mExpression(expression), mWidgetId(widgetId),
      mElementId(elementId), mLeft(left), mRight(right) {
}

Condition::~Condition() {
}

const std::string& Condition::getName() const {
    return mName;
}

const ExpressionEnum Condition::getExpression() const {
    return mExpression;
}

const uint32_t Condition::getWidgetId() const {
    return mWidgetId;
}

const uint32_t Condition::getElementId() const {
    return mElementId;
}

int32_t Condition::getDefault() const {
    return mLeft;
}

int32_t Condition::getLeft() const {
    return mLeft;
}

int32_t Condition::getRight() const {
    return mRight;
}

}  // namespace cpfw

