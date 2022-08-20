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

#include "cpfw/core/include/Logic.h"

#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "cpfw/core/include/DataParser.h"

namespace cpfw {

Logic::Logic() {
}

Logic::Logic(std::string configurationFile) {
    mStore = std::make_shared<DataStore>();
    DataParser parser = DataParser(configurationFile, mStore);
    mResponsibilityChain = std::make_unique<ResponsibilityChain>(mStore);
    mHandler = std::make_unique<LogicHandler>(this);
}

Logic::~Logic() {
}

void Logic::initialize() {
    mHandler->initialize();
}

void Logic::addWidget(std::shared_ptr<Widget> widget) {
    mStore->addWidget(widget);
}

int32_t Logic::setProfile(
        const std::string &widgetName, int32_t value, const PostFlag flag) {
    return setProfile(widgetName, "default", value, flag);
}

int32_t Logic::setProfile(const std::string &widgetName,
        const std::string &elementName, int32_t value, const PostFlag flag) {
    Message msg;
    msg.mWhat = std::hash<std::string>{}(widgetName + elementName);
    msg.mWidgetName = widgetName;
    msg.mElementName = elementName;
    msg.mValue = value;

    mHandler->post(msg, flag);

    return 0;
}

int32_t Logic::setProfileDelay(const std::string &widgetName,
            int32_t value, uint64_t delayTimeMs, const PostFlag flag) {
    return setProfileDelay(widgetName, "default", value, delayTimeMs, flag);
}

int32_t Logic::setProfileDelay(const std::string &widgetName,
        const std::string &elementName, int32_t value,
        uint64_t delayTimeMs, const PostFlag flag) {
    Message msg;
    msg.mWhat = std::hash<std::string>{}(widgetName + elementName);
    msg.mWidgetName = widgetName;
    msg.mElementName = elementName;
    msg.mValue = value;

    mHandler->postDelay(msg, delayTimeMs, flag);

    return 0;
}


int32_t Logic::getProfile(const std::string &widgetName) {
    return getProfile(widgetName, "default");
}

int32_t Logic::getProfile(const std::string &widgetName,
            const std::string &elementName) {
    return 0;
}

Logic::LogicHandler::LogicHandler(Logic* logic) : mLogic(logic) {
    std::cout << "LogicHandler ctor" << std::endl;
}

Logic::LogicHandler::~LogicHandler() {
}

int32_t Logic::LogicHandler::onInvoke(const Message &message) {
//    std::cout << "Logic[" << __func__ << "] " << message.mWidgetName
//        << " -> " << message.mElementName << std::endl;
    mLogic->mStore->setProfile(
        message.mWidgetName, message.mElementName, message.mValue);
    return mLogic->mResponsibilityChain->invokeChain(message.mWidgetName);
}

}  // namespace cpfw

