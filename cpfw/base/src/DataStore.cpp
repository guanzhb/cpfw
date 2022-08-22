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

#include "cpfw/base/include/DataStore.h"

#include <iostream>

namespace cpfw {

const TINVOKE_CHAIN DataStore::EMPTY_INVOKE_CHAIN = {""};
const TINVOKE_CONDITION DataStore::EMPTY_CONDITION
    = std::make_pair(ExpressionEnum::EMPTY, std::vector<Condition>());
Profile DataStore::EMPTY_PROFILE = Profile();

DataStore::DataStore() {
    std::cout << "ctor DataStore " << std::endl;
}

DataStore::~DataStore() {
}

std::optional<std::shared_ptr<Widget>>
        DataStore::getWidget(const std::string &name) {
    std::optional<std::shared_ptr<Widget>> ret = std::nullopt;
    if (mWidgetTable.find(name) != mWidgetTable.end()) {
        ret = mWidgetTable[name];
    }
    return ret;
}

const TINVOKE_CHAIN& DataStore::getChain(const std::string &parentName) {
    return  mInvokeChainTable.find(parentName) != mInvokeChainTable.end()
        ? mInvokeChainTable[parentName] : EMPTY_INVOKE_CHAIN;
    return EMPTY_INVOKE_CHAIN;
}

Profile& DataStore::getProfile(const std::string &name) {
    return mProfileTable.find(name) != mProfileTable.end()
        ? mProfileTable[name] : EMPTY_PROFILE;
}

void DataStore::setProfile(const std::string &profileName, int32_t value) {
    setProfile(profileName, "default", value);
}

void DataStore::setProfile(const std::string &profileName,
        const std::string &elementName, int32_t value) {
    std::cout << "DataStore[" << __func__ << "] " << profileName
        << " -> " << elementName << std::endl;
    Profile &profile = getProfile(profileName);
    if (&EMPTY_PROFILE == &profile) {
        return;
    }
    auto elementItor = profile.elements.find(elementName);
    if (elementItor == profile.elements.end()) {
        return;
    }
    auto &element = elementItor->second;
    element.flag = false;
    if (value != element.current) {
        element.flag = true;
        element.backup = element.current;
        element.current = std::clamp(value, element.min, element.max);
    }
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

const TINVOKE_CONDITION& DataStore::getCondition(const std::string widgetName) {
    return mConditionTable.find(widgetName) != mConditionTable.end()
        ? mConditionTable[widgetName] : EMPTY_CONDITION;
}

void DataStore::addWidget(std::shared_ptr<Widget> widget) {
    widget->linkDataStore(shared_from_this());
    mWidgetTable.emplace(widget->getName(), widget);
}

void DataStore::addInvokeChain(std::string parent, TINVOKE_CHAIN children) {
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

void DataStore::addCondition(std::string widgetName, TINVOKE_CONDITION condition) {
    mConditionTable.emplace(widgetName, condition);
}

}  // namespace cpfw

