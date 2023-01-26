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
#ifndef CPFW_CORE_INCLUDE_DATAPARSER_H_
#define CPFW_CORE_INCLUDE_DATAPARSER_H_

#include <memory>
#include <string>

#include "tinyxml2.h"

#include "DataStore.h"

namespace cpfw {

class LogicDataParser {
 public:
    static const std::string ROOT;
    static const std::string TAG_WIDGETS;
    static const std::string TAG_WIDGET;
    static const std::string ATTR_WIDGET;
    static const std::string ATTR_NAME;
    static const std::string TAG_INVOKE_CHAIN;
    static const std::string TAG_INVOKE_CHAIN_PARENT;
    static const std::string TAG_INVOKE_CHAIN_CHILD;
    static const std::string ATTR_CONDITION;
    static const std::string TAG_CONDITIONS;
    static const std::string TAG_CONDITION;
    static const std::string ATTR_LOGIC;
    static const std::string ATTR_CONDITION_WHEN;
    static const std::string DEFAULT_NAME;
    static const std::string PROFILE_DELIM;
    static const std::string ATTR_CONDITION_WHEN_DELIM;
    static const std::string ATTR_CONDITION_VALUE_DELIM;
    static const std::string TAG_PROFILES;
    static const std::string ATTR_ID;
    static const std::string ATTR_BIND_TO;
    static const std::string TAG_PROFILE;
    static const std::string TAG_ELEMENT;
    static const std::string ATTR_MIN;
    static const std::string ATTR_MAX;
    static const std::string ATTR_CURRENT;
    static const std::string ATTR_TYPE;
    static const std::string TAG_CONVERTS;
    static const std::string TAG_CONVERT;
    static const std::string TAG_CALCULATE;
    static const std::string ATTR_ORIGIN;
    static const std::string ATTR_TARGET;
    static const std::string TAG_CONTEXT;

 public:
    LogicDataParser();

    LogicDataParser(const std::string &configurationFile,
               std::shared_ptr<DataStore> dataStore);

    ~LogicDataParser();

 private:
    void loadConfiguration(tinyxml2::XMLElement *root);

    void loadInvokeChain(tinyxml2::XMLElement *root);

    void loadConditions(tinyxml2::XMLElement *root);

    void loadProfile(tinyxml2::XMLElement *root);

    void loadDataConvert(tinyxml2::XMLElement *root);

 private:
    std::shared_ptr<DataStore> mDataStore;
};

}  // namespace cpfw

#endif  // CPFW_CORE_INCLUDE_DATAPARSER_H_

