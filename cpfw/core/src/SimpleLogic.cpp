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
#define TAG "SimpleLogic"

#include "SimpleLogic.h"

#include "Log.hpp"
#include "Utils.h"

namespace cpfw {

SimpleLogic::SimpleLogic()
    : SimpleLogic("", nullptr) {
}

SimpleLogic::SimpleLogic(const std::string &name,
        std::shared_ptr<SimpleLogicHelper> helper)
        : mName(name) {
    mHandler = std::make_unique<SimpleLogicHandler>(this, helper);
}

SimpleLogic::~SimpleLogic() {
}

const std::string& SimpleLogic::getName() const {
    return mName;
}

void SimpleLogic::sendMessage(Message message) {
    mHandler->post(message);
}

SimpleLogic::SimpleLogicHandler::SimpleLogicHandler(SimpleLogic *SimpleLogic,
        std::shared_ptr<SimpleLogicHelper> helper)
        : mSimpleLogic(SimpleLogic), mHelper(helper) {
    LOGI("SimpleLogicHandler ctor");
}

SimpleLogic::SimpleLogicHandler::~SimpleLogicHandler() {
}

int32_t SimpleLogic::SimpleLogicHandler::onInvoke(const Message &message) {
    LOGI("onInvoke enter state:%s", mHelper->getStateName(message.mWhat).c_str());
    const auto &widgetItor = mHelper->getWidgetPair(message.mWhat);
    for (auto itor=widgetItor.begin(); itor!=widgetItor.end(); ++itor) {
        LOGI("onInvoke widget:%s", itor->first->getName().c_str());
        std::invoke(itor->first->getCallback(), const_cast<WidgetValue&>(itor->second));
    }
    return 0;
}

void SimpleLogic::SimpleLogicHandler::onReply(
        const Message &message, const int32_t status) {
}

}  // namespace cpfw

