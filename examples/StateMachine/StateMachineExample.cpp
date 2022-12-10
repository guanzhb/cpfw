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

#define TAG "StateMachineExample"

#include "StateMachineExample.h"

#include <memory>

#include "Log.hpp"

using namespace cpfw;

namespace cpfw {

const uint32_t StateMachineExample::STATE_INIT = 0;
const uint32_t StateMachineExample::STATE_1 = 1;
const uint32_t StateMachineExample::STATE_2 = 2;
const uint32_t StateMachineExample::STATE_3 = 3;

const uint32_t StateMachineExample::MSG_1 = 1;
const uint32_t StateMachineExample::MSG_2 = 2;
const uint32_t StateMachineExample::MSG_3 = 3;
const uint32_t StateMachineExample::MSG_4 = 4;
const uint32_t StateMachineExample::MSG_5 = 5;
const uint32_t StateMachineExample::MSG_6 = 6;

StateMachineExample::StateMachineExample() : StateMachine("example") {
}

StateInit::StateInit(StateMachine *context) : State(context, "stateInit") {
}

void StateInit::enter() {
    LOGD("StateInit %s", __func__);
}

void StateInit::exit() {
    LOGD("StateInit %s", __func__);
}

bool StateInit::handleMessage(const Message &message) {
    LOGD("StateInit %s", __func__);
    switch (message.mWhat) {
    case StateMachineExample::MSG_4:
        mContext->transitStateTo(StateMachineExample::STATE_1);
        break;
    default:
        break;
    }
    return true;
}

State1::State1(StateMachine *context) : State(context, "state1") {
}

void State1::enter() {
    LOGD("State1 %s", __func__);
}

void State1::exit() {
    LOGD("State1 %s", __func__);
}

bool State1::handleMessage(const Message &message) {
    LOGD("State1 %s", __func__);
    switch (message.mWhat) {
    case StateMachineExample::MSG_1:
        mContext->transitStateTo(StateMachineExample::STATE_2);
        break;
    default:
        break;
    }
    return true;
}

State2::State2(StateMachine *context) : State(context, "state2") {
}

void State2::enter() {
    LOGD("State2 %s", __func__);
}

void State2::exit() {
    LOGD("State2 %s", __func__);
}

bool State2::handleMessage(const Message &message) {
    LOGD("State2 %s", __func__);
    switch (message.mWhat) {
    case StateMachineExample::MSG_2:
        mContext->transitStateTo(StateMachineExample::STATE_3);
        break;
    default:
        break;
    }
    return true;
}

State3::State3(StateMachine *context) : State(context, "state3") {
}

void State3::enter() {
    LOGD("State3 %s", __func__);
}

void State3::exit() {
    LOGD("State3 %s", __func__);
}

bool State3::handleMessage(const Message &message) {
    LOGD("State3 %s", __func__);
    switch (message.mWhat) {
    case StateMachineExample::MSG_3:
        mContext->transitStateTo(StateMachineExample::STATE_INIT);
        break;
    default:
        break;
    }
    return true;
}

}  // namespace cpfw

int main() {
    std::shared_ptr<cpfw::StateMachine> machine = std::make_shared<cpfw::StateMachineExample>();

    std::shared_ptr<cpfw::State> initState = std::make_shared<cpfw::StateInit>(machine.get());
    machine->addState(cpfw::StateMachineExample::STATE_INIT, initState);
    std::shared_ptr<cpfw::State> state1 = std::make_shared<cpfw::State1>(machine.get());
    machine->addState(cpfw::StateMachineExample::STATE_1, state1);
    std::shared_ptr<cpfw::State> state2 = std::make_shared<cpfw::State2>(machine.get());
    machine->addState(cpfw::StateMachineExample::STATE_2, state2);
    std::shared_ptr<cpfw::State> state3 = std::make_shared<cpfw::State3>(machine.get());
    machine->addState(cpfw::StateMachineExample::STATE_3, state3);

    machine->setInitialState(cpfw::StateMachineExample::STATE_INIT);

    cpfw::Message message;
    message.mWhat = cpfw::StateMachineExample::MSG_4;
    machine->sendMessage(message);
    message.mWhat = cpfw::StateMachineExample::MSG_1;
    machine->sendMessage(message);
    message.mWhat = cpfw::StateMachineExample::MSG_2;
    machine->sendMessage(message);
    message.mWhat = cpfw::StateMachineExample::MSG_3;
    machine->sendMessage(message);
    message.mWhat = cpfw::StateMachineExample::MSG_4;
    machine->sendMessage(message);
    message.mWhat = cpfw::StateMachineExample::MSG_5;
    machine->sendMessage(message);

    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    return 0;
}
