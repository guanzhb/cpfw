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

#define TAG "Handler"

#include "Handler.h"

#include <chrono>

#include "Log.hpp"
#include "Utils.h"

namespace cpfw {

Handler::Handler() {
    mMsgPool = std::make_unique<MessagePool>();
    mRunning.store(true);
    mWorkingThread = std::thread(&Handler::handleMessage, this);
}

Handler::~Handler() {
    mRunning.store(false);
    Message msg;
    post(msg);
    mWorkingThread.join();
}

int32_t Handler::post(const Message &msg) {
    return postDelay(msg, 0);
}

int32_t Handler::postDelay(const Message &msg, uint64_t delayMs) {
    LOGI("postDelay flag:" + std::to_string(msg.mFlag));

    if (0 != (msg.mFlag & PostFlag::SYNC)) {
        return onInvoke(msg);
    }

    uint64_t whenMs = getCurrentTimeMs() + delayMs;
    int32_t ret = 0;
    if (0 != (msg.mFlag & PostFlag::DELETE_FORMER)) {
        mMsgPool->postAndDeleteFormers(whenMs, msg, msg.mWhat);
    } else if (0 != (msg.mFlag & PostFlag::OMIT_IF_EXIST)) {
        mMsgPool->postButOmitIfExist(whenMs, msg, msg.mWhat);
    } else {
        mMsgPool->post(whenMs, msg, msg.mWhat);
    }
    return ret;
}

int32_t Handler::postWhen(const Message &msg, uint64_t whenMs) {
    uint64_t currentTimeMs = getCurrentTimeMs();
    if (currentTimeMs > whenMs) {
        return 0;  // The task is planned to do at a past time. Ignore it.
    }

    return postDelay(msg, whenMs - currentTimeMs);
}

void Handler::handleMessage() {
    LOGI("handleMessage");
    while (mRunning.load()) {
        uint64_t currentTimeMs = getCurrentTimeMs();
        auto itor = mMsgPool->front();
        if (itor->first > currentTimeMs) {
            if (std::cv_status::no_timeout
                    == mMsgPool->waitFor(itor->first-currentTimeMs)) {
                continue;
            }
        }
        if (!mRunning.load()) {
            break;
        }

        const Message &msg = itor->second;

        int32_t status = onInvoke(msg);
        LOGI("handleMessage invoke over status: " + std::to_string(status));
        reply(msg, status);

        if (0 != (msg.mFlag & PostFlag::LOOP)) {
            uint64_t delayMs = 0;
            Bundle &bundle = const_cast<Message&>(msg).mBundle;
            if (bundle.get(KEY_DELAY_TIME_MS, delayMs)) {
                postDelay(msg, delayMs);
            }
        }

        // why del here? msg is a reference, if del before LOOP check, msg would be null
        mMsgPool->popFront();
    }
}

void Handler::reply(const Message &msg, int32_t status) {
    if (nullptr != msg.mCallback) {
        LOGI("reply to widget");
        msg.mCallback(status);
    } else {
        LOGI("reply to Handler status: " + std::to_string(status));
        onReply(msg, status);
    }
}

}  // namesapce cpfw

