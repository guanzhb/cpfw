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

#ifndef CPFW_BASE_INCLUDE_STATEMACHINE_H_
#define CPFW_BASE_INCLUDE_STATEMACHINE_H_

#include <map>
#include <memory>

#include "Handler.h"
#include "State.h"

namespace cpfw {

class State;

class StateMachine {
 public:
    StateMachine();
    explicit StateMachine(const std::string &name);
    ~StateMachine();

    const std::string& getName() const;

    void addState(uint32_t stateId, std::shared_ptr<State> state);

    void removeState(uint32_t stateId);

    void transitStateTo(uint32_t stateId);

    void setInitialState(uint32_t stateId);

    void sendMessage(Message message);

    std::shared_ptr<State> getCurrentState();

 private:
    class StateMachineHandler : public Handler {
     public:
         StateMachineHandler(StateMachine *stateMachine);
         ~StateMachineHandler();

         int32_t onInvoke(const Message &message) override;

         void onReply(const Message &message, const int32_t status) override;

     private:
         StateMachine* mStateMachine;
    };

 private:
    const std::string mName;
    std::unique_ptr<Handler> mHandler;
    std::map<uint32_t, std::shared_ptr<State>> mStateTable;
    std::shared_ptr<State> mCurrentState;
};  // StateMachine

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_STATEMACHINE_H_

