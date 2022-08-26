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

#include "cpfw/base/include/Base.h"
#include "cpfw/base/include/Condition.h"
#include "cpfw/base/include/Utils.h"

namespace cpfw {

std::map<ExpressionEnum,
    std::shared_ptr<IExpressionStrategy>> Widget::mStrategy {
        {ExpressionEnum::EQUAL,
            std::make_shared<ExpressionStrategyEqual>()},
        {ExpressionEnum::NOT_EQUAL,
            std::make_shared<ExpressionStrategyNotEqual>()},
        {ExpressionEnum::IN_RANGE,
            std::make_shared<ExpressionStrategyInRange>()},
        {ExpressionEnum::OUT_RANGE,
            std::make_shared<ExpressionStrategyOutRange>()},
        {ExpressionEnum::CHANGE,
            std::make_shared<ExpressionStrategyChange>()}
};


Widget::Widget() : Widget("") {
}

Widget::Widget(std::string name) : mName(name) {
}

Widget::Widget(std::string name, FUNCTION_0INT funcAction)
    : mName(name), mFuncAction(funcAction) {
}

Widget::Widget(std::string name, FUNCTION_1INT funcAction)
    : mName(name), mFuncAction(funcAction) {
}

Widget::Widget(std::string name, FUNCTION_2INT funcAction)
    : mName(name), mFuncAction(funcAction) {
}

Widget::Widget(std::string name, FUNCTION_3INT funcAction)
    : mName(name), mFuncAction(funcAction) {
}

Widget::Widget(std::string name, FUNCTION_4INT funcAction)
    : mName(name), mFuncAction(funcAction) {
}

Widget::Widget(std::string name, FUNCTION_5INT funcAction)
    : mName(name), mFuncAction(funcAction) {
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
    auto &logic = conditionPair.first;
    for (auto itor : conditionPair.second) {
        const auto &expressionIn = itor.getExpression();

        if (mStrategy.find(expressionIn) != mStrategy.end()) {
            ret = mStrategy[expressionIn]->handle(itor, mStore);
        }

        if (0 == ret && ExpressionEnum::OR == logic) {
            break;
        } else if (0 != ret && ExpressionEnum::AND == logic) {
            break;
        }
    }

    return ret;
}

int32_t Widget::action() {
    std::cout << "widget " << getName()
        << " action time: " << cpfw::getCurrentTimeMs() << std::endl;
    uint32_t type = static_cast<uint32_t>(ElementType::PUBLIC);
    auto values = parseProfile(
        mStore->getProfile(getName()), type, getName(), mStore);
    int32_t ret = 0;
    switch (values.size()) {
    case 0:
        ret = std::invoke(std::any_cast<FUNCTION_0INT>(mFuncAction));
        break;
    case 1:
        ret = std::invoke(std::any_cast<FUNCTION_1INT>(mFuncAction), values[0]);
        break;
    case 2:
        ret = std::invoke(std::any_cast<FUNCTION_2INT>(mFuncAction),
                          values[0], values[1]);
    case 3:
        ret = std::invoke(std::any_cast<FUNCTION_3INT>(mFuncAction),
                          values[0], values[1], values[2]);
    case 4:
        ret = std::invoke(std::any_cast<FUNCTION_4INT>(mFuncAction),
                          values[0], values[1], values[2], values[3]);
        break;
    case 5:
        ret = std::invoke(std::any_cast<FUNCTION_5INT>(mFuncAction),
                          values[0], values[1], values[2], values[3], values[4]);
        break;
    default:
        break;
    }
    std::cout << "Widget action status: " << ret << std::endl;
    return ret;
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

