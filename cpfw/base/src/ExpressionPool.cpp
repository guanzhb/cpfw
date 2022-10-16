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
#include "ExpressionPool.h"

namespace cpfw {

std::map<std::string, ExpressionEnum> ExpressionPool::mTable {
    {EXPRESSION_EQUAL, ExpressionEnum::EQUAL},
    {EXPRESSION_NOT_EQUAL, ExpressionEnum::NOT_EQUAL},
    {EXPRESSION_IN_RANGE, ExpressionEnum::IN_RANGE},
    {EXPRESSION_OUT_RANGE, ExpressionEnum::OUT_RANGE},
    {EXPRESSION_GREATER_THAN, ExpressionEnum::GREATER_THAN},
    {EXPRESSION_LESS_THAN, ExpressionEnum::LESS_THAN},
    {EXPRESSION_CHANGE, ExpressionEnum::CHANGE},

    {EXPRESSION_TO_CONST, ExpressionEnum::TO_CONST},
    {EXPRESSION_TO_VARIABLE, ExpressionEnum::TO_VARIABLE},
    {EXPRESSION_ADD_CONST, ExpressionEnum::ADD_CONST},
    {EXPRESSION_ADD_VARIABLE, ExpressionEnum::ADD_VARIABLE},
    {EXPRESSION_SUB_CONST, ExpressionEnum::SUB_CONST},
    {EXPRESSION_SUB_VARIABLE, ExpressionEnum::SUB_VARIABLE},
    {EXPRESSION_MUL_CONST, ExpressionEnum::MUL_CONST},
    {EXPRESSION_MUL_VARIABLE, ExpressionEnum::MUL_VARIABLE},
    {EXPRESSION_DIV_CONST, ExpressionEnum::DIV_CONST},
    {EXPRESSION_DIV_VARIABLE, ExpressionEnum::DIV_VARIABLE},

    {EXPRESSION_AND, ExpressionEnum::AND},
    {EXPRESSION_OR, ExpressionEnum::OR},
};
    
ExpressionEnum ExpressionPool::getEnum(const std::string &expressionStr) {
    return mTable[expressionStr];  // XXX crash when exec if not find
}

}  // namespace cpfw

