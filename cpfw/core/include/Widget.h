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
#ifndef CPFW_CORE_INCLUDE_WIDGET_H_
#define CPFW_CORE_INCLUDE_WIDGET_H_

#include <any>
#include <map>
#include <memory>

#include "Bundle.hpp"
#include "DataStore.h"
#include "ExpressionPool.h"
#include "StrategyLogic.h"

namespace cpfw {

class DataStore;
class IStrategyLogic;

namespace {
    using TCallback = std::function<int32_t(Bundle&)>;
}

class Widget {
 public:
    Widget();

    explicit Widget(const std::string &name, uint32_t id);

    explicit Widget(const std::string &name, uint32_t id, TCallback callback);

    virtual ~Widget();

    virtual void linkDataStore(std::shared_ptr<DataStore> dataStore);

    virtual const std::string& getName() const;

    virtual const uint32_t getId() const;

    virtual std::shared_ptr<DataStore> getDataStore() const;

    virtual int32_t check();

    virtual int32_t action();

    virtual int32_t swipe();

    virtual int32_t reset();

    virtual const TCallback& getCallback() const;

 private:
    const std::string mName;
    const uint32_t mId;
    std::shared_ptr<DataStore> mStore;
    static std::map<ExpressionEnum, std::shared_ptr<IStrategyLogic>> mStrategy;
    const TCallback mCallback;
};

}  // namespace cpfw

#endif  // CPFW_CORE_INCLUDE_WIDGET_H_

