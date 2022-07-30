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

#include "cpfw/base/include/Widget.h"

#include <iostream>

#include "cpfw/base/include/Condition.h"
#include "cpfw/base/include/Utils.hpp"

namespace cpfw {

const std::string Widget::EXPRESSION_EQ = "eq";
const std::string Widget::EXPRESSION_NOT_EQ = "not_eq";
const std::string Widget::EXPRESSION_IN_RANGE = "in_range";
const std::string Widget::EXPRESSION_OUT_RANGE = "out_range";
const std::string Widget::EXPRESSION_AND = "and";
const std::string Widget::EXPRESSION_OR = "or";

Widget::Widget() : Widget("") {
}

Widget::Widget(std::string name) : mName(name) {
}

Widget::Widget(std::string name, std::shared_ptr<DataStore> store)
        : mName(name), mStore(store) {
}

Widget::~Widget() {
}

std::string Widget::getName() const {
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
    auto conditionPair = mStore->getCondition(mName);
    for (auto &itor : conditionPair) {
        auto &c = itor.second;
        const std::string &profileName = c.getProfileName();
        const std::string &elementName = c.getElementName();
        const std::string &expressionIn = c.getExpression();
        Profile* profile = mStore->getProfile(profileName);
        int32_t current = profile->elements[elementName].current;
        if (0 == EXPRESSION_EQ.compare(expressionIn)) {
             if (current != c.getDefault()) {
                 ret = EINVAL;  // TODO(guanzhb) LOGI
             }
        } else if (0 == EXPRESSION_NOT_EQ.compare(expressionIn)) {
             if (current == c.getDefault()) {
                 ret = EINVAL;  // TODO(guanzhb) LOGI
             }
        } else if (0 == EXPRESSION_IN_RANGE.compare(expressionIn)) {
             if (current < c.getLeft() || current > c.getRight()) {
                 ret = EINVAL;  // TODO(guanzhb) LOGI
             }
        } else if (0 == EXPRESSION_OUT_RANGE.compare(expressionIn)) {
             if (current > c.getLeft() && current < c.getRight()) {
                 ret = EINVAL;  // TODO(guanzhb) LOGI
             }
        }
        const std::string &expressionBetween = itor.first;
        if (0 == EXPRESSION_OR.compare(expressionBetween)) {
            break;
        } else if (0 == EXPRESSION_AND.compare(expressionBetween)) {

        }
        if (0 != ret) {
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

}  // namespace cpfw

