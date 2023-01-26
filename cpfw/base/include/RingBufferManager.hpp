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
#define LOG_TAG "RingBufferManager"

#include <condition_variable>
#include <functional>
#include <memory>
#include <shared_mutex>
#include <string>
#include <vector>

#include "Handler.h"
#include "Log.hpp"
#include "RingBuffer.hpp"

namespace cpfw {

namespace {
    const std::string KEY_WRITE = "funcWrite";
    const std::string KEY_READ = "funcRead";
    const std::string KEY_DATA = "data";

    const uint32_t WHAT_WRITE_DIRECT = 0U;
    const uint32_t WHAT_WRITE_LOOP = 1U;
    const uint32_t WHAT_READ_DIRECT = 2U;
    const uint32_t WHAT_READ_LOOP = 3U;
}

template<typename T, int32_t N>
class RingBufferManager {
 public:
    using FUNCTION_READ = std::function<int32_t(std::vector<T> &readBuffer, int32_t readSize)>;
    using FUNCTION_WRITE = std::function<int32_t()>;

    explicit RingBufferManager(const std::string &name = "default") : mName(name) {
    }

    int32_t write(const std::vector<T> &writeBuffer) {
        if (mBuffer.getIdleSize() >= writeBuffer.size()) {
            return writeLock(writeBuffer);
        }

        Message msg;
        msg.mWhat = WHAT_WRITE_DIRECT;
        Bundle bundle;
        bundle.set(KEY_DATA, writeBuffer);
        msg.mBundle = bundle;
        makeWriteHandlerIfNull();
        mWriteHandler->post(msg);
        return 0;
    }

    int32_t read(std::vector<T> &readBuffer, int32_t readSize) {
        return readLock(readBuffer, readSize);
    }

    int32_t registerWrite(FUNCTION_WRITE funcWrite, uint64_t intervalTimeMs) {
        Message msg;
        msg.mWhat = WHAT_WRITE_LOOP;
        msg.mFlag = PostFlag::LOOP;
        Bundle bundle;
        bundle.set(KEY_DELAY_TIME_MS, intervalTimeMs);
        bundle.set(KEY_WRITE, funcWrite);
        msg.mBundle = bundle;
        makeWriteHandlerIfNull();
        mWriteHandler->post(msg);
        return 0;
    }

    int32_t unregisterWrite() {
        Message msg;
        msg.mWhat = WHAT_WRITE_LOOP;
        msg.mFlag = PostFlag::DELETE_FORMER;
        makeWriteHandlerIfNull();
        mWriteHandler->post(msg);
        return 0;
    }

    int32_t registerRead(FUNCTION_READ funcRead, int32_t readSize, uint64_t intervalTimeMs) {
        Message msg;
        msg.mWhat = WHAT_READ_LOOP;
        msg.mFlag = PostFlag::LOOP;
        msg.mArg1 = readSize;
        Bundle bundle;
        bundle.set(KEY_DELAY_TIME_MS, intervalTimeMs);
        bundle.set(KEY_READ, funcRead);
        msg.mBundle = bundle;
        makeReadHandlerIfNull();
        mReadHandler->post(msg);
        return 0;
    }

    int32_t unregisterRead() {
        Message msg;
        msg.mWhat = WHAT_READ_LOOP;
        msg.mFlag = PostFlag::DELETE_FORMER;
        makeReadHandlerIfNull();
        mReadHandler->post(msg);
        return 0;
    }

#ifdef TEST
    RingBuffer<T, N> &get() {
        return mBuffer;
    }
#endif

 private:
    void makeWriteHandlerIfNull() {
        if (!mWriteHandler) {
            mWriteHandler = std::make_unique<WriteRingHandler>(this);
        }
    }

    void makeReadHandlerIfNull() {
        if (!mReadHandler) {
            mReadHandler = std::make_unique<ReadRingHandler>(this);
        }
    }

    int32_t writeLock(const std::vector<T> &writeBuffer) {
        while (mBuffer.getIdleSize() < writeBuffer.size()) {
            LOGW("%s overrun", mName.c_str());
            std::unique_lock<std::shared_mutex> lck(mMutex);
            mConditionVariable.wait(lck);
        }
        mBuffer.write(writeBuffer);
        mConditionVariable.notify_all();
        return 0;
    }

    int32_t readLock(std::vector<T> &readBuffer, int32_t readSize) {
        while (mBuffer.getAvailableSize() < readSize) {
            LOGW("%s underrun", mName.c_str());
            std::shared_lock<std::shared_mutex> lck(mMutex);
            mConditionVariable.wait(lck);
        }
        mBuffer.read(readBuffer, readSize);
        mConditionVariable.notify_all();
        return 0;
    }

    class WriteRingHandler : public Handler {
     public:
        WriteRingHandler(RingBufferManager *rbm) : mRmb(rbm) {
        }

        int32_t onInvoke(const Message &message) override {
            Bundle &bundle = const_cast<Message&>(message).mBundle;
            switch (message.mWhat) {
            case WHAT_WRITE_DIRECT: {
                std::vector<T> writeBuffer;
                bundle.get(KEY_DATA, writeBuffer);
                mRmb->writeLock(writeBuffer);
                break;
            }
            case WHAT_WRITE_LOOP: {
                RingBufferManager<T, N>::FUNCTION_WRITE funcWrite;
                bundle.get(KEY_WRITE, funcWrite);
                funcWrite();
                break;
            }
            default:
                break;
            }
            return 0;
        }

        void onReply(const Message &msg, const int32_t status) override {
        }

     private:
        RingBufferManager *mRmb;
    };

    class ReadRingHandler : public Handler {
     public:
         ReadRingHandler(RingBufferManager *rbm) : mRmb(rbm) {
         }

        int32_t onInvoke(const Message &message) override {
            Bundle &bundle = const_cast<Message&>(message).mBundle;
            switch (message.mWhat) {
            case WHAT_READ_LOOP: {
                std::vector<T> readBuffer;
                RingBufferManager<T, N>::FUNCTION_READ funcRead;
                bundle.get(KEY_READ, funcRead);
                mRmb->readLock(readBuffer, message.mArg1/*size*/);
                funcRead(readBuffer, message.mArg1/*size*/);
                break;
            }
            default:
                break;
            }
            return 0;
        }

        void onReply(const Message &msg, const int32_t status) override {
        }

     private:
         RingBufferManager *mRmb;
    };

 private:
    const std::string mName;
    mutable std::shared_mutex mMutex;
    std::condition_variable_any mConditionVariable;
    RingBuffer<T, N> mBuffer;
    std::unique_ptr<WriteRingHandler> mWriteHandler;
    std::unique_ptr<ReadRingHandler> mReadHandler;
};

}  // namespace cpfw

