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

#include <memory>
#include <string>

#include "Base.h"
#include "DataStore.h"
#include "Handler.h"
#include "ResponsibilityChain.h"
#include "Widget.h"

namespace cpfw {

using TCallbackWithName = std::function<void(const std::string &widgetName,
                         const std::vector<TElementPairWithName> &elementPairs,
                         const int32_t status)>;

using TCallbackWithId = std::function<void(const uint32_t widgetId,
                         const std::vector<TElementPairWithId> &elementPairs,
                         const int32_t status)>;

class Logic {
 public:
    Logic();

    Logic(const std::string &configurationFile);

    ~Logic();

    void registerCallback(TCallbackWithName callbackWithName);
    void unregisterCallback(TCallbackWithName callbackWithName);
    void registerCallback(TCallbackWithId callbackWithId);
    void unregisterCallback(TCallbackWithId callbackWithId);

    void addWidget(std::shared_ptr<Widget> widget);

    int32_t setProfile(const uint32_t widgetId,
                const std::vector<TElementPairWithId> &elementPairs,
                const PostFlag flag = PostFlag::NONE);
    int32_t setProfile(const std::string &widgetName,
                const std::vector<TElementPairWithName> &elementPairs,
                const PostFlag flag = PostFlag::NONE);

    int32_t setProfileDelay(const uint32_t widgetId,
                const std::vector<TElementPairWithId> &elementPairs,
                uint64_t delayTimeMs,
                const PostFlag flag = PostFlag::NONE);
    int32_t setProfileDelay(const std::string &widgetName,
                const std::vector<TElementPairWithName> &elementPairs,
                uint64_t delayTimeMs,
                const PostFlag flag = PostFlag::NONE);

    std::map<uint32_t, int32_t> getProfile(const uint32_t widgeteId,
         const std::vector<uint32_t> elementId = {});
    std::map<uint32_t, int32_t> getProfile(const std::string &widgeteName,
         const std::vector<std::string> &elementName = {"default"});

    void onReply(const Message &message, const int32_t status);

 private:
    class LogicHandler : public Handler {
     public:
        LogicHandler(Logic* logic);
        ~LogicHandler();

        int32_t onInvoke(const Message &message) override;

        void onReply(const Message &message, const int32_t status) override;

     private:
        Logic* mLogic;
    };

 private:
    TCallbackWithName mCallbackWithName;
    TCallbackWithId mCallbackWithId;
    std::unique_ptr<Handler> mHandler;
    std::shared_ptr<DataStore> mStore;
    std::unique_ptr<ResponsibilityChain> mResponsibilityChain;
};

}  // namespace cpfw

#endif  // CPFW_CORE_INCLUDE_DATAPARSER_H_

