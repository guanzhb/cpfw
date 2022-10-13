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

#define TAG "ThreadPoolTest"

#include "ThreadPool.hpp"
#include "Log.hpp"

uint32_t getThreadId() {
    auto id = std::this_thread::get_id();
    return *((uint32_t*)&id);
}

void fun1(int32_t slp) {
    LOGI("  hello, fun1 ! " + std::to_string(getThreadId()));
    if (slp > 0) {
        LOGI(" ======= fun1 sleep " + std::to_string(slp)
            + " ==== " + std::to_string(getThreadId()));
        std::this_thread::sleep_for(std::chrono::milliseconds(slp));
    }
}

struct gfun {
    int32_t operator()(int32_t n) {
        LOGI(std::to_string(n) + " hello, gfun ! " + std::to_string(getThreadId()));
        return 42;
    }
};

class A {
public:
    static int32_t Afun(int32_t n = 0) {
        LOGI(std::to_string(n) + "hello, Afun ! " + std::to_string(getThreadId()));
        return n;
    }

    static std::string Bfun(int32_t n, std::string str, char c) {
        LOGI(std::to_string(n) + "hello, Bfun ! " + str + "  "
             + std::to_string(c) + std::to_string(getThreadId()));
        return str;
    }
};

int32_t main() {
    cpfw::ThreadPool executor { 50 };
    A a;
    std::future<void> ff = executor.commit(fun1, 0);
    std::future<int> fg = executor.commit(gfun{}, 0);
    std::future<int> gg = executor.commit(a.Afun, 9999);
    std::future<std::string> gh = executor.commit(A::Bfun, 9998, "mult args", 123);
    std::future<std::string> fh = executor.commit(
        []() -> std::string {
            LOGI("hello, fh !  " + std::to_string(getThreadId()));
            return "hello,fh ret !";
        });

    LOGI(" =======  sleep ========= " + std::to_string(getThreadId()));
    std::this_thread::sleep_for(std::chrono::microseconds(900));

    for (int32_t i = 0; i < 50; i++) {
        executor.commit(fun1, i*100 );
    }
    LOGI(" =======  commit all ========= " + std::to_string(getThreadId())
         + " idlsize=" + std::to_string(executor.getIdleThreadCount()));

    LOGI(" =======  sleep ========= " + std::to_string(getThreadId()));
    std::this_thread::sleep_for(std::chrono::seconds(3));

    ff.get();
    //LOGI(std::string(ff.get()) + " " + fh.get() + " " + std::to_string(getThreadId()));

    LOGI(" =======  sleep ========= " + std::to_string(getThreadId()));
    std::this_thread::sleep_for(std::chrono::seconds(3));

    LOGI(" =======  func1,55 ========= " + std::to_string(getThreadId()));
    executor.commit(fun1, 55).get();

    LOGI(" =======  end ========= " + std::to_string(getThreadId()));

    cpfw::ThreadPool pool(4);
    std::vector< std::future<int> > results;

    for (int32_t i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.commit([i] {
                LOGI(" =======  hello ========= " + std::to_string(i));
                std::this_thread::sleep_for(std::chrono::seconds(1));
                LOGI(" =======  world ========= " + std::to_string(i));
                return i*i;
            })
        );
    }
    LOGI(" =======  commit all2 ========= " + std::to_string(getThreadId()));

    std::string tmp;
    for (auto &&result : results) {
        tmp.append(std::to_string(result.get()));
        tmp.append(" ");
    }
    LOGI(tmp);
    return 0;
}

