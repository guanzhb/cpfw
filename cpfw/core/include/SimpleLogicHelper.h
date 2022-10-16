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

#ifndef CPFW_CORE_INCLUDE_SIMPLELOGICHELPER_H_
#define CPFW_CORE_INCLUDE_SIMPLELOGICHELPER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "tinyxml2.h"
#include "Widget.h"

namespace cpfw {

using WidgetValue = std::vector<int32_t>;
using WidgetPair = std::pair<std::shared_ptr<Widget>, WidgetValue>; 

class SimpleLogicHelper {
 public:
    static const std::string ROOT;
    static const std::string TAG_STATE;
    static const std::string ATTR_NAME;
    static const std::string ATTR_VALUE;
    static const std::string TAG_CHILD;
    static const std::string ATTR_WIDGET;
    static const std::string ATTR_ID;
    static const std::string VALUE_DELIM;
    static const std::vector<WidgetPair> EMPTY_WIDGET_PAIR;
    static const std::string EMPTY_STATE;

 public:
    SimpleLogicHelper();

    ~SimpleLogicHelper();

    void addWidget(std::shared_ptr<Widget> widget);

    void loadConfiguration(const std::string &configurationFile);
    
    const std::string& getStateName(uint32_t stateId);
    const std::vector<WidgetPair>& getWidgetPair(uint32_t stateId);

private:
    void loadState(tinyxml2::XMLElement *root);

 private:
    std::map<uint32_t/*state id*/, std::string> mStateNameTable;
    std::map<uint32_t/*state id*/, std::vector<WidgetPair>> mStateTable;
    std::map<uint32_t/*widget id*/, std::shared_ptr<Widget>> mWidgetTable;
};

}  // namespace cpfw

#endif  // CPFW_CORE_INCLUDE_SIMPLELOGICHELPER_H_

