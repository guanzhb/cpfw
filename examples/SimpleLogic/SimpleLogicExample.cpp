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

using namespace cpfw;

namespace cpfw {

SimpleLogicExample::SimpleLogicExample(
        const std::string &name,
        std::shared_ptr<SimpleLogicHelper> helper)
    : SimpleLogic(name, helper) {
}

}  // namespace cpfw

static std::string parseValue(std::vector<int32_t> &v) {
    std::string tmp;
    for (auto s : v) {
        tmp.append(":");
        tmp.append(std::to_string(s));
    }
    return tmp;
}

static int32_t funcv1(std::vector<int32_t> &v) {
    LOGI("funcv1 %s", parseValue(v).c_str());
    return 0;
}

static int32_t funcv2(std::vector<int32_t> &v) {
    LOGI("funcv2 %s", parseValue(v).c_str());
    return 0;
}

int main() {
    auto helper = std::make_shared<cpfw::SimpleLogicHelper>();

    auto sc = std::make_shared<cpfw::Widget>("screen", 1U, funcv1);
    helper->addWidget(sc);
    auto wifi = std::make_shared<cpfw::Widget>("wifi", 2U, funcv2);
    helper->addWidget(wifi);
    auto sound = std::make_shared<cpfw::Widget>("sound", 3U, funcv1);
    helper->addWidget(sound);
    auto gps = std::make_shared<cpfw::Widget>("gps", 4U, funcv1);
    helper->addWidget(gps);
    auto app = std::make_shared<cpfw::Widget>("app", 5U, funcv2);
    helper->addWidget(app);
    auto bt = std::make_shared<cpfw::Widget>("bluetooth", 6U, funcv1);
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

    std::this_thread::sleep_for(std::chrono::milliseconds(20U));

    return 0;
}

