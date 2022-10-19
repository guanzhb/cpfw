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

#define TAG "Widget"

#include "Widget.h"

#include "Base.h"
#include "Condition.h"
#include "Log.hpp"
#include "Utils.h"

namespace cpfw {

Widget::Widget() : Widget("", 0) {
}

Widget::Widget(const std::string &name, uint32_t id) : Widget(name, id, nullptr) {
}

Widget::Widget(const std::string &name, uint32_t id, TCallback callback)
    : mName(name), mId(id), mCallback(callback) {
}

Widget::~Widget() {
}

void Widget::linkDataStore(std::shared_ptr<DataStore> store) {
    mStore = store;
}

const std::string& Widget::getName() const {
    return mName;
}

const uint32_t Widget::getId() const {
    return mId;
}

std::shared_ptr<DataStore> Widget::getDataStore() const {
    return mStore;
}

int32_t Widget::check() {
    int32_t ret = 0;
    if (!mStore) {
        return ret;
    }
    auto &conditionPair = mStore->getCondition(mId);
    if (&DataStore::EMPTY_CONDITION == &conditionPair) {
        LOGI("widget " + mName + " no check");
        return 0;
    }
    auto &logic = conditionPair.first;
    for (auto itor : conditionPair.second) {
        const auto &expressionIn = itor.getExpression();
        using SLP = StrategyLogicPool;
        ret = SLP::getStrategy(expressionIn)->handle(itor, mStore);
        if (0 == ret && ExpressionEnum::OR == logic) {
            break;
        } else if (0 != ret && ExpressionEnum::AND == logic) {
            break;
        }
    }

    return ret;
}

int32_t Widget::action() {
    LOGD("widget " + mName + " action");
    constexpr uint32_t type = ElementType::PUBLIC;
    auto bindId = mStore->getBind(getId());
    auto values = parseProfile(mStore->getProfile(getId()), type, getId(), mStore);
    if (values.empty() && (bindId != DataStore::EMPTY_BIND)) {
        values = parseProfile(mStore->getProfile(bindId), type, getId(), mStore);
    }
    if (nullptr != mCallback) {
        return std::invoke(mCallback, values);
    }
    if (bindId != DataStore::EMPTY_BIND) {
        return std::invoke(mStore->getWidget(bindId).value()->getCallback(), values);
    }

    return 0;
}

int32_t Widget::swipe() {
    return 0;
}

int32_t Widget::reset() {
    Profile &profile = mStore->getProfile(getId());
    for (auto &itor : profile.elements) {
        itor.second.current = itor.second.backup;
    }
    return 0;
}

const TCallback& Widget::getCallback() const {
    return mCallback;
}

}  // namespace cpfw

