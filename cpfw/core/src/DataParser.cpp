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
 * WITHOUT WARRANTIES OR DataParserS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define TAG "DataParser"

#include "DataParser.h"

#include <iostream>

#include "ExpressionPool.h"
#include "Log.hpp"

namespace cpfw {

const std::string DataParser::ROOT = "cpfw";
const std::string DataParser::TAG_WIDGETS = "widgets";
const std::string DataParser::TAG_WIDGET ="widget";
const std::string DataParser::ATTR_WIDGET ="widget";
const std::string DataParser::ATTR_NAME = "name";
const std::string DataParser::TAG_INVOKE_CHAIN = "invokeChains";
const std::string DataParser::TAG_INVOKE_CHAIN_PARENT = "parent";
const std::string DataParser::TAG_INVOKE_CHAIN_CHILD = "child";
const std::string DataParser::ATTR_CONDITION = "condition";
const std::string DataParser::TAG_CONDITIONS = "conditions";
const std::string DataParser::TAG_CONDITION = "condition";
const std::string DataParser::ATTR_LOGIC = "logic";
const std::string DataParser::ATTR_CONDITION_WHEN = "when";
const std::string DataParser::DEFAULT_NAME = "default";
const std::string DataParser::PROFILE_DELIM = ";";
const std::string DataParser::ATTR_CONDITION_WHEN_DELIM = "@";
const std::string DataParser::ATTR_CONDITION_VALUE_DELIM = ",";
const std::string DataParser::TAG_PROFILES = "profiles";
const std::string DataParser::TAG_PROFILE = "profile";
const std::string DataParser::ATTR_BIND_TO = "bindTo";
const std::string DataParser::TAG_ELEMENT = "element";
const std::string DataParser::ATTR_MIN = "min";
const std::string DataParser::ATTR_MAX = "max";
const std::string DataParser::ATTR_CURRENT = "current";
const std::string DataParser::ATTR_TYPE = "type";
const std::string DataParser::TAG_CONTEXT = "context";
const std::string DataParser::TAG_CONVERTS = "converts";
const std::string DataParser::TAG_CONVERT = "convert";
const std::string DataParser::TAG_CALCULATE = "calculate";
const std::string DataParser::ATTR_ORIGIN = "origin";
const std::string DataParser::ATTR_TARGET = "target";

DataParser::DataParser() {
}

DataParser::DataParser(std::string configurationFile,
        std::shared_ptr<DataStore> dataStore)
        : mDataStore(dataStore) {
    LOGD(TAG, "DataParser begin");
    LOGD(TAG, "DataParser load " + configurationFile + " begin");
    std::shared_ptr<tinyxml2::XMLDocument> doc = std::make_shared<tinyxml2::XMLDocument>();
    tinyxml2::XMLError err = doc->LoadFile(configurationFile.c_str());
    if (err != tinyxml2::XML_SUCCESS) {
        LOGE(TAG, "read file error!");
        return;
    }

    tinyxml2::XMLElement *root = doc->RootElement();
    loadConfiguration(root);

    LOGD(TAG, "DataParser load " + configurationFile + " end");
}

DataParser::~DataParser() {
}

void DataParser::loadConfiguration(tinyxml2::XMLElement *root) {
    loadInvokeChain(root);
    loadConditions(root);
    loadProfile(root);
    loadDataConvert(root);
}

void DataParser::loadInvokeChain(tinyxml2::XMLElement *root) {
    LOGD(TAG, "loadInvokeChain begin");
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
            chains.push_back(childWidgetName);
            surfaceChild = surfaceChild->NextSiblingElement();
        }
        mDataStore->addInvokeChain(parentWidgetName, chains);
        surfaceParent = surfaceParent->NextSiblingElement();
    }
}

static Condition parseCondition(std::string name, std::string strs) {
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
            return Condition(name, ss[0], ss[1], ExpressionPool::getEnum(ss[2]),
                         std::atoi(ss[3].c_str()), std::atoi(ss[3].c_str()));
        case 3:
            return Condition(name, ss[0], ss[1], ExpressionPool::getEnum(ss[2]),
                             0, 0);
    }
    return Condition(name, ss[0], ss[1], ExpressionPool::getEnum(ss[2]),
                     std::atoi(ss[3].c_str()), std::atoi(ss[4].c_str()));
}

void DataParser::loadConditions(tinyxml2::XMLElement *root) {
    LOGD(TAG, "loadCondition begin");
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
            Condition c = parseCondition(widgetName, when);
            condition.push_back(c);
            surfaceElement = surfaceElement->NextSiblingElement();
        }
        mDataStore->addCondition(widgetName,
                        std::make_pair(ExpressionPool::getEnum(logic), condition));
        surfaceCondition = surfaceCondition->NextSiblingElement();
    }
}

void DataParser::loadProfile(tinyxml2::XMLElement *root) {
    LOGD(TAG, "loadProfile begin");
    tinyxml2::XMLElement *surface = root->FirstChildElement(TAG_PROFILES.c_str());
    tinyxml2::XMLElement *surfaceProfile = surface->FirstChildElement(TAG_PROFILE.c_str());
    
    while (surfaceProfile) {
        const char* widgetName = surfaceProfile->Attribute(ATTR_WIDGET.c_str());
        const char* bindTo = surfaceProfile->Attribute(ATTR_BIND_TO.c_str());
        if (0 != DataStore::EMPTY_BIND.compare(bindTo)) {
             mDataStore->addBind(widgetName, bindTo);
        }
        Profile profile;
        tinyxml2::XMLElement *surfaceElement
            = surfaceProfile->FirstChildElement(TAG_ELEMENT.c_str());
        while (surfaceElement) {
            const char* name = surfaceElement->Attribute(ATTR_NAME.c_str());
            int min = 0;
            surfaceElement->QueryIntAttribute(ATTR_MIN.c_str(), &min);
            int max = 0;
            surfaceElement->QueryIntAttribute(ATTR_MAX.c_str(), &max);
            int current = 0;
            surfaceElement->QueryIntAttribute(ATTR_CURRENT.c_str(), &current);
            int type = 0;
            surfaceElement->QueryIntAttribute(ATTR_TYPE.c_str(), &type);

            Element ele = {min, max, current, current, static_cast<uint32_t>(type)};
            profile.elements.emplace(name, ele);
            surfaceElement = surfaceElement->NextSiblingElement();
        }
        mDataStore->addProfile(widgetName, profile);
        surfaceProfile = surfaceProfile->NextSiblingElement();
    }
}

static std::vector<Convert> parseCalculate(std::string name, std::string strs) {
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
            ret.emplace_back(Convert(
                name, ExpressionPool::getEnum(expression),
                value.substr(posVariable+1, value.size()),
                value.substr(0, posVariable)));
        }
        strs = strs.substr(pos+1, strs.size());
        pos = strs.find(splitExpression);
    }
    return ret;
}

void DataParser::loadDataConvert(tinyxml2::XMLElement *root) {
    LOGD(TAG, "loadDataConvert begin");
    tinyxml2::XMLElement *surface = root->FirstChildElement(TAG_CONVERTS.c_str());
    tinyxml2::XMLElement *surfaceContext
        = surface->FirstChildElement(TAG_CONTEXT.c_str());
    
    while (surfaceContext) {
        tinyxml2::XMLElement *surfaceElement
            = surfaceContext->FirstChildElement(TAG_ELEMENT.c_str());
        const char* widgetName = surfaceContext->Attribute(TAG_WIDGET.c_str());
        const char* calculate = surfaceContext->Attribute(TAG_CALCULATE.c_str());
        if (0 != std::string("").compare(calculate)) {
            mDataStore->addDataConvert(widgetName, parseCalculate(widgetName, calculate));
        }
        while (surfaceElement) {
            int origin = 0;
            surfaceElement->QueryIntAttribute(ATTR_ORIGIN.c_str(), &origin);
            int target = 0;
            surfaceElement->QueryIntAttribute(ATTR_TARGET.c_str(), &target);
            surfaceElement = surfaceElement->NextSiblingElement();
            mDataStore->addDataConvert(widgetName, origin, target);
        }
        surfaceContext = surfaceContext->NextSiblingElement();
    }
}

}  // namespace cpfw

