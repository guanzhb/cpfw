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
#ifndef CPFW_CORE_INCLUDE_STRATEGYCALCULATE_H_
#define CPFW_CORE_INCLUDE_STRATEGYCALCULATE_H_

#include <memory>
#include <string>
#include <vector>

#include "Convert.h"
#include "DataStore.h"

namespace cpfw {

class IStrategyCalculate {
 public:
    virtual int32_t handle(const uint32_t widgetId, const int32_t origin,
                           const Convert &convert, std::shared_ptr<DataStore> dataStore) = 0;
};

class StrategyCalculateDummy : public IStrategyCalculate {
 public:
    int32_t handle(const uint32_t widgetId, const int32_t origin,
                   const Convert &convert, std::shared_ptr<DataStore> dataStore) override;
};

class StrategyCalculateAddConst : public IStrategyCalculate {
 public:
    int32_t handle(const uint32_t widgetId, const int32_t origin,
                   const Convert &convert, std::shared_ptr<DataStore> dataStore) override;
};

class StrategyCalculateAddVariable : public IStrategyCalculate {
 public:
    int32_t handle(const uint32_t widgetId, const int32_t origin,
                   const Convert &convert, std::shared_ptr<DataStore> dataStore) override;
};

class StrategyCalculateSubConst : public IStrategyCalculate {
 public:
    int32_t handle(const uint32_t widgetId, const int32_t origin,
                   const Convert &convert, std::shared_ptr<DataStore> dataStore) override;
};

class StrategyCalculateSubVariable : public IStrategyCalculate {
 public:
    int32_t handle(const uint32_t widgetId, const int32_t origin,
                   const Convert &convert, std::shared_ptr<DataStore> dataStore) override;
};

class StrategyCalculateMulConst : public IStrategyCalculate {
 public:
    int32_t handle(const uint32_t widgetId, const int32_t origin,
                   const Convert &convert, std::shared_ptr<DataStore> dataStore) override;
};

class StrategyCalculateMulVariable : public IStrategyCalculate {
 public:
    int32_t handle(const uint32_t widgetId, const int32_t origin,
                   const Convert &convert, std::shared_ptr<DataStore> dataStore) override;
};

class StrategyCalculateDivConst : public IStrategyCalculate {
 public:
    int32_t handle(const uint32_t widgetId, const int32_t origin,
                   const Convert &convert, std::shared_ptr<DataStore> dataStore) override;
};

class StrategyCalculateDivVariable : public IStrategyCalculate {
 public:
    int32_t handle(const uint32_t widgetId, const int32_t origin,
                   const Convert &convert, std::shared_ptr<DataStore> dataStore) override;
};

class StrategyCalculatePool {
 public:
    static std::shared_ptr<IStrategyCalculate> getStrategy(ExpressionEnum expression);

 private:
    static std::map<ExpressionEnum, std::shared_ptr<IStrategyCalculate>> mStrategy;
    static std::shared_ptr<IStrategyCalculate> STRATEGY_DUMMY;
};  // StrategyCalculatePool

}  // namespace cpfw

#endif  // CPFW_CORE_INCLUDE_STRATEGYCALCULATE_H_
