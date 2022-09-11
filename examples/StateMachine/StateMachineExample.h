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

#ifndef EXAMPLES_STATEMACHINE_INCLUDE_STATEMACHINEEXAMPLE_H_
#define EXAMPLES_STATEMACHINE_INCLUDE_STATEMACHINEEXAMPLE_H_

#include "StateMachine.h"

namespace cpfw {

class StateMachineExample : public StateMachine {
 public:
    StateMachineExample();
    static const uint32_t STATE_INIT;
    static const uint32_t STATE_1;
    static const uint32_t STATE_2;
    static const uint32_t STATE_3;

    static const uint32_t MSG_1;
    static const uint32_t MSG_2;
    static const uint32_t MSG_3;
    static const uint32_t MSG_4;
    static const uint32_t MSG_5;
    static const uint32_t MSG_6;
};

class StateInit : public State {
 public:
    StateInit(StateMachine *context);

    void enter() override;
    void exit() override;
    bool handleMessage(const Message &message) override;
};

class State1 : public State {
 public:
    State1(StateMachine *context);

    void enter() override;
    void exit() override;
    bool handleMessage(const Message &message) override;
};

class State2 : public State {
 public:
    State2(StateMachine *context);

    void enter() override;
    void exit() override;
    bool handleMessage(const Message &message) override;
};

class State3 : public State {
 public:
    State3(StateMachine *context);

    void enter() override;
    void exit() override;
    bool handleMessage(const Message &message) override;
};

}  // namespace cpfw
#endif  // EXAMPLES_STATEMACHINE_INCLUDE_STATEMACHINEEXAMPLE_H_
