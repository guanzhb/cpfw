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
#ifndef CPFW_BASE_INCLUDE_HANDLER_H_
#define CPFW_BASE_INCLUDE_HANDLER_H_

#include <atomic>
#include <memory>
#include <thread>

#include "Message.h"
#include "MessagePool.h"

namespace cpfw {

class Handler {
 public:
    Handler();
    ~Handler();

    int32_t post(const Message &msg);

    int32_t postDelay(const Message &msg, uint64_t delayMs);

    int32_t postWhen(const Message &msg, uint64_t whenMs);

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

#endif  // CPFW_BASE_INCLUDE_HANDLER_H_
