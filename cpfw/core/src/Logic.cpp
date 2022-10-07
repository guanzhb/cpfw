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
    uint32_t widgetId = mStore->getIdWithStr(widgetName).value();
    uint32_t elementId = mStore->getIdWithStr(elementName).value();
    msg.mWhat = (widgetId << 16) + elementId;
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

int32_t Logic::setProfileDelay(const std::string &widgetName, int32_t value,
        uint64_t delayTimeMs, const PostFlag flag) {
    return setProfileDelay(widgetName, "default", value, delayTimeMs, flag);
}

int32_t Logic::setProfileDelay(const std::string &widgetName,
        const std::string &elementName, int32_t value,
        uint64_t delayTimeMs, const PostFlag flag) {
    Message msg;
    uint32_t widgetId = mStore->getIdWithStr(widgetName).value();
    uint32_t elementId = mStore->getIdWithStr(elementName).value();
    msg.mWhat = (widgetId << 16) + elementId;
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
    if (nullptr != mCallbackWithName
            && message.mArg1 == static_cast<int32_t>(DataType::STRING)) {
        Bundle &bundle = const_cast<Message&>(message).mBundle;
        std::string widgetName;
        if (!bundle.get<std::string>(KEY_PROFILE, widgetName)) {
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
        mCallbackWithName(widgetName, elementName, value, status);
    } else if (nullptr != mCallbackWithId
           && message.mArg1 == static_cast<int32_t>(DataType::INT32)) {
        Bundle &bundle = const_cast<Message&>(message).mBundle;
        uint32_t widgetId = 0;
        if (!bundle.get(KEY_PROFILE, widgetId)) {
            return;
        }
        uint32_t elementId = 0;
        if (!bundle.get(KEY_ELEMENT, elementId)) {
            return;
        }
        int32_t value = 0;
        if (!bundle.get<int32_t>(KEY_VALUE, value)) {
            return;
        }
        mCallbackWithId(widgetId, elementId, value, status);
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
    uint32_t elementId = 0;

    if (message.mArg1 == static_cast<int32_t>(DataType::STRING)) {
        std::string widgetName;
        if (!bundle.get<std::string>(KEY_PROFILE, widgetName)) {
            return EINVAL;
        }
        std::string elementName;
        if (!bundle.get<std::string>(KEY_ELEMENT, elementName)) {
            return EINVAL;
        }
        widgetId = mLogic->mStore->getIdWithStr(widgetName).value();
        elementId = mLogic->mStore->getIdWithStr(elementName).value();
    } else {
        if (!bundle.get(KEY_PROFILE, widgetId)) {
            return EINVAL;
        }
        if (!bundle.get(KEY_ELEMENT, elementId)) {
            return EINVAL;
        }
    }
    int32_t value = 0;
    if (!bundle.get<int32_t>(KEY_VALUE, value)) {
        return EINVAL;
    }
    LOGI("onInvoke widgetId:" + std::to_string(widgetId)
         + " -> elemetId:" + std::to_string(elementId)
         + "with value:" + std::to_string(value));
    mLogic->mStore->setProfile(widgetId, elementId, value);
    return mLogic->mResponsibilityChain->invokeChain(widgetId);
}

void Logic::LogicHandler::onReply(const Message &message, const int32_t status) {
    mLogic->onReply(message, status);
}

}  // namespace cpfw

