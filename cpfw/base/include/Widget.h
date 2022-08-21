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

#include "cpfw/base/include/DataStore.h"

#include <memory>

namespace cpfw {

class DataStore;

class Widget {
 public:
    static const std::string EXPRESSION_EQ;
    static const std::string EXPRESSION_NOT_EQ;
    static const std::string EXPRESSION_IN_RANGE;
    static const std::string EXPRESSION_CHANGE;
    static const std::string EXPRESSION_OUT_RANGE;
    static const std::string EXPRESSION_AND;
    static const std::string EXPRESSION_OR;

 public:
     Widget();

     explicit Widget(std::string name);

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
};

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_WIDGET_H_

