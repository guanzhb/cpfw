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

#ifndef CPFW_BASE_INCLUDE_CONVERT_H_
#define CPFW_BASE_INCLUDE_CONVERT_H_

#include <string>

#include "ExpressionPool.h"

namespace cpfw {

class Convert {
 public:
    Convert();

    Convert(std::string name,
            ExpressionEnum expression,
            float factor);

    Convert(std::string name,
            ExpressionEnum expression,
            std::string profileName,
            std::string elementName);

    ~Convert();

    const std::string& getName() const;

    const ExpressionEnum getExpression() const;

    const std::string& getProfileName() const;

    const std::string& getElementName() const;

    float getFactor() const;

 private:
    Convert(std::string name,
            ExpressionEnum expression,
            float factor,
            std::string profileName,
            std::string elementName);

 private:
    const std::string mName;
    const ExpressionEnum mExpression;
    const float mFactor;
    const std::string mProfileName;
    const std::string mElementName;
};

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_CONVERT_H_

