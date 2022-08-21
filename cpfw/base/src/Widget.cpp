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

#include "cpfw/base/include/Widget.h"

#include <iostream>

#include "cpfw/base/include/Condition.h"
#include "cpfw/base/include/Utils.h"

namespace cpfw {

const std::string Widget::EXPRESSION_EQ = "equal";
const std::string Widget::EXPRESSION_NOT_EQ = "not_equal";
const std::string Widget::EXPRESSION_IN_RANGE = "in_range";
const std::string Widget::EXPRESSION_OUT_RANGE = "out_range";
const std::string Widget::EXPRESSION_CHANGE = "change";
const std::string Widget::EXPRESSION_AND = "and";
const std::string Widget::EXPRESSION_OR = "or";

std::map<const std::string, std::shared_ptr<IExpressionStrategy>> Widget::mStrategy {
    {EXPRESSION_EQ, std::make_shared<ExpressionStrategyEqual>()},
    {EXPRESSION_NOT_EQ, std::make_shared<ExpressionStrategyNotEqual>()},
    {EXPRESSION_IN_RANGE, std::make_shared<ExpressionStrategyInRange>()},
    {EXPRESSION_OUT_RANGE, std::make_shared<ExpressionStrategyOutRange>()},
    {EXPRESSION_CHANGE, std::make_shared<ExpressionStrategyChange>()}
};

Widget::Widget() : Widget("") {
}

Widget::Widget(std::string name) : mName(name) {
}

Widget::~Widget() {
}

void Widget::linkDataStore(std::shared_ptr<DataStore> store) {
    mStore = store;
}

const std::string& Widget::getName() const {
    return mName;
}

std::shared_ptr<DataStore> Widget::getDataStore() const {
    return mStore;
}

int32_t Widget::check() {
    int32_t ret = 0;
    if (!mStore) {
        return ret;
    }
    auto &conditionPair = mStore->getCondition(mName);
    if (&DataStore::EMPTY_CONDITION == &conditionPair) {
        std::cout << "Widget[" << mName << "] no check" << std::endl;
        return 0;
    }
    std::string logic = conditionPair.first;
    for (auto itor : conditionPair.second) {
        Profile &profile = mStore->getProfile(itor.getProfileName());
        int32_t current = profile.elements[itor.getElementName()].current;
        const std::string &expressionIn = itor.getExpression();

        if (mStrategy.find(expressionIn) != mStrategy.end()) {
            ret = mStrategy[expressionIn]->handle(itor, mStore);
        }

        if (0 == ret && 0 == EXPRESSION_OR.compare(logic)) {
            break;
        } else if (0 != ret && 0 == EXPRESSION_AND.compare(logic)) {
            break;
        }
    }

    return ret;
}

int32_t Widget::action() {
    return 0;
}

int32_t Widget::swipe() {
    return 0;
}

int32_t Widget::reset() {
    Profile &profile = mStore->getProfile(getName());
    for (auto &itor : profile.elements) {
        itor.second.current = itor.second.backup;
    }
    return 0;
}

}  // namespace cpfw

