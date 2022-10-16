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

#define TAG "SimpleLogicExample"

#include "SimpleLogicExample.h"

#include "Log.hpp"

namespace cpfw {

SimpleLogicExample::SimpleLogicExample(
        const std::string &name,
        std::shared_ptr<SimpleLogicHelper> helper)
    : SimpleLogic(name, helper) {
}

}  // namespace cpfw

static int32_t func0() {
    LOGI("func0");
    return 0;
}

static int32_t func1(int32_t v1) {
    LOGI("func1 v1:" + std::to_string(v1));
    return 0;
}

static int32_t func2(int32_t v1, int32_t v2) {
    LOGI("func2 v1:" + std::to_string(v1) + ", v2:" + std::to_string(v2));
    return 0;
}

static int32_t func3(int32_t v1, int32_t v2, int32_t v3) {
    LOGI("func4 v1:" + std::to_string(v1) + ", v2:" + std::to_string(v2)
         + ", v3:" + std::to_string(v3));
    return 0;
}

static int32_t func4(int32_t v1, int32_t v2, int32_t v3, int32_t v4) {
    LOGI("func4 v1:" + std::to_string(v1) + ", v2:" + std::to_string(v2)
         + ", v3:" + std::to_string(v3) + ", v4:" + std::to_string(v4));
    return 0;
}

int main() {
    auto helper = std::make_shared<cpfw::SimpleLogicHelper>();

    auto sc = std::make_shared<cpfw::Widget>("screen", 1, func1);
    helper->addWidget(sc);
    auto wifi = std::make_shared<cpfw::Widget>("wifi", 2, func2);
    helper->addWidget(wifi);
    auto sound = std::make_shared<cpfw::Widget>("sound", 3, func1);
    helper->addWidget(sound);
    auto gps = std::make_shared<cpfw::Widget>("gps", 4, func1);
    helper->addWidget(gps);
    auto app = std::make_shared<cpfw::Widget>("app", 5, func3);
    helper->addWidget(app);
    auto bt = std::make_shared<cpfw::Widget>("bluetooth", 6, func1);
    helper->addWidget(bt);

    helper->loadConfiguration("./simpleLogic.xml");
    
    cpfw::SimpleLogic sl {"smc", helper};

    cpfw::Message message;
    message.mWhat = 1;
    sl.sendMessage(message);
    message.mWhat = 2;
    sl.sendMessage(message);
    message.mWhat = 3;
    sl.sendMessage(message);

    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    return 0;
}

