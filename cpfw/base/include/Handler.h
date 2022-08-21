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

#ifndef CPFW_BASE_INCLUDE_HANDLER_H__
#define CPFW_BASE_INCLUDE_HANDLER_H__

#include <atomic>
#include <memory>
#include <thread>
#include <iostream>

#include "cpfw/base/include/Message.h"
#include "cpfw/base/include/MessagePool.h"

namespace cpfw {

enum class PostFlag : uint8_t {
    NONE = 0x0,
    DELETE_FORMER = 0x1,
    OMIT_IF_EXIST = 0x2,
    SYNC = 0x3,
};

class Handler {
 public:
    Handler();
    ~Handler();

    int32_t post(const Message &msg, const PostFlag flag = PostFlag::NONE);

    int32_t postDelay(const Message &msg, uint64_t delayMs,
             const PostFlag flag = PostFlag::NONE);

    int32_t postWhen(const Message &msg, uint64_t whenMs,
             const PostFlag flag = PostFlag::NONE);

    /**
     * @brief deal message actually.
     * not to take too much time in it.
     *
     * @param msg what to deal.
     * @return int32_t 0 if success, else errno
     */
    virtual int32_t onInvoke(const Message &msg) = 0;

    virtual void onReply(const Message &msg, const int32_t status) = 0;
 private:
    void handleMessage();

    void reply(const Message &msg, int32_t status);

 private:
    std::unique_ptr<MessagePool> mMsgPool;
    std::atomic_bool mRunning;
    std::thread mWorkingThread;
};

}  // namesapce cpfw

#endif  // CPFW_BASE_INCLUDE_HANDLER_H__
