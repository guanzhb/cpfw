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

#define TAG "Logic"

#include "Logic.h"

#include <functional>
#include <memory>
#include <string>

#include "Log.hpp"
#include "LogicDataParser.h"

namespace cpfw {

Logic::Logic() {
}

Logic::Logic(const std::string &configurationFile) {
    mStore = std::make_shared<DataStore>();
    LogicDataParser parser = LogicDataParser(configurationFile, mStore);
    mResponsibilityChain = std::make_unique<ResponsibilityChain>(mStore);
    mHandler = std::make_unique<LogicHandler>(this);
}

Logic::~Logic() {
}

void Logic::registerCallback(TCallbackWithName callbackWithName) {
    mCallbackWithName = callbackWithName;
}

void Logic::unregisterCallback(TCallbackWithName callbackWithName) {
    mCallbackWithName = nullptr;
}

void Logic::registerCallback(TCallbackWithId callbackWithId) {
    mCallbackWithId = callbackWithId;
}

void Logic::unregisterCallback(TCallbackWithId callbackWithId) {
    mCallbackWithId = nullptr;
}

void Logic::addWidget(std::shared_ptr<Widget> widget) {
    mStore->addWidget(widget);
}

int32_t Logic::setProfile(const uint32_t widgetId,
        const std::vector<TElementPairWithId> &elementPairs, const PostFlag flag) {
    Message msg;
    msg.mWhat = widgetId;
    Bundle bundle;
    bundle.set(KEY_WIDGET, widgetId);
    bundle.set(KEY_ELEMENT, elementPairs);
    msg.mBundle = bundle;
    msg.mFlag = flag;
    msg.mArg1 = DataType::INT32;

    mHandler->post(msg);

    return 0;
}

int32_t Logic::setProfile(const std::string &widgetName,
        const std::vector<TElementPairWithName> &elementPairs, const PostFlag flag) {
    Message msg;
    uint32_t widgetId = mStore->getIdWithStr(widgetName).value();
    msg.mWhat = widgetId;
    Bundle bundle;
    bundle.set(KEY_WIDGET, widgetName);
    bundle.set(KEY_ELEMENT, elementPairs);
    msg.mBundle = bundle;
    msg.mFlag = flag;
    msg.mArg1 = DataType::STRING;

    mHandler->post(msg);

    return 0;
}

int32_t Logic::setProfileDelay(const uint32_t widgetId,
        const std::vector<TElementPairWithId> &elementPairs,
        uint64_t delayTimeMs, const PostFlag flag) {
    Message msg;
    msg.mWhat = widgetId;
    Bundle bundle;
    bundle.set(KEY_WIDGET, widgetId);
    bundle.set(KEY_ELEMENT, elementPairs);
    bundle.set(KEY_DELAY_TIME_MS, delayTimeMs);
    msg.mBundle = bundle;
    msg.mFlag = flag;
    msg.mArg1 = DataType::INT32;

    mHandler->postDelay(msg, delayTimeMs);

    return 0;
}

int32_t Logic::setProfileDelay(const std::string &widgetName,
        const std::vector<TElementPairWithName> &elementPairs,
        uint64_t delayTimeMs, const PostFlag flag) {
    Message msg;
    uint32_t widgetId = mStore->getIdWithStr(widgetName).value();
    msg.mWhat = widgetId;
    Bundle bundle;
    bundle.set(KEY_WIDGET, widgetName);
    bundle.set(KEY_ELEMENT, elementPairs);
    bundle.set(KEY_DELAY_TIME_MS, delayTimeMs);
    msg.mBundle = bundle;
    msg.mFlag = flag;
    msg.mArg1 = DataType::STRING;

    mHandler->postDelay(msg, delayTimeMs);

    return 0;
}

std::map<uint32_t, int32_t> Logic::getProfile(
        const uint32_t widgetId, const std::vector<uint32_t> elementId) {
    std::map<uint32_t, int32_t> ret;
    Profile profile = mStore->getProfile(widgetId);
    auto &elements = profile.elements;
    if (!elementId.empty()) {
        for (auto elementIdItor : elementId) {
            if (auto elementItor = elements.find(elementIdItor);
                    elementItor != elements.end()) {
                ret.emplace(elementIdItor, elementItor->second.current);
            }
        }
    } else {
        for (auto &elementItor : elements) {
            ret.emplace(elementItor.first, elementItor.second.current);
        }
    }
    return ret;
}

std::map<uint32_t, int32_t> Logic::getProfile(
        const std::string &widgetName, const std::vector<std::string> &elementName) {
    std::map<uint32_t, int32_t> ret;
    Profile profile = mStore->getProfile(widgetName);
    auto &elements = profile.elements;
    if (elements.empty()) {
        return ret;
    }
    if (!elementName.empty()) {
        for (auto elementNameItor : elementName) {
            if (auto id = mStore->getIdWithStr(elementNameItor); id) {
                if (auto elementItor = elements.find(id.value());
                        elementItor != elements.end()) {
                    ret.emplace(id.value(), elementItor->second.current);
                }
            }
        }
    } else {
        for (auto &elementItor : elements) {
            ret.emplace(elementItor.first, elementItor.second.current);
        }
    }
    return ret;
}

// get bundle with no safe version for perf, ensure it's safe inside the Logic
void Logic::onReply(const Message &message, const int32_t status) {
    if (nullptr != mCallbackWithName
            && message.mArg1 == DataType::STRING) {
        Bundle &bundle = const_cast<Message&>(message).mBundle;
        std::string widgetName;
        bundle.get(KEY_WIDGET, widgetName);
        std::vector<TElementPairWithName> elementPairs;
        bundle.get(KEY_ELEMENT, elementPairs);
        mCallbackWithName(widgetName, elementPairs, status);
    } else if (nullptr != mCallbackWithId && message.mArg1 == DataType::INT32) {
        Bundle &bundle = const_cast<Message&>(message).mBundle;
        uint32_t widgetId = 0;
        bundle.get(KEY_WIDGET, widgetId);
        std::vector<TElementPairWithId> elementPairs;
        bundle.get(KEY_ELEMENT, elementPairs);
        mCallbackWithId(widgetId, elementPairs, status);
    }
}

Logic::LogicHandler::LogicHandler(Logic* logic) : mLogic(logic) {
    LOGI("LogicHandler ctor");
}

Logic::LogicHandler::~LogicHandler() {
}

// get bundle with no safe version for perf, ensure it's safe inside the Logic
int32_t Logic::LogicHandler::onInvoke(const Message &message) {
    Bundle &bundle = const_cast<Message&>(message).mBundle;
    uint32_t widgetId = 0;

    if (message.mArg1 == DataType::STRING) {
        std::string widgetName;
        bundle.get(KEY_WIDGET, widgetName);
        if (auto widget = mLogic->mStore->getIdWithStr(widgetName); widget) {
            widgetId = widget.value();
        } else {
            return EINVAL;
        }
        std::vector<TElementPairWithName> elementPairs;
        bundle.get(KEY_ELEMENT, elementPairs);
        mLogic->mStore->setProfile(widgetName, elementPairs);
    } else if (message.mArg1 == DataType::INT32) {
        bundle.get(KEY_WIDGET, widgetId);
        std::vector<TElementPairWithId> elementPairs;
        bundle.get(KEY_ELEMENT, elementPairs);
        mLogic->mStore->setProfile(widgetId, elementPairs);
    }
    return mLogic->mResponsibilityChain->invokeChain(widgetId);
}

void Logic::LogicHandler::onReply(const Message &message, const int32_t status) {
    mLogic->onReply(message, status);
}

}  // namespace cpfw

