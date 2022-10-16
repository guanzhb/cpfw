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
 * WITHOUT WARRANTIES OR SimpleLogicHelperS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define TAG "SimpleLogicHelper"

//#define LOG_VERBLE

#ifdef LOG_VERBLE
#define LOGV LOGD
#else
#define LOGV(...) while (0);
#endif

#include "SimpleLogicHelper.h"

#include <string>
#include <memory>

#include "tinyxml2.h"
#include "Widget.h"
#include "Log.hpp"

namespace cpfw {

const std::string SimpleLogicHelper::ROOT = "simpleLogic";
const std::string SimpleLogicHelper::TAG_STATE = "state";
const std::string SimpleLogicHelper::ATTR_NAME = "name";
const std::string SimpleLogicHelper::ATTR_VALUE = "value";
const std::string SimpleLogicHelper::TAG_CHILD = "child";
const std::string SimpleLogicHelper::ATTR_WIDGET = "widget";
const std::string SimpleLogicHelper::ATTR_ID = "id";
const std::string SimpleLogicHelper::VALUE_DELIM = ",";
const std::vector<WidgetPair> SimpleLogicHelper::EMPTY_WIDGET_PAIR = {};
const std::string SimpleLogicHelper::EMPTY_STATE = "empty";

SimpleLogicHelper::SimpleLogicHelper() {
}

SimpleLogicHelper::~SimpleLogicHelper() {
}

void SimpleLogicHelper::addWidget(std::shared_ptr<Widget> widget) {
    mWidgetTable.emplace(widget->getId(), widget);
}

void SimpleLogicHelper::loadConfiguration(const std::string &configurationFile) {
    LOGV("loadConfiguration begin file:" + configurationFile);
    std::shared_ptr<tinyxml2::XMLDocument> doc = std::make_shared<tinyxml2::XMLDocument>();
    tinyxml2::XMLError err = doc->LoadFile(configurationFile.c_str());
    if (err != tinyxml2::XML_SUCCESS) {
        LOGE("read file error!");
        return;
    }

    tinyxml2::XMLElement *root = doc->RootElement();
    loadState(root);
}

static std::vector<int32_t> parseValue(std::string strs) {
    std::string delim = ",";
    std::vector<int32_t> ret;

    size_t pos = strs.find(delim);
    while (pos != strs.npos) {
        ret.push_back(std::atoi(strs.substr(0, pos).c_str()));
        strs = strs.substr(pos+1, strs.size());
        pos = strs.find(delim);
    }
    ret.push_back(std::atoi(strs.substr(0, pos).c_str()));
    return ret;
}

void SimpleLogicHelper::loadState(tinyxml2::XMLElement *root) {
    LOGV("loadState begin");
    tinyxml2::XMLElement *surfaceState
        = root->FirstChildElement(TAG_STATE.c_str());
    
    while (surfaceState) {
        int32_t stateValue = 0;
        surfaceState->QueryIntAttribute(ATTR_VALUE.c_str(), &stateValue);
        const char *stateName = surfaceState->Attribute(ATTR_NAME.c_str());
        mStateNameTable.emplace(stateValue, stateName);
        tinyxml2::XMLElement *surfaceChild
            = surfaceState->FirstChildElement(TAG_CHILD.c_str());
        std::vector<std::pair<std::shared_ptr<Widget>, WidgetValue>> stateAction;
        while (surfaceChild) {
            const char *widgetName = surfaceChild->Attribute(ATTR_WIDGET.c_str());
            LOGV("loadState widget:" + std::string(widgetName));
            uint32_t widgetId = 0;
            surfaceChild->QueryUnsignedAttribute(ATTR_ID.c_str(), &widgetId);
            const char *widgetValue = surfaceChild->Attribute(ATTR_VALUE.c_str());
            LOGV("loadState widget value:" + std::string(widgetValue));
            stateAction.push_back(std::make_pair(
                    mWidgetTable[widgetId], parseValue(widgetValue)));
            surfaceChild = surfaceChild->NextSiblingElement();
        }
        mStateTable.emplace(stateValue, stateAction);
        surfaceState = surfaceState->NextSiblingElement();
    }
    mWidgetTable.clear();
}

const std::vector<WidgetPair>& SimpleLogicHelper::getWidgetPair(uint32_t stateId) {
    const auto &itor = mStateTable.find(stateId);
    return itor == mStateTable.end() ? EMPTY_WIDGET_PAIR : itor->second;
}

const std::string& SimpleLogicHelper::getStateName(uint32_t stateId) {
    const auto &itor = mStateNameTable.find(stateId);
    return itor == mStateNameTable.end() ? EMPTY_STATE : itor->second;
}

}  // namespace cpfw

