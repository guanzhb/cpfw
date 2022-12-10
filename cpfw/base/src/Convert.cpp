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

#include "Convert.h"

namespace cpfw {

Convert::Convert() : Convert("", ExpressionEnum::EMPTY, 0.0f, 0, 0) {
}

Convert::Convert(const std::string &name, ExpressionEnum expression, float factor)
    : Convert(name, expression, factor, 0, 0) {
}

Convert::Convert(const std::string &name, ExpressionEnum expression,
            uint32_t widgetId, uint32_t elementId)
    : Convert(name, expression, 0.0f, widgetId, elementId) {
}

Convert::Convert(const std::string &name, ExpressionEnum expression, float factor,
        uint32_t widgetId, uint32_t elementId)
    : mName(name), mExpression(expression), mFactor(factor),
      mWidgetId(widgetId), mElementId(elementId) {
}

Convert::~Convert() {
}

const std::string& Convert::getName() const {
    return mName;
}

const ExpressionEnum Convert::getExpression() const {
    return mExpression;
}

const uint32_t Convert::getWidgetId() const {
    return mWidgetId;
}

const uint32_t Convert::getElementId() const {
    return mElementId;
}

const float Convert::getFactor() const {
    return mFactor;
}

}  // namespace cpfw

