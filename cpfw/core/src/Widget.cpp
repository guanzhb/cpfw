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
#define LOG_TAG "Widget"

#include "Widget.h"

#include "Base.h"
#include "Condition.h"
#include "Log.hpp"

#include "StrategyCalculate.h"

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

int32_t Widget::process() {
    int32_t ret = 0;
    if ((ret = check()) != 0) {
        LOGE("check for name:%s, errno:%d", mName.c_str(), ret);
        return ret;
    }

    if ((ret = action()) != 0) {
        reset();
        LOGE("action for name:%s, errno:%d", mName.c_str(), ret);
        return ret;
    }

    swipe();

    return ret;
}

int32_t Widget::check() {
    int32_t ret = 0;
    if (!mStore) {
        return ret;
    }

    auto &conditionPair = mStore->getCondition(mId);
    if (&DataStore::EMPTY_CONDITION == &conditionPair) {
        LOGD("widget %s no check", mName.c_str());
        return 0;
    }

    auto &logic = conditionPair.first;
    for (auto itor : conditionPair.second) {
        const auto &expressionIn = itor.expression;
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

std::vector<int32_t> Widget::parseProfile(const Profile &profile, uint32_t type,
        uint32_t widgetId, std::shared_ptr<DataStore> store) {
    std::vector<int32_t> ret;
    if (!store) {
        return ret;
    }
    auto &elements = profile.elements;
    uint32_t i = 0;
    std::for_each(elements.begin(), elements.end(),
        [&] (auto &data) -> void {
            auto &d = data.second;
            if (0 == (type & ElementType::PUBLIC) || (0 == (d.type & ElementType::PUBLIC))) {
                return;
            }
            int32_t current = d.current;
            if (0 != (d.type & ElementType::NEED_CONVERT)) {
                current = store->getConvertedData(widgetId, current);
                auto& converts = store->getConvertTable(widgetId);
                for (auto &c : converts) {
                    current = StrategyCalculatePool::getStrategy(c.expression)
                                  ->handle(widgetId, current, c, store);
                }
            }
            ret.push_back(current);
            ++i;
    });
    return ret;
}

int32_t Widget::action() {
    LOGD("widget %s action", mName.c_str());
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

