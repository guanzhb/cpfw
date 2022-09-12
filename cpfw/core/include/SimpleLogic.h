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

#ifndef CPFW_CORE_INCLUDE_SIMPLELOGIC_H_
#define CPFW_CORE_INCLUDE_SIMPLELOGIC_H_

#include <map>
#include <memory>

#include "Handler.h"
#include "SimpleLogicHelper.h"

namespace cpfw {

class SimpleLogic {
 public:
    SimpleLogic();
    explicit SimpleLogic(
        const std::string &name, std::shared_ptr<SimpleLogicHelper> helper);
    ~SimpleLogic();

    const std::string& getName() const;

    void sendMessage(Message message);

 private:
    class SimpleLogicHandler : public Handler {
     public:
         SimpleLogicHandler(
            SimpleLogic *SimpleLogic,
            std::shared_ptr<SimpleLogicHelper> helper);
         ~SimpleLogicHandler();

         int32_t onInvoke(const Message &message) override;

         void onReply(const Message &message, const int32_t status) override;

     private:
         SimpleLogic* mSimpleLogic;
         std::shared_ptr<SimpleLogicHelper> mHelper;
    };

 private:
    const std::string mName;
    std::unique_ptr<Handler> mHandler;
};  // SimpleLogic

}  // namespace cpfw

#endif  // CPFW_CORE_INCLUDE_SIMPLELOGIC_H_

