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

#ifndef CPFW_BASE_INCLUDE_CONDITION_H_
#define CPFW_BASE_INCLUDE_CONDITION_H_

#include <string>

namespace cpfw {

class Condition {
 public:
    Condition();

    Condition(std::string name,
              std::string elementName,
              std::string profileName,
              std::string expression,
              int32_t left,
              int32_t right);

    ~Condition();

    const std::string& getName() const;

    const std::string& getExpression() const;

    const std::string& getProfileName() const;

    const std::string& getElementName() const;
    int32_t getDefault() const;

    int32_t getLeft() const;

    int32_t getRight() const;

 private:
    const std::string mName;
    const std::string mExpression;
    const std::string mProfileName;
    const std::string mElementName;
    int32_t mLeft;
    int32_t mRight;
};

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_CONDITION_H_

