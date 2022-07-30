/**
 * Copyright (C) 2020 The Cross Platform Framework Project
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

#include "cpfw/base/include/DataStore.h"

#include <iostream>

namespace cpfw {

DataStore::DataStore() {
}

DataStore::~DataStore() {
}

std::shared_ptr<Widget> DataStore::getWidget(const std::string &name) {
    std::shared_ptr<Widget> ret = std::make_shared<Widget>();
    if (mWidgetTable.find(name) != mWidgetTable.end()) {
        ret = mWidgetTable[name];
    }
    return ret;
}

std::vector<std::string> DataStore::getChain(const std::string &parentName) {
    std::vector<std::string> ret;
    if (mInvokeChainTable.find(parentName) != mInvokeChainTable.end()) {
        ret = mInvokeChainTable[parentName];
    }
    return ret;
}

Profile* DataStore::getProfile(const std::string &name) {
    Profile* ret = nullptr;
    if (mProfileTable.find(name) != mProfileTable.end()) {
        ret = &mProfileTable[name];
    }
    return ret;
}

void DataStore::setProfile(const std::string &profileName, int32_t value) {
    setProfile(profileName, "default", value);
}

void DataStore::setProfile(
        const std::string &profileName, const std::string &elementName, int32_t value) {
    Profile *profile = getProfile(profileName);
    if (nullptr == profile) {
        return;
    }
    auto elementItor = profile->elements.find(elementName);
    if (elementItor == profile->elements.end()) {
        return;
    }
    elementItor->second.current = value;
}

int32_t DataStore::getConvertedData(std::string context, int32_t origin) {
    auto convert = mDataConvertTable.find(context);
    if (convert == mDataConvertTable.end()) {
        return origin;  // TODO(guanzhb) LODI
    }
    auto data = convert->second.find(origin);
    if (data == convert->second.end()) {
        return origin;  // TODO(guanzhb) LODI
    }
    return data->second;
}

std::vector<std::pair<std::string, Condition>>
        DataStore::getCondition(const std::string context) {
    std::vector<std::pair<std::string, Condition>> ret;
    auto conditionItor = mConditionTable.find(context);
    if (conditionItor != mConditionTable.end()) {
        ret = mConditionTable[context];
    }
    return ret;
}

void DataStore::addWidget(
        const std::string widgetName, std::shared_ptr<Widget> widget) {
    mWidgetTable.emplace(widgetName, widget);
}

void DataStore::addInvokeChain(
        std::string parent, const std::vector<std::string> children) {
    mInvokeChainTable.emplace(parent, children);
}

void DataStore::addProfile(
        const std::string name, const Profile profile) {
    mProfileTable.emplace(name, profile);
}

void DataStore::addDataConvert(
        std::string context, int32_t origin, int32_t target) {
    std::map<int32_t, int32_t> convert;
    convert.emplace(origin, target);
    mDataConvertTable.emplace(context, convert);
}

void DataStore::addCondition(std::string context,
        const std::vector<std::pair<std::string, Condition>> condition) {
    mConditionTable.emplace(context, condition);
}

}  // namespace cpfw

