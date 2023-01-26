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

#define LOG_TAG "singleton-a"

#include "Log.hpp"
#include "Singleton.hpp"
#include "Log.hpp"


using namespace cpfw;

namespace cpfw {

class SingletonA : public Singleton<SingletonA> {
 public:
    SingletonA(int32_t a) : SingletonA(a, 0) {
        LOGI("ctor a:%d", a);
    }

    SingletonA(int32_t a, uint8_t b) : mA(a), mB(b) {
        LOGI("ctor a:%d, b:%d", a, b);
    }

    int32_t getA() {
        return mA;
    }

    int32_t getB() {
        return mB;
    }

 private:
    int32_t mA;
    uint8_t mB;
};
} // namespace cpfw

int main() {
    auto a = cpfw::SingletonA::getInstance(2);
    LOGI("get a::a:%d", a->getA());
    LOGI("get a::b:%d", a->getB());

    auto b = cpfw::SingletonA::getInstance(4, 5);
    LOGI("get a::a:%d", a->getA());
    LOGI("get a::b:%d", a->getB());
    LOGI("get b::a:%d", b->getA());
    LOGI("get b::b:%d", b->getB());

    return 0;
}
