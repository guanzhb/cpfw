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

#ifndef CPFW_BASE_INCLUDE_WIDGET_H_
#define CPFW_BASE_INCLUDE_WIDGET_H_

#include <any>
#include <memory>
#include <map>

#include "cpfw/base/include/DataStore.h"
#include "cpfw/base/include/StrategyLogic.h"
#include "cpfw/base/include/ExpressionPool.h"

namespace cpfw {

class DataStore;
class IStrategyLogic;

class Widget {
 public:
     Widget();

     explicit Widget(std::string name);

     Widget(std::string name, FUNCTION_0INT func);
     Widget(std::string name, FUNCTION_1INT func);
     Widget(std::string name, FUNCTION_2INT func);
     Widget(std::string name, FUNCTION_3INT func);
     Widget(std::string name, FUNCTION_4INT func);
     Widget(std::string name, FUNCTION_5INT func);

     virtual ~Widget();

     virtual void linkDataStore(std::shared_ptr<DataStore> dataStore);

     virtual const std::string& getName() const;

     virtual std::shared_ptr<DataStore> getDataStore() const;

     virtual int32_t check();

     virtual int32_t action();

     virtual int32_t swipe();

     virtual int32_t reset();

 private:
    const std::string mName;
    std::shared_ptr<DataStore> mStore;
    static std::map<ExpressionEnum,
        std::shared_ptr<IStrategyLogic>> mStrategy;
    std::any mFuncAction;
};

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_WIDGET_H_

