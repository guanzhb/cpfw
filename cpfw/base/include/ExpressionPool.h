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
#ifndef CPFW_BASE_INCLUDE_EXPRESSIONPOOL_H__
#define CPFW_BASE_INCLUDE_EXPRESSIONPOOL_H__

#include <string>
#include <map>

namespace cpfw {

enum class ExpressionEnum : uint64_t {
    EQUAL = 0,
    NOT_EQUAL,
    IN_RANGE,
    CHANGE,
    OUT_RANGE,
    AND,
    OR,
    EMPTY,
};

const std::string EXPRESSION_EQUAL = "equal";
const std::string EXPRESSION_NOT_EQUAL = "not_equal";
const std::string EXPRESSION_IN_RANGE = "in_range";
const std::string EXPRESSION_OUT_RANGE = "out_range";
const std::string EXPRESSION_CHANGE = "change";
const std::string EXPRESSION_AND = "and";
const std::string EXPRESSION_OR = "or";

class ExpressionPool {
 public:
    static ExpressionEnum getEnum(std::string expressionStr);

 private:
    static std::map<std::string, ExpressionEnum> mTable;
};

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_EXPRESSIONPOOL_H__

