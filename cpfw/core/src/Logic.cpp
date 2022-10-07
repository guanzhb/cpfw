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
#include <iostream>
#include <memory>
#include <string>

#include "LogicDataParser.h"
#include "Log.hpp"

namespace cpfw {

Logic::Logic() {
}

Logic::Logic(std::string configurationFile) {
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
    bundle.set(KEY_PROFILE, widgetId);
    bundle.set(KEY_ELEMENT, elementPairs);
    msg.mBundle = bundle;
    msg.mFlag = flag;
    msg.mArg1 = static_cast<int32_t>(DataType::INT32);

    mHandler->post(msg);

    return 0;
}

int32_t Logic::setProfile(const std::string &widgetName,
        const std::vector<TElementPairWithName> &elementPairs, const PostFlag flag) {
    Message msg;
    uint32_t widgetId = mStore->getIdWithStr(widgetName).value();
    msg.mWhat = widgetId;
    Bundle bundle;
    bundle.set(KEY_PROFILE, widgetName);
    bundle.set(KEY_ELEMENT, elementPairs);
    msg.mBundle = bundle;
    msg.mFlag = flag;
    msg.mArg1 = static_cast<int32_t>(DataType::STRING);

    mHandler->post(msg);

    return 0;
}

int32_t Logic::setProfileDelay(const uint32_t widgetId,
        const std::vector<TElementPairWithId> &elementPairs,
        uint64_t delayTimeMs, const PostFlag flag) {
    Message msg;
    msg.mWhat = widgetId;
    Bundle bundle;
    bundle.set(KEY_PROFILE, widgetId);
    bundle.set(KEY_ELEMENT, elementPairs);
    bundle.set(KEY_DELAY_TIME_MS, delayTimeMs);
    msg.mBundle = bundle;
    msg.mFlag = flag;
    msg.mArg1 = static_cast<int32_t>(DataType::INT32);

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
    bundle.set(KEY_PROFILE, widgetName);
    bundle.set(KEY_ELEMENT, elementPairs);
    bundle.set(KEY_DELAY_TIME_MS, delayTimeMs);
    msg.mBundle = bundle;
    msg.mFlag = flag;
    msg.mArg1 = static_cast<int32_t>(DataType::STRING);

    mHandler->postDelay(msg, delayTimeMs);

    return 0;
}

int32_t Logic::getProfile(const uint32_t widgetId) {
    return getProfile(widgetId, 0);
}

int32_t Logic::getProfile(const uint32_t widgetId,
            const uint32_t elementId) {
    return 0;
}

int32_t Logic::getProfile(const std::string &widgetName) {
    return getProfile(widgetName, "default");
}

int32_t Logic::getProfile(const std::string &widgetName,
            const std::string &elementName) {
    return 0;
}

void Logic::onReply(const Message &message, const int32_t status) {
    if (nullptr != mCallbackWithName
            && message.mArg1 == static_cast<int32_t>(DataType::STRING)) {
        Bundle &bundle = const_cast<Message&>(message).mBundle;
        std::string widgetName;
        if (!bundle.get(KEY_PROFILE, widgetName)) {
            return;
        }
        std::vector<TElementPairWithName> elementPairs;
        if (!bundle.get(KEY_ELEMENT, elementPairs)) {
            return;
        }
        mCallbackWithName(widgetName, elementPairs, status);
    } else if (nullptr != mCallbackWithId
           && message.mArg1 == static_cast<int32_t>(DataType::INT32)) {
        Bundle &bundle = const_cast<Message&>(message).mBundle;
        uint32_t widgetId = 0;
        if (!bundle.get(KEY_PROFILE, widgetId)) {
            return;
        }
        std::vector<TElementPairWithId> elementPairs;
        if (!bundle.get(KEY_ELEMENT, elementPairs)) {
            return;
        }
        mCallbackWithId(widgetId, elementPairs, status);
    }
}

Logic::LogicHandler::LogicHandler(Logic* logic) : mLogic(logic) {
    LOGI("LogicHandler ctor");
}

Logic::LogicHandler::~LogicHandler() {
}

int32_t Logic::LogicHandler::onInvoke(const Message &message) {
    Bundle &bundle = const_cast<Message&>(message).mBundle;
    uint32_t widgetId = 0;

    if (message.mArg1 == static_cast<int32_t>(DataType::STRING)) {
        std::string widgetName;
        if (!bundle.get(KEY_PROFILE, widgetName)) {
            return EINVAL;
        }
        if (auto widget = mLogic->mStore->getIdWithStr(widgetName); widget) {
            widgetId = widget.value();
        } else {
            return EINVAL;
        }
        std::vector<TElementPairWithName> elementPairs;
        if (!bundle.get(KEY_ELEMENT, elementPairs)) {
            return EINVAL;
        }
        mLogic->mStore->setProfile(widgetName, elementPairs);
    } else if (message.mArg1 == static_cast<int32_t>(DataType::INT32)) {
        if (!bundle.get(KEY_PROFILE, widgetId)) {
            return EINVAL;
        }
        std::vector<TElementPairWithId> elementPairs;
        if (!bundle.get(KEY_ELEMENT, elementPairs)) {
            return EINVAL;
        }
        mLogic->mStore->setProfile(widgetId, elementPairs);
    }
    return mLogic->mResponsibilityChain->invokeChain(widgetId);
}

void Logic::LogicHandler::onReply(const Message &message, const int32_t status) {
    mLogic->onReply(message, status);
}

}  // namespace cpfw

