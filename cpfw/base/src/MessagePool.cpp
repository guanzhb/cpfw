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

#include "cpfw/base/include/MessagePool.h"

namespace cpfw {

MessagePool::MessagePool() {
}

MessagePool::~MessagePool() {
}

void MessagePool::post(
        const uint64_t whenMs, const Message &message, const uint64_t what) {
    std::unique_lock<std::mutex> lck(mMutex);
    postWithLock(whenMs, message, what);
    notify();
}

void MessagePool::postAndDeleteFormers(
        const uint64_t whenMs, const Message &message, const uint64_t what) {
    std::unique_lock<std::mutex> lck(mMutex);
    deleteMessage(what);
    postWithLock(whenMs, message, what);
    notify();
}

void MessagePool::postButOmitIfExist(
        const uint64_t whenMs, const Message &message, const uint64_t what) {
    std::unique_lock<std::mutex> lck(mMutex);
    if (findEntry(what)) {
        return;
    }
    postWithLock(whenMs, message, what);
    notify();
}

std::multimap<const uint64_t, const Message>::iterator MessagePool::front() {
    wait();
    return mQueue.begin();
}

void MessagePool::popFront() {
    std::unique_lock<std::mutex> lck(mMutex);
    if (!mQueue.empty()) {
        auto itorToDel = mQueue.begin();
        // mQueue must keep path with mFlagTable, so no need to check nullptr
        auto range = mFlagTable.equal_range(itorToDel->second.mWhat);
        if (range.first != range.second) {
            mFlagTable.erase(range.first);
        }
        mQueue.erase(itorToDel);
    }
}

void MessagePool::clear() {
    std::unique_lock<std::mutex> lck(mMutex);
    mQueue.clear();
    mFlagTable.clear();
}

void MessagePool::notify() {
    mConditionVariable.notify_one();
}

void MessagePool::wait() {
    std::unique_lock<std::mutex> lck(mMutex);
    if (mQueue.empty()) {
        mConditionVariable.wait(lck);
    }
}

std::cv_status MessagePool::waitFor(uint64_t waitTimeMs) {
    std::unique_lock<std::mutex> lck(mMutex);
    return mConditionVariable.wait_for(lck, std::chrono::milliseconds(waitTimeMs));
}

void MessagePool::postWithLock(
        const uint64_t whenMs, const Message &message, const uint64_t what) {
    mQueue.emplace(whenMs, message);
    mFlagTable.emplace(what, whenMs);
}

void MessagePool::deleteMessage(const uint64_t what) {
    std::erase_if(mQueue, [&what](const auto& item) {
        auto const& [whenMs, message] = item;
        return message.mWhat == what;
    });

    mFlagTable.erase(what);
}

bool MessagePool::findEntry(const uint64_t what) {
    return mFlagTable.find(what) != mFlagTable.end();
}

}  // namespace cpfw

