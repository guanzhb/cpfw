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

#ifndef CPFW_BASE_INCLUDE_MESSAGEPOOL_H_
#define CPFW_BASE_INCLUDE_MESSAGEPOOL_H_

#include <condition_variable>
#include <map>
#include <shared_mutex>

#include "Message.h"

namespace cpfw {

class MessagePool {
 public:
    MessagePool();

    ~MessagePool();

    void post(const uint64_t whenMs, const Message &message, const uint64_t what);

    void postAndDeleteFormers(
            const uint64_t whenMs, const Message &message, const uint64_t what);

    void postButOmitIfExist(
            const uint64_t whenMs, const Message &message, const uint64_t what);

    std::multimap<const uint64_t, const Message>::iterator front();

    void popFront();

    void clear();

    void notify();

    void wait();

    std::cv_status waitFor(uint64_t waitTimeMs = UINT64_MAX);

 private:
    void postWithLock(
            const uint64_t whenMs, const Message &message, const uint64_t what);

    void deleteMessage(const uint64_t what);

    bool findEntry(const uint64_t what);

 private:
    std::multimap<const uint64_t/*whenMs*/, const Message> mQueue;
    // perf for find when post with flag delete or omit
    std::multimap<uint64_t/*what*/, uint64_t/*whenMs*/> mFlagTable;
    mutable std::shared_mutex mMutex;
    std::condition_variable_any mCv;
};

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_MESSAGEPOOL_H_

