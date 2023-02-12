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
 * WITHOUT WARRANTIES OR CONDITIONS  OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_LOG_TAG "LogicDataParser"

#include "LogicDataParser.h"

#include "ExpressionPool.h"
#include "Log.hpp"

namespace cpfw {

const std::string LogicDataParser::ROOT = "cpfw";
const std::string LogicDataParser::TAG_WIDGETS = "widgets";
const std::string LogicDataParser::TAG_WIDGET ="widget";
const std::string LogicDataParser::ATTR_WIDGET ="widget";
const std::string LogicDataParser::ATTR_NAME = "name";
const std::string LogicDataParser::TAG_INVOKE_CHAIN = "invokeChains";
const std::string LogicDataParser::TAG_INVOKE_CHAIN_PARENT = "parent";
const std::string LogicDataParser::TAG_INVOKE_CHAIN_CHILD = "child";
const std::string LogicDataParser::ATTR_CONDITION = "condition";
const std::string LogicDataParser::TAG_CONDITIONS = "conditions";
const std::string LogicDataParser::TAG_CONDITION = "condition";
const std::string LogicDataParser::ATTR_LOGIC = "logic";
const std::string LogicDataParser::ATTR_CONDITION_WHEN = "when";
const std::string LogicDataParser::DEFAULT_NAME = "default";
const std::string LogicDataParser::PROFILE_DELIM = ";";
const std::string LogicDataParser::ATTR_CONDITION_WHEN_DELIM = "@";
const std::string LogicDataParser::ATTR_CONDITION_VALUE_DELIM = ",";
const std::string LogicDataParser::TAG_PROFILES = "profiles";
const std::string LogicDataParser::TAG_PROFILE = "profile";
const std::string LogicDataParser::ATTR_ID = "id";
const std::string LogicDataParser::ATTR_BIND_TO = "bindTo";
const std::string LogicDataParser::TAG_ELEMENT = "element";
const std::string LogicDataParser::ATTR_MIN = "min";
const std::string LogicDataParser::ATTR_MAX = "max";
const std::string LogicDataParser::ATTR_CURRENT = "current";
const std::string LogicDataParser::ATTR_FLAG = "flag";
const std::string LogicDataParser::TAG_CONTEXT = "context";
const std::string LogicDataParser::TAG_CONVERTS = "converts";
const std::string LogicDataParser::TAG_CONVERT = "convert";
const std::string LogicDataParser::TAG_CALCULATE = "calculate";
const std::string LogicDataParser::ATTR_ORIGIN = "origin";
const std::string LogicDataParser::ATTR_TARGET = "target";

LogicDataParser::LogicDataParser() {
}

LogicDataParser::LogicDataParser(const std::string &configurationFile,
        std::shared_ptr<DataStore> dataStore)
        : mDataStore(dataStore) {
    LOGD("LogicDataParser load %s begin", configurationFile.c_str());
    tinyxml2::XMLDocument doc = tinyxml2::XMLDocument();
    tinyxml2::XMLError err = doc.LoadFile(configurationFile.c_str());
    if (err != tinyxml2::XML_SUCCESS) {
        LOGE("read file error!");
        return;
    }

    tinyxml2::XMLElement *root = doc.RootElement();
    loadConfiguration(root);

    LOGD("LogicDataParser load %s end", configurationFile.c_str());
}

LogicDataParser::~LogicDataParser() {
}

void LogicDataParser::loadConfiguration(tinyxml2::XMLElement *root) {
    loadProfile(root);
    loadInvokeChain(root);
    loadConditions(root);
    loadDataConvert(root);
}

void LogicDataParser::loadInvokeChain(tinyxml2::XMLElement *root) {
    LOGD("loadInvokeChain begin");
    tinyxml2::XMLElement *surface = root->FirstChildElement(TAG_INVOKE_CHAIN.c_str());
    tinyxml2::XMLElement *surfaceParent
        = surface->FirstChildElement(TAG_INVOKE_CHAIN_PARENT.c_str());

    while (surfaceParent) {
        const char* parentWidgetName = surfaceParent->Attribute(ATTR_WIDGET.c_str());
        tinyxml2::XMLElement *surfaceChild
            = surfaceParent->FirstChildElement(TAG_INVOKE_CHAIN_CHILD.c_str());
        TINVOKE_CHAIN chains;
        while (surfaceChild) {
            const char* childWidgetName = surfaceChild->Attribute(ATTR_WIDGET.c_str());
            chains.push_back(mDataStore->getIdWithStr(childWidgetName).value());
            surfaceChild = surfaceChild->NextSiblingElement();
        }
        mDataStore->addInvokeChain(
            mDataStore->getIdWithStr(parentWidgetName).value(), chains);
        surfaceParent = surfaceParent->NextSiblingElement();
    }
}

static Condition parseCondition(DataStore *dataStore, std::string name, std::string strs) {
    std::vector<std::string> delim = {"@", ";", ":", ",", "@#$"/*end sentinel*/};
    std::vector<std::string> ss;

    int32_t i = 0;

    size_t pos = strs.find(delim[i]);
    while (pos != strs.npos) {
        std::string temp = strs.substr(0, pos);
        ss.push_back(temp);
        strs = strs.substr(pos+1, strs.size());
        ++i;
        pos = strs.find(delim[i]);
    }
    ss.push_back(strs);
    switch (ss.size()) {
        case 4:
            return Condition(name,
                             dataStore->getIdWithStr(ss[0]).value(),
                             dataStore->getIdWithStr(ss[1]).value(),
                             ExpressionPool::getEnum(ss[2]),
                             std::atoi(ss[3].c_str()),
                             std::atoi(ss[3].c_str()));
        case 3:
            return Condition(name,
                             dataStore->getIdWithStr(ss[0]).value(),
                             dataStore->getIdWithStr(ss[1]).value(),
                             ExpressionPool::getEnum(ss[2]),
                             0,
                             0);
    }
    return Condition(name,
                     dataStore->getIdWithStr(ss[0]).value(),
                     dataStore->getIdWithStr(ss[1]).value(),
                     ExpressionPool::getEnum(ss[2]),
                     std::atoi(ss[3].c_str()),
                     std::atoi(ss[4].c_str()));
}

void LogicDataParser::loadConditions(tinyxml2::XMLElement *root) {
    LOGD("loadCondition begin");
    tinyxml2::XMLElement *surface = root->FirstChildElement(TAG_CONDITIONS.c_str());
    tinyxml2::XMLElement *surfaceCondition
        = surface->FirstChildElement(TAG_CONDITION.c_str());

    while (surfaceCondition) {
        tinyxml2::XMLElement *surfaceElement
            = surfaceCondition->FirstChildElement(TAG_ELEMENT.c_str());
        const char* widgetName = surfaceCondition->Attribute(ATTR_WIDGET.c_str());
        const char* logic = surfaceCondition->Attribute(ATTR_LOGIC.c_str());
        std::vector<Condition> condition;
        while (surfaceElement) {
            const char* when = surfaceElement->Attribute(ATTR_CONDITION_WHEN.c_str());
            Condition c = parseCondition(mDataStore.get(), widgetName, when);
            condition.push_back(c);
            surfaceElement = surfaceElement->NextSiblingElement();
        }
        mDataStore->addCondition(widgetName,
                        std::make_pair(ExpressionPool::getEnum(logic), condition));
        surfaceCondition = surfaceCondition->NextSiblingElement();
    }
}

void LogicDataParser::loadProfile(tinyxml2::XMLElement *root) {
    LOGD("loadProfile begin");
    tinyxml2::XMLElement *surface = root->FirstChildElement(TAG_PROFILES.c_str());
    tinyxml2::XMLElement *surfaceProfile
        = surface->FirstChildElement(TAG_PROFILE.c_str());

    while (surfaceProfile) {
        const char* widgetName = surfaceProfile->Attribute(ATTR_WIDGET.c_str());
        uint32_t widgetId = 0;
        surfaceProfile->QueryUnsignedAttribute(ATTR_ID.c_str(), &widgetId);
        mDataStore->addStrIdPair(widgetName, widgetId);

        uint32_t bindId = 0;
        surfaceProfile->QueryUnsignedAttribute(ATTR_BIND_TO.c_str(), &bindId);
        if (bindId != DataStore::EMPTY_BIND) {
            mDataStore->addBind(widgetId, bindId);
        }

        Profile profile;
        tinyxml2::XMLElement *surfaceElement
            = surfaceProfile->FirstChildElement(TAG_ELEMENT.c_str());
        while (surfaceElement) {
            const char* name = surfaceElement->Attribute(ATTR_NAME.c_str());
            uint32_t id = 0;
            surfaceElement->QueryUnsignedAttribute(ATTR_ID.c_str(), &id);
            int32_t min = 0;
            surfaceElement->QueryIntAttribute(ATTR_MIN.c_str(), &min);
            int32_t max = 0;
            surfaceElement->QueryIntAttribute(ATTR_MAX.c_str(), &max);
            int32_t current = 0;
            surfaceElement->QueryIntAttribute(ATTR_CURRENT.c_str(), &current);
            uint32_t flag = 0;
            surfaceElement->QueryUnsignedAttribute(ATTR_FLAG.c_str(), &flag);

            Element ele = {min, max, current, current, flag};
            mDataStore->addStrIdPair(name, id);
            profile.elements.emplace(id, ele);
            surfaceElement = surfaceElement->NextSiblingElement();
        }
        mDataStore->addProfile(widgetId, profile);
        surfaceProfile = surfaceProfile->NextSiblingElement();
    }
}

static std::vector<Convert> parseCalculate(DataStore *dataStore, std::string name, std::string strs) {
    std::vector<Convert> ret;
    std::string splitExpression = ";";
    std::string splitKv = ":";
    std::string splitVariable = "@";

    strs += splitExpression;
    size_t pos = strs.find(splitExpression);
    while (pos != strs.npos) {
        std::string expressionSingle = strs.substr(0, pos);
        size_t posKv = expressionSingle.find(splitKv);
        std::string expression = expressionSingle.substr(0, posKv);
        std::string value = expressionSingle.substr(
                                    posKv+1, expressionSingle.size());
        if (expression.ends_with("const")) {
            ret.emplace_back(Convert(name, ExpressionPool::getEnum(expression),
                                  atof(value.c_str())));
        } else {
            size_t posVariable = value.find(splitVariable);
            ret.emplace_back(
                Convert(name, ExpressionPool::getEnum(expression),
                        dataStore->getIdWithStr(value.substr(posVariable+1, value.size())).value(),
                        dataStore->getIdWithStr(value.substr(0, posVariable)).value()));
        }
        strs = strs.substr(pos+1, strs.size());
        pos = strs.find(splitExpression);
    }
    return ret;
}

void LogicDataParser::loadDataConvert(tinyxml2::XMLElement *root) {
    LOGD("loadDataConvert begin");
    tinyxml2::XMLElement *surface = root->FirstChildElement(TAG_CONVERTS.c_str());
    tinyxml2::XMLElement *surfaceContext
        = surface->FirstChildElement(TAG_CONTEXT.c_str());

    while (surfaceContext) {
        tinyxml2::XMLElement *surfaceElement
            = surfaceContext->FirstChildElement(TAG_ELEMENT.c_str());
        const char* widgetName = surfaceContext->Attribute(TAG_WIDGET.c_str());
        const char* calculate = surfaceContext->Attribute(TAG_CALCULATE.c_str());
        if (0 != std::string("").compare(calculate)) {
            mDataStore->addDataConvert(
                widgetName, parseCalculate(mDataStore.get(), widgetName, calculate));
        }
        while (surfaceElement) {
            int32_t origin = 0;
            surfaceElement->QueryIntAttribute(ATTR_ORIGIN.c_str(), &origin);
            int32_t target = 0;
            surfaceElement->QueryIntAttribute(ATTR_TARGET.c_str(), &target);
            surfaceElement = surfaceElement->NextSiblingElement();
            mDataStore->addDataConvert(widgetName, origin, target);
        }
        surfaceContext = surfaceContext->NextSiblingElement();
    }
}

}  // namespace cpfw

