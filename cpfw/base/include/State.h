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
#ifndef CPFW_BASE_INCLUDE_STATE_H_
#define CPFW_BASE_INCLUDE_STATE_H_

#include <string>

#include "Message.h"
#include "StateMachine.h"

namespace cpfw {

class StateMachine;

class State {
 public:
    State() : State(nullptr, "") {
    }

    explicit State(StateMachine *context, const std::string &name)
        : mContext(context), mName(name) {
    }

    virtual ~State() { }

    virtual void enter() { }

    virtual void exit() { }

    virtual bool handleMessage(const Message &message) {
        return true;
    }

    virtual const std::string& getName() const {
        return mName;
    }

 protected:
    const std::string mName;
    StateMachine *mContext;
};  // State

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_STATE_H_

