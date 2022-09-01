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

#include "cpfw/base/include/Convert.h"

namespace cpfw {

Convert::Convert() : Convert("", ExpressionEnum::EMPTY, 0.0f, "", "") {
}

Convert::Convert(std::string name, ExpressionEnum expression, float factor)
    : Convert(name, expression, factor, "", "") {
}

Convert::Convert(std::string name, ExpressionEnum expression,
            std::string profileName, std::string elementName)
    : Convert(name, expression, 0.0f, profileName, elementName) {
}

Convert::Convert(
        std::string name, ExpressionEnum expression, float factor,
        std::string profileName, std::string elementName)
    : mName(name), mExpression(expression), mFactor(factor),
      mProfileName(profileName), mElementName(elementName) {
}

Convert::~Convert() {
}

const std::string& Convert::getName() const {
    return mName;
}

const ExpressionEnum Convert::getExpression() const {
    return mExpression;
}

const std::string& Convert::getProfileName() const {
    return mProfileName;
}

const std::string& Convert::getElementName() const {
    return mElementName;
}

float Convert::getFactor() const {
    return mFactor;
}

}  // namespace cpfw 
