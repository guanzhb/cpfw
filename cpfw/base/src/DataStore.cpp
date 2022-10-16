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

#define TAG "DataStore"

#include "DataStore.h"
#include "Log.hpp"

#include <iostream>

namespace cpfw {

const TINVOKE_CHAIN DataStore::EMPTY_INVOKE_CHAIN = {};
const TINVOKE_CONDITION DataStore::EMPTY_CONDITION
    = std::make_pair(ExpressionEnum::EMPTY, std::vector<Condition>());
Profile DataStore::EMPTY_PROFILE = Profile();
const std::vector<Convert> DataStore::EMPTY_CONVERT = {};
const uint32_t DataStore::EMPTY_BIND = UINT32_MAX;

DataStore::DataStore() {
    LOGD("ctor DataStore");
}

DataStore::~DataStore() {
}

std::optional<std::shared_ptr<Widget>>
        DataStore::getWidget(const uint32_t widgetId) {
    return getOptionalFromMap(mWidgetTable, widgetId);
}

std::optional<std::shared_ptr<Widget>>
        DataStore::getWidget(const std::string &widgetName) {
    if (auto id = getIdWithStr(widgetName); id) {
        return getWidget(id.value());
    }
    return  std::nullopt;
}

const TINVOKE_CHAIN& DataStore::getChain(uint32_t parentId) {
    return getOrDefaultFromMap(mInvokeChainTable, parentId, EMPTY_INVOKE_CHAIN);
}

const TINVOKE_CHAIN& DataStore::getChain(const std::string &parentName) {
    if (auto id = getIdWithStr(parentName); id) {
        return getChain(id.value());
    }
    return EMPTY_INVOKE_CHAIN;
}

Profile& DataStore::getProfile(const uint32_t widgetId) {
    return getOrDefaultFromMap(mProfileTable, widgetId, EMPTY_PROFILE);
}

Profile& DataStore::getProfile(const std::string &widgetName) {
    if (auto id = getIdWithStr(widgetName); id) {
        return getProfile(id.value());
    }
    return EMPTY_PROFILE;
}

void DataStore::setProfile(
        const uint32_t widgetId, std::vector<TElementPairWithId> elementPairs) {
    Profile &profile = getProfile(widgetId);
    if (&EMPTY_PROFILE == &profile) {
        return;
    }
    std::for_each(elementPairs.begin(), elementPairs.end(),
        [&](auto &elementPair) -> void {
            auto elementItor = profile.elements.find(elementPair.first);
            if (elementItor == profile.elements.end()) {
                return;
            }
            auto &element = elementItor->second;
            element.flag = false;
            if (elementPair.second != element.current) {
                element.flag = true;
                element.backup = element.current;
                element.current = std::clamp(elementPair.second, element.min, element.max);
            }
        });
}

void DataStore::setProfile(
        const std::string &widgetName, std::vector<TElementPairWithName> elementPairs) {
    uint32_t widgetId = 0;
    if (auto widgetIdOption = getIdWithStr(widgetName); widgetIdOption) {
        widgetId = widgetIdOption.value();
    } else {
        return;
    }
    Profile &profile = getProfile(widgetId);
    if (&EMPTY_PROFILE == &profile) {
        return;
    }
    std::for_each(elementPairs.begin(), elementPairs.end(),
        [&](auto &elementPair) -> void {
            uint32_t elementId = 0;
            if (auto elementIdOption = getIdWithStr(elementPair.first); elementIdOption) {
                elementId = elementIdOption.value();
            } else {
                return;
            }
            auto elementItor = profile.elements.find(elementId);
            if (elementItor == profile.elements.end()) {
                return;
            }
            auto &element = elementItor->second;
            element.flag = false;
            if (elementPair.second != element.current) {
                element.flag = true;
                element.backup = element.current;
                element.current = std::clamp(elementPair.second, element.min, element.max);
            }
        });
}

int32_t DataStore::getConvertedData(const uint32_t contextId, int32_t origin) {
    auto convert = mDataMapTable.find(contextId);
    if (convert == mDataMapTable.end()) {
        return origin;
    }
    auto data = convert->second.find(origin);
    if (data == convert->second.end()) {
        return origin;
    }
    return data->second;
}


int32_t DataStore::getConvertedData(const std::string &context, int32_t origin) {
    if (auto id = getIdWithStr(context); id) {
        return getConvertedData(id.value(), origin);
    }
    return origin;
}

const std::vector<Convert>& DataStore::getConvertTable(const uint32_t widgetId) {
    return getOrDefaultFromMap(mConvertTable, widgetId, EMPTY_CONVERT);
}

const std::vector<Convert>& DataStore::getConvertTable(const std::string &context) {
    if (auto id = getIdWithStr(context); id) {
        return getConvertTable(id.value());
    }
    return EMPTY_CONVERT;
}

const TINVOKE_CONDITION& DataStore::getCondition(const uint32_t widgetId) {
    return getOrDefaultFromMap(mConditionTable, widgetId, EMPTY_CONDITION);
}

const TINVOKE_CONDITION& DataStore::getCondition(const std::string &widgetName) {
    if (auto id = getIdWithStr(widgetName); id) {
        return getCondition(id.value());
    }
    return EMPTY_CONDITION;
}

const uint32_t DataStore::getBind(const uint32_t widgetId) {
    return getOrDefaultFromMap(mBindTable, widgetId, EMPTY_BIND);
}

const uint32_t DataStore::getBind(const std::string &widgetName) {
    if (auto id = getIdWithStr(widgetName); id) {
        return getBind(id.value());
    }
    return UINT32_MAX;
}

void DataStore::addWidget(std::shared_ptr<Widget> widget) {
    widget->linkDataStore(shared_from_this());
    mWidgetTable.emplace(widget->getId(), widget);
}

void DataStore::addInvokeChain(const uint32_t parent, TINVOKE_CHAIN children) {
    mInvokeChainTable.emplace(parent, children);
}

void DataStore::addProfile(const uint32_t widgetId, const Profile profile) {
    mProfileTable.emplace(widgetId, profile);
}

void DataStore::addDataConvert(
        const std::string &context, int32_t origin, int32_t target) {
    std::map<int32_t, int32_t> convert;
    convert.emplace(origin, target);
    mDataMapTable.emplace(getIdWithStr(context).value(), convert);
}

void DataStore::addDataConvert(
        const std::string &context, std::vector<Convert> convert) {
    mConvertTable.emplace(getIdWithStr(context).value(), convert);
}

void DataStore::addCondition(
        const std::string &widgetName, TINVOKE_CONDITION condition) {
    mConditionTable.emplace(getIdWithStr(widgetName).value(), condition);
}

void DataStore::addBind(const uint32_t widgetId, const uint32_t bindId) {
    mBindTable.emplace(widgetId, bindId);
}

void DataStore::addStrIdPair(const std::string &name, uint32_t id) {
    mStrToIdTable.emplace(name, id);
}

std::optional<uint32_t> DataStore::getIdWithStr(const std::string &name) {
    return getOptionalFromMap(mStrToIdTable, name);
}

}  // namespace cpfw

