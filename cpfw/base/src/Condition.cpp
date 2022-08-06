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

#include "cpfw/base/include/Condition.h"

namespace cpfw {

Condition::Condition() : Condition("", "", "", "", 0, 0) {
}

Condition::Condition(
        std::string name, std::string elementName,
        std::string profileName, std::string expression,
        int32_t left, int32_t right)
    : mName(name), mExpression(expression),
      mProfileName(profileName), mElementName(elementName),
      mLeft(left), mRight(right) {
}

Condition::~Condition() {
}

const std::string& Condition::getName() const {
    return mName;
}

const std::string& Condition::getExpression() const {
    return mExpression;
}

const std::string& Condition::getProfileName() const {
    return mProfileName;
}

const std::string& Condition::getElementName() const {
    return mElementName;
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

