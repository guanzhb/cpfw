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
#ifndef CPFW_CORE_INCLUDE_STRATEGYLOGIC_H_
#define CPFW_CORE_INCLUDE_STRATEGYLOGIC_H_

#include <memory>

#include "Condition.h"
#include "DataStore.h"

namespace cpfw {

class DataStore;
class Condition;

class IStrategyLogic {
 public:
    virtual int32_t handle(const Condition &condition, std::shared_ptr<DataStore> dataStore) = 0;
};

class StrategyLogicDummy : public IStrategyLogic {
 public:
    int32_t handle(const Condition &condition, std::shared_ptr<DataStore> dataStore) override;
};

class StrategyLogicEqual : public IStrategyLogic {
 public:
    int32_t handle(const Condition &condition, std::shared_ptr<DataStore> dataStore) override;
};

class StrategyLogicNotEqual : public IStrategyLogic {
 public:
    int32_t handle(const Condition &condition, std::shared_ptr<DataStore> dataStore) override;
};

class StrategyLogicInRange : public IStrategyLogic {
 public:
    int32_t handle(const Condition &condition, std::shared_ptr<DataStore> dataStore) override;
};

class StrategyLogicOutRange : public IStrategyLogic {
 public:
    int32_t handle(const Condition &condition, std::shared_ptr<DataStore> dataStore) override;
};

class StrategyLogicChange : public IStrategyLogic {
 public:
    int32_t handle(const Condition &condition, std::shared_ptr<DataStore> dataStore) override;
};

class StrategyLogicPool {
 public:
    static std::shared_ptr<IStrategyLogic> getStrategy(ExpressionEnum expression);

 private:
    static std::map<ExpressionEnum, std::shared_ptr<IStrategyLogic>> mStrategy;
    static std::shared_ptr<IStrategyLogic> STRATEGY_DUMMY;
};  // StrategyLogicPool

}  // namespace cpfw

#endif  // CPFW_CORE_INCLUDE_STRATEGYLOGIC_H_
