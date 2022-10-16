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

#include <map>
#include <string>

namespace cpfw {

enum class ExpressionEnum : uint64_t {
    EQUAL = 0,
    NOT_EQUAL,
    IN_RANGE,
    OUT_RANGE,
    GREATER_THAN,
    LESS_THAN,
    CHANGE,

    TO_CONST,
    TO_VARIABLE,
    ADD_CONST,
    ADD_VARIABLE,
    SUB_CONST,
    SUB_VARIABLE,
    MUL_CONST,
    MUL_VARIABLE,
    DIV_CONST,
    DIV_VARIABLE,

    AND,
    OR,

    EMPTY,
};

const std::string EXPRESSION_EQUAL = "equal";
const std::string EXPRESSION_NOT_EQUAL = "not_equal";
const std::string EXPRESSION_IN_RANGE = "in_range";
const std::string EXPRESSION_OUT_RANGE = "out_range";
const std::string EXPRESSION_GREATER_THAN = "greater_than";
const std::string EXPRESSION_LESS_THAN = "less_than";
const std::string EXPRESSION_CHANGE = "change";

const std::string EXPRESSION_TO_CONST = "to_const";
const std::string EXPRESSION_TO_VARIABLE = "to_variable";
const std::string EXPRESSION_ADD_CONST = "add_const";
const std::string EXPRESSION_ADD_VARIABLE = "add_variable";
const std::string EXPRESSION_SUB_CONST = "sub_const";
const std::string EXPRESSION_SUB_VARIABLE = "sub_variable";
const std::string EXPRESSION_MUL_CONST = "mul_const";
const std::string EXPRESSION_MUL_VARIABLE = "mul_variable";
const std::string EXPRESSION_DIV_CONST = "div_const";
const std::string EXPRESSION_DIV_VARIABLE = "div_variable";

const std::string EXPRESSION_AND = "and";
const std::string EXPRESSION_OR = "or";

class ExpressionPool {
 public:
    static ExpressionEnum getEnum(const std::string &expressionStr);

 private:
    static std::map<std::string, ExpressionEnum> mTable;
};

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_EXPRESSIONPOOL_H__

