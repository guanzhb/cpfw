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

void Logic::registerCallback(TCallback callback) {
    mCallback = callback;
}

void Logic::unregisterCallback(TCallback callback) {
    mCallback = nullptr;
}

void Logic::addWidget(std::shared_ptr<Widget> widget) {
    mStore->addWidget(widget);
}

int32_t Logic::setProfile(
        const uint32_t widgetId, int32_t value, const PostFlag flag) {
    return setProfile(widgetId, 0, value, flag);
}

int32_t Logic::setProfile(const uint32_t widgetId,
        const uint32_t elementId, int32_t value, const PostFlag flag) {
    Message msg;
    msg.mWhat = (widgetId << 16) | elementId;
    Bundle bundle;
    bundle.set(KEY_PROFILE, widgetId);
    bundle.set(KEY_ELEMENT, elementId);
    bundle.set(KEY_VALUE, value);
    msg.mBundle = bundle;
    msg.mFlag = flag;
    msg.mArg1 = static_cast<int32_t>(DataType::UINT32);

    mHandler->post(msg);

    return 0;
}

int32_t Logic::setProfile(
        const std::string &widgetName, int32_t value, const PostFlag flag) {
    return setProfile(widgetName, "default", value, flag);
}

int32_t Logic::setProfile(const std::string &widgetName,
        const std::string &elementName, int32_t value, const PostFlag flag) {
    Message msg;
    msg.mWhat = std::hash<std::string>{}(widgetName + elementName);
    Bundle bundle;
    bundle.set(KEY_PROFILE, widgetName);
    bundle.set(KEY_ELEMENT, elementName);
    bundle.set(KEY_VALUE, value);
    msg.mBundle = bundle;
    msg.mFlag = flag;
    msg.mArg1 = static_cast<int32_t>(DataType::STRING);

    mHandler->post(msg);

    return 0;
}

int32_t Logic::setProfileDelay(const uint32_t widgetId,
            int32_t value, uint64_t delayTimeMs, const PostFlag flag) {
    return setProfileDelay(widgetId, 0, value, delayTimeMs, flag);
}

int32_t Logic::setProfileDelay(const uint32_t widgetId,
        const uint32_t elementId, int32_t value,
        uint64_t delayTimeMs, const PostFlag flag) {
    Message msg;
    msg.mWhat = (widgetId << 16) + elementId;
    Bundle bundle;
    bundle.set(KEY_PROFILE, widgetId);
    bundle.set(KEY_ELEMENT, elementId);
    bundle.set(KEY_VALUE, value);
    bundle.set(KEY_DELAY_TIME_MS, delayTimeMs);
    msg.mBundle = bundle;
    msg.mFlag = flag;
    msg.mArg1 = static_cast<int32_t>(DataType::UINT32);

    mHandler->postDelay(msg, delayTimeMs);

    return 0;
}

int32_t Logic::setProfileDelay(const std::string &widgetName,
        const std::string &elementName, int32_t value,
        uint64_t delayTimeMs, const PostFlag flag) {
    Message msg;
    msg.mWhat = std::hash<std::string>{}(widgetName + elementName);
    Bundle bundle;
    bundle.set(KEY_PROFILE, widgetName);
    bundle.set(KEY_ELEMENT, elementName);
    bundle.set(KEY_VALUE, value);
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
#if 0  // FIXME(guanzhb) callback value can be int or str
    if (nullptr != mCallback) {
        Bundle &bundle = const_cast<Message&>(message).mBundle;
        std::string profileName;
        if (!bundle.get<std::string>(KEY_PROFILE, profileName)) {
            return;
        }
        std::string elementName;
        if (!bundle.get<std::string>(KEY_ELEMENT, elementName)) {
            return;
        }
        int32_t value = 0;
        if (!bundle.get<int32_t>(KEY_VALUE, value)) {
            return;
        }
        mCallback(profileName, elementName, value, status);
    }
#endif
}

Logic::LogicHandler::LogicHandler(Logic* logic) : mLogic(logic) {
    LOGI("LogicHandler ctor");
}

Logic::LogicHandler::~LogicHandler() {
}

int32_t Logic::LogicHandler::onInvoke(const Message &message) {
    Bundle &bundle = const_cast<Message&>(message).mBundle;
    if (message.mArg1 == static_cast<int32_t>(DataType::STRING)) {
#if 0  // FIXME(guanzhb) whether export str interface to user?
        std::string profileName;
        if (!bundle.get<std::string>(KEY_PROFILE, profileName)) {
            return EINVAL;
        }
        std::string elementName;
        if (!bundle.get<std::string>(KEY_ELEMENT, elementName)) {
            return EINVAL;
        }
        int32_t value = 0;
        if (!bundle.get<int32_t>(KEY_VALUE, value)) {
            return EINVAL;
        }
        LOGI("onInvoke " + profileName + " -> " + elementName);
        mLogic->mStore->setProfile(profileName, elementName, value);
        return mLogic->mResponsibilityChain->invokeChain(profileName);
#endif
    } else {
        uint32_t widgetId;
        if (!bundle.get(KEY_PROFILE, widgetId)) {
            return EINVAL;
        }
        uint32_t elementId;
        if (!bundle.get(KEY_ELEMENT, elementId)) {
            return EINVAL;
        }
        int32_t value = 0;
        if (!bundle.get<int32_t>(KEY_VALUE, value)) {
            return EINVAL;
        }
        mLogic->mStore->setProfile(widgetId, elementId, value);
        return mLogic->mResponsibilityChain->invokeChain(widgetId);
    }
    return 0;
}

void Logic::LogicHandler::onReply(const Message &message, const int32_t status) {
    mLogic->onReply(message, status);
}

}  // namespace cpfw

