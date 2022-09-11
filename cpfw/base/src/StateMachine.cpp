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
#define TAG "StateMachine"

#include "StateMachine.h"

#include "Log.hpp"

namespace cpfw {

StateMachine::StateMachine() : StateMachine("") {
}

StateMachine::StateMachine(const std::string &name) : mName(name) {
    mHandler = std::make_unique<StateMachineHandler>(this);
}

StateMachine::~StateMachine() {
}

const std::string& StateMachine::getName() const {
    return mName;
}

void StateMachine::addState(uint32_t stateId, std::shared_ptr<State> state) {
    mStateTable.emplace(stateId, state);
}

void StateMachine::removeState(uint32_t stateId) {
    mStateTable.erase(stateId);
}

void StateMachine::setInitialState(uint32_t stateId) {
    const auto &initialStateItor = mStateTable.find(stateId);
    if (initialStateItor == mStateTable.end()) {
         LOGE("[transitState] stateId:" + std::to_string(stateId) + " mismatch");
         return;
    }
    mCurrentState = initialStateItor->second;
    mCurrentState->enter();
}

void StateMachine::transitStateTo(uint32_t stateId) {
    const auto &nextStateItor = mStateTable.find(stateId);
    if (nextStateItor == mStateTable.end()) {
         LOGE("[transitState] stateId:" + std::to_string(stateId) + " mismatch");
         return;
    }
    mCurrentState->exit();
    mCurrentState = nextStateItor->second;
    mCurrentState->enter();
}

void StateMachine::sendMessage(Message message) {
    mHandler->post(message);
}

std::shared_ptr<State> StateMachine::getCurrentState() {
    return mCurrentState;
}

StateMachine::StateMachineHandler::StateMachineHandler(StateMachine *stateMachine)
    : mStateMachine(stateMachine) {
    LOGI("StateMachineHandler ctor");
}

StateMachine::StateMachineHandler::~StateMachineHandler() {
}

int32_t StateMachine::StateMachineHandler::onInvoke(const Message &message) {
    mStateMachine->getCurrentState()->handleMessage(message);
    return 0;
}

void StateMachine::StateMachineHandler::onReply(
        const Message &message, const int32_t status) {
}

}  // namespace cpfw

