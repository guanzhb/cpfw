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

#ifndef CPFW_BASE_INCLUDE_THREADPOOL_HPP__
#define CPFW_BASE_INCLUDE_THREADPOOL_HPP__

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <functional>
#include <vector>

namespace cpfw {

namespace {
    const uint8_t MAX_THREAD_COUNT = 16;
}

class ThreadPool {
 public:
    ThreadPool(uint8_t size = 4) {
        initThreadPool(size);
    }
    
    ~ThreadPool() {
        mIsRunning = false;
        mCv.notify_all();
        for (std::thread& thrd : mThreadPool) {
            if (thrd.joinable()) {
                thrd.join();
             }
        }
    }

    template<class F, class... Args>
    auto commit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using RetType = decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<RetType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        std::future<RetType> future = task->get_future();
        {
            std::lock_guard<std::mutex> lock { mLock };
            mTaskPool.emplace([task]() {
                (*task)();
            });
        }
        mCv.notify_one();

        return future;
    }

    uint8_t getIdleThreadCount() {
        return mIdleThreadCount;
    }

    uint8_t getThreadCount() {
        return mThreadPool.size();
    }

 private:
    void initThreadPool(uint8_t size) {
        for (; mThreadPool.size() < MAX_THREAD_COUNT && size > 0; --size) {
            mThreadPool.emplace_back([this] {
                while (true) {
                    Task task;
                    {
                        std::unique_lock<std::mutex> lock { mLock };
                        mCv.wait(lock, [this] {
                            return !mIsRunning || !mTaskPool.empty();
                        });
                        if (!mIsRunning && mTaskPool.empty()) {
                            return;
                        }
                        --mIdleThreadCount;
                        task = std::move(mTaskPool.front());
                        mTaskPool.pop();
                    }
                    task();
                    {
                        std::unique_lock<std::mutex> lock { mLock };
                        ++mIdleThreadCount;
                    }
                }
            });
            {
                std::unique_lock<std::mutex> lock { mLock };
                ++mIdleThreadCount;
            }
        }
    }
 private:
    using Task = std::function<void()>;
    std::vector<std::thread> mThreadPool;
    std::queue<Task> mTaskPool;
    std::mutex mLock;
    std::condition_variable mCv;
    std::atomic<bool> mIsRunning { true };
    std::atomic<uint8_t> mIdleThreadCount { 0 };
};

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_THREADPOOL_HPP__

