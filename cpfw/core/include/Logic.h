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

#ifndef CPFW_CORE_INCLUDE_LOGIC_H_
#define CPFW_CORE_INCLUDE_LOGIC_H_

#include <string>
#include <memory>

#include "cpfw/base/include/Base.h"
#include "cpfw/base/include/DataStore.h"
#include "cpfw/base/include/Widget.h"
#include "cpfw/base/include/ResponsibilityChain.h"
#include "cpfw/base/include/Handler.h"

namespace cpfw {


class Logic {
 public:
    Logic();

    Logic(std::string configurationFile);

    ~Logic();

    void initialize();

    void addWidget(std::shared_ptr<Widget> widget);

    int32_t setProfile(const std::string &widgetName,
                int32_t value, const PostFlag flag = PostFlag::NONE);
    int32_t setProfile(const std::string &widgetName, const std::string &elementName,
                int32_t value, const PostFlag flag = PostFlag::NONE);

    int32_t setProfileDelay(const std::string &widgetName,
                int32_t value, uint64_t delayTimeMs,
                const PostFlag flag = PostFlag::NONE);
    int32_t setProfileDelay(const std::string &widgetName,
                const std::string &elementName, int32_t value,
                uint64_t delayTimeMs, const PostFlag flag = PostFlag::NONE);

    int32_t getProfile(const std::string &widgetName);
    int32_t getProfile(const std::string &widgeteName,
                const std::string &elementName);

 private:
    class LogicHandler : public Handler {
     public:
         LogicHandler(Logic* logic);
         ~LogicHandler();

         int32_t onInvoke(const Message &message) override;

     private:
         Logic* mLogic;
    };

 private:
    std::unique_ptr<Handler> mHandler;
    std::shared_ptr<DataStore> mStore;
    std::unique_ptr<ResponsibilityChain> mResponsibilityChain;
};

}  // namespace cpfw

#endif  // CPFW_CORE_INCLUDE_DATAPARSER_H_

