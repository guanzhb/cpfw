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
#define LOG_TAG "StrategyLogic"

#include "StrategyLogic.h"

#include "Log.hpp"

//#define LOG_VV
#ifdef LOG_VV
#define LOGVV LOGV
#else
#define LOGVV(...) do { } while(0)
#endif

namespace cpfw {

namespace {
    void tryLog(const char *name, const Condition &condition) {
        LOGVV("%s", name);
        LOGVV("condition, name:%s, widgetId:%lu, elementId:%lu",
              condition.name, condition.widgetId, condition.elementId);
    }
}  // namespace

int32_t StrategyLogicDummy::handle(
            const Condition &condition, std::shared_ptr<DataStore> dataStore) {
    tryLog("Dymmy", condition);
    Profile &profile = dataStore->getProfile(condition.widgetId);
    return 0;
}

int32_t StrategyLogicEqual::handle(
            const Condition &condition, std::shared_ptr<DataStore> dataStore) {
    tryLog("Equal", condition);
    Profile &profile = dataStore->getProfile(condition.widgetId);
    int32_t current = profile.elements[condition.elementId].current;
    return current == condition.value ? 0 : EINVAL;
}

int32_t StrategyLogicNotEqual::handle(
            const Condition &condition, std::shared_ptr<DataStore> dataStore) {
    tryLog("NotEqual", condition);
    Profile &profile = dataStore->getProfile(condition.widgetId);
    int32_t current = profile.elements[condition.elementId].current;
    return current != condition.value ? 0 : EINVAL;
}

int32_t StrategyLogicInRange::handle(
            const Condition &condition, std::shared_ptr<DataStore> dataStore) {
    tryLog("InRange", condition);
    Profile &profile = dataStore->getProfile(condition.widgetId);
    int32_t current = profile.elements[condition.elementId].current;
    return (current >= condition.left && current <= condition.right) ? 0 : EINVAL;
}

int32_t StrategyLogicOutRange::handle(
            const Condition &condition, std::shared_ptr<DataStore> dataStore) {
    tryLog("OutRange", condition);
    Profile &profile = dataStore->getProfile(condition.widgetId);
    int32_t current = profile.elements[condition.elementId].current;
    return (current < condition.left && current > condition.right) ? 0 : EINVAL;
}

int32_t StrategyLogicChange::handle(
            const Condition &condition, std::shared_ptr<DataStore> dataStore) {
    tryLog("Change", condition);
    Profile &profile = dataStore->getProfile(condition.widgetId);
    return (profile.elements[condition.elementId].flag) ? 0 : EINVAL;
}

std::map<ExpressionEnum, std::shared_ptr<IStrategyLogic>> StrategyLogicPool::mStrategy {
    { ExpressionEnum::EQUAL, std::make_shared<StrategyLogicEqual>() },
    { ExpressionEnum::NOT_EQUAL, std::make_shared<StrategyLogicNotEqual>() },
    { ExpressionEnum::IN_RANGE, std::make_shared<StrategyLogicInRange>() },
    { ExpressionEnum::OUT_RANGE, std::make_shared<StrategyLogicOutRange>() },
    { ExpressionEnum::CHANGE, std::make_shared<StrategyLogicChange>() }
};

std::shared_ptr<IStrategyLogic> StrategyLogicPool::STRATEGY_DUMMY
    = std::make_shared<StrategyLogicDummy>();

std::shared_ptr<IStrategyLogic> StrategyLogicPool::getStrategy(ExpressionEnum expression) {
    return mStrategy.find(expression) != mStrategy.end() ? mStrategy[expression] : STRATEGY_DUMMY;
}

}  // namespace cpfw

