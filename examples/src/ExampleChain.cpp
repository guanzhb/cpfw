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
#define TAG "Example"

#include "examples/include/ExampleChain.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "cpfw/base/include/Base.h"
#include "cpfw/base/include/Utils.h"
#include "cpfw/base/include/Log.hpp"

namespace cpfw {

static int32_t func0() {
    LOGI(TAG, "func0");
    return 0;
}

static int32_t func1(int32_t v1) {
    LOGI(TAG, "func0 v1:" + std::to_string(v1));
    return 0;
}

static int32_t func2(int32_t v1, int32_t v2) {
    LOGI(TAG, "func0 v1:" + std::to_string(v1) + ", v2:" + std::to_string(v2));
    return 0;
}

static int32_t func4(int32_t v1, int32_t v2, int32_t v3, int32_t v4) {
    LOGI(TAG, "func0 v1:" + std::to_string(v1) + ", v2:" + std::to_string(v2)
         + ", v3:" + std::to_string(v3) + ", v4:" + std::to_string(v4));
    return 0;
}

static int32_t handle(
        DataStore *store, const std::string &funcName,
        const std::vector<int32_t> &values) {
    LOGI(TAG, funcName);
    std::for_each(values.begin(), values.end(), [](auto d) -> void {
        LOGI(TAG, "" + std::to_string(d));
    });
    return 0;
}

static void onReply(const std::string &widgetName, const std::string &elementName,
             const int32_t value, const int32_t status) {
    LOGI(TAG, "reply: widget:" + widgetName + ", element:" + elementName
        + ", value:" + std::to_string(value) + ", status:" + std::to_string(status));
}

class WidgetStub : public Widget {
 public:
    WidgetStub(std::string name) : Widget(name) {
    }

    int32_t action() override {
        LOGI(TAG, "widget override " + getName()
             + " action time: " + std::to_string(cpfw::getCurrentTimeMs()));
        uint32_t type = static_cast<uint32_t>(ElementType::PUBLIC);
        auto values = parseProfile(
            getDataStore()->getProfile(getName()), type, getName(), getDataStore());
        int32_t ret = handle(getDataStore().get(), getName() + " override action ", values);
        LOGI(TAG, "WidgetStub action status: " + std::to_string(ret));
        return ret;
    }
};

ExampleChain::ExampleChain() {
    mLogic = std::make_shared<Logic>("./configs/exampleChain.xml");

    auto sv = std::make_shared<Widget>("volume", func1);
    mLogic->addWidget(sv);
    auto sl = std::make_shared<Widget>("loudness", func1);
    mLogic->addWidget(sl);
    auto sf = std::make_shared<Widget>("fade", func1);
    mLogic->addWidget(sf);
    auto se = std::make_shared<Widget>("equalizer", func4);
    mLogic->addWidget(se);
    auto sd = std::make_shared<Widget>("duck");
    mLogic->addWidget(sd);
    auto ss = std::make_shared<WidgetStub>("stub");
    mLogic->addWidget(ss);

    mLogic->registerCallback(onReply);
}

ExampleChain::~ExampleChain() {
}

int32_t ExampleChain::setVolume(int32_t volume) {
    return mLogic->setProfile("volume", volume);
}

int32_t ExampleChain::setFade(int32_t fade) {
    return mLogic->setProfile("fade", fade);
}

int32_t ExampleChain::setEq(std::string band, int32_t db) {
    return mLogic->setProfile("equalizer", band, db);
}

int32_t ExampleChain::setLoudness(int32_t loudness) {
    return mLogic->setProfile("loudness", loudness);
}

int32_t ExampleChain::setStub(int32_t stub) {
    LOGI(TAG, "stub call time: " + std::to_string(cpfw::getCurrentTimeMs()));
    return mLogic->setProfileDelay("stub", stub, 10, PostFlag::SYNC);
}

}  // namespace cpfw

int main() {
    std::unique_ptr<cpfw::ExampleChain> example = std::make_unique<cpfw::ExampleChain>();
    LOGI(TAG, "start");

    example->setStub(10);
    LOGI(TAG, "stub success");

    example->setVolume(30);
    LOGI(TAG, "volume success");

    example->setFade(50);
    LOGI(TAG, "fade success");

    example->setEq("gain_100hz", 100);
    LOGI(TAG, "eq success");

    example->setLoudness(120);
    LOGI(TAG, "loudness success");

    example->setVolume(300);
    LOGI(TAG, "volume success");

    example->setEq("gain_400hz", 90);
    LOGI(TAG, "eq success");

    example->setLoudness(10);
    LOGI(TAG, "loudness success");

    example->setStub(40);
    LOGI(TAG, "stub success");

    LOGI(TAG, "start time: " + std::to_string(cpfw::getCurrentTimeMs()));
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    LOGI(TAG, "ExampleChain exit");
    LOGI(TAG, "end time: " + std::to_string(cpfw::getCurrentTimeMs()));

    return 0;
}
