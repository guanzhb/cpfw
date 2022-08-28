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

#include "cpfw/base/include/StrategyCalculate.h"

#include <iostream>

namespace cpfw {

int32_t StrategyCalculateDummy::handle(const std::string &context,
        const int32_t origin, const Convert &convert,
        std::shared_ptr<DataStore> dataStore) {
    return origin;
}

int32_t StrategyCalculateAddConst::handle(const std::string &context,
        const int32_t origin, const Convert &convert,
        std::shared_ptr<DataStore> dataStore) {
    return origin + convert.getFactor();
}

int32_t StrategyCalculateAddVariable::handle(const std::string &context,
        const int32_t origin, const Convert &convert,
        std::shared_ptr<DataStore> dataStore) {
    return origin
        + dataStore->getProfile(convert.getProfileName())
              .elements[convert.getElementName()].current;
}

int32_t StrategyCalculateSubConst::handle(const std::string &context,
        const int32_t origin, const Convert &convert,
        std::shared_ptr<DataStore> dataStore) {
    return origin - convert.getFactor();
}

int32_t StrategyCalculateSubVariable::handle(const std::string &context,
        const int32_t origin, const Convert &convert,
        std::shared_ptr<DataStore> dataStore) {
    return origin
        - dataStore->getProfile(convert.getProfileName())
              .elements[convert.getElementName()].current;
}

int32_t StrategyCalculateMulConst::handle(const std::string &context,
        const int32_t origin, const Convert &convert,
        std::shared_ptr<DataStore> dataStore) {
    return origin * convert.getFactor();
}

int32_t StrategyCalculateMulVariable::handle(const std::string &context,
        const int32_t origin, const Convert &convert,
        std::shared_ptr<DataStore> dataStore) {
    return origin
        * dataStore->getProfile(convert.getProfileName())
              .elements[convert.getElementName()].current;
}

int32_t StrategyCalculateDivConst::handle(const std::string &context,
        const int32_t origin, const Convert &convert,
        std::shared_ptr<DataStore> dataStore) {
    return origin / convert.getFactor();
}

int32_t StrategyCalculateDivVariable::handle(const std::string &context,
        const int32_t origin, const Convert &convert,
        std::shared_ptr<DataStore> dataStore) {
    return origin
        / dataStore->getProfile(convert.getProfileName())
              .elements[convert.getElementName()].current;
}

std::map<ExpressionEnum,
    std::shared_ptr<IStrategyCalculate>> StrategyCalculatePool::mStrategy {
        {ExpressionEnum::ADD_CONST,
            std::make_shared<StrategyCalculateAddConst>()},
        {ExpressionEnum::ADD_VARIABLE,
            std::make_shared<StrategyCalculateAddVariable>()},
        {ExpressionEnum::SUB_CONST,
            std::make_shared<StrategyCalculateSubConst>()},
        {ExpressionEnum::SUB_VARIABLE,
            std::make_shared<StrategyCalculateSubVariable>()},
        {ExpressionEnum::MUL_CONST,
            std::make_shared<StrategyCalculateMulConst>()},
        {ExpressionEnum::MUL_VARIABLE,
            std::make_shared<StrategyCalculateMulVariable>()},
        {ExpressionEnum::DIV_CONST,
            std::make_shared<StrategyCalculateDivConst>()},
        {ExpressionEnum::DIV_VARIABLE,
            std::make_shared<StrategyCalculateDivVariable>()},
};

std::shared_ptr<IStrategyCalculate> StrategyCalculatePool::STRATEGY_DUMMY
    = std::make_shared<StrategyCalculateDummy>();

std::shared_ptr<IStrategyCalculate>
StrategyCalculatePool::getStrategy(ExpressionEnum expression) {
    return mStrategy.find(expression) != mStrategy.end()
        ? mStrategy[expression] : STRATEGY_DUMMY;
}

}  // namespace cpfw

