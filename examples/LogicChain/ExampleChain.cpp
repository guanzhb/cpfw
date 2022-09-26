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

#include "ExampleChain.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "Base.h"
#include "Utils.h"
#include "Log.hpp"

namespace cpfw {

static int32_t func0() {
    LOGI("func0");
    return 0;
}

static int32_t func1(int32_t v1) {
    LOGI("func0 v1:" + std::to_string(v1));
    return 0;
}

static int32_t func2(int32_t v1, int32_t v2) {
    LOGI("func0 v1:" + std::to_string(v1) + ", v2:" + std::to_string(v2));
    return 0;
}

static int32_t func4(int32_t v1, int32_t v2, int32_t v3, int32_t v4) {
    LOGI("func0 v1:" + std::to_string(v1) + ", v2:" + std::to_string(v2)
         + ", v3:" + std::to_string(v3) + ", v4:" + std::to_string(v4));
    return 0;
}

static int32_t handle(
        DataStore *store, const std::string &funcName,
        const std::vector<int32_t> &values) {
    LOGI(funcName);
    std::for_each(values.begin(), values.end(), [](auto d) -> void {
        LOGI("" + std::to_string(d));
    });
    return 0;
}

static void onReply(const std::string &widgetName, const std::string &elementName,
             const int32_t value, const int32_t status) {
    LOGI("reply: widget:" + widgetName + ", element:" + elementName
        + ", value:" + std::to_string(value) + ", status:" + std::to_string(status));
}

class WidgetStub : public Widget {
 public:
    WidgetStub(std::string name, uint32_t id) : Widget(name, id) {
    }

    int32_t action() override {
        LOGI("widget override " + getName() + " action");
        uint32_t type = static_cast<uint32_t>(ElementType::PUBLIC);
        auto values = parseProfile(
            getDataStore()->getProfile(getId()), type, getId(), getDataStore());
        int32_t ret = handle(getDataStore().get(), getName() + " override action ", values);
        LOGI("WidgetStub action status: " + std::to_string(ret));
        return ret;
    }
};

ExampleChain::ExampleChain() {
    mLogic = std::make_shared<Logic>("./exampleChain.xml");

    auto sv = std::make_shared<Widget>("volume", 11221, func1);
    mLogic->addWidget(sv);
    auto sl = std::make_shared<Widget>("loudness", 11225, func1);
    mLogic->addWidget(sl);
    auto sf = std::make_shared<Widget>("fade", 11222, func1);
    mLogic->addWidget(sf);
    auto se = std::make_shared<Widget>("equalizer", 11223, func4);
    mLogic->addWidget(se);
    auto sd = std::make_shared<Widget>("duck", 11224);
    mLogic->addWidget(sd);
    auto ss = std::make_shared<WidgetStub>("stub", 11226);
    mLogic->addWidget(ss);

    mLogic->registerCallback(onReply);
}

ExampleChain::~ExampleChain() {
}

int32_t ExampleChain::setVolume(int32_t volume) {
    return mLogic->setProfile(11221, volume);
}

int32_t ExampleChain::setFade(int32_t fade) {
    return mLogic->setProfile(11222, fade);
}

int32_t ExampleChain::setEq(uint32_t band, int32_t db) {
    return mLogic->setProfile(11223, band, db);
}

int32_t ExampleChain::setLoudness(int32_t loudness) {
    return mLogic->setProfile(11225, loudness);
}

int32_t ExampleChain::setStub(int32_t stub) {
    LOGI("stub call");
    return mLogic->setProfileDelay(11226, stub, 10, PostFlag::SYNC);
}

}  // namespace cpfw

int main() {
    std::unique_ptr<cpfw::ExampleChain> example = std::make_unique<cpfw::ExampleChain>();
    LOGI("start");

    example->setStub(10);
    LOGI("stub success");

    example->setVolume(30);
    LOGI("volume success");

    example->setFade(5);
    LOGI("fade success");

    example->setEq(0, 100);
    LOGI("eq success");

    example->setLoudness(120);
    LOGI("loudness success");

    example->setVolume(20);
    LOGI("volume success");

    example->setEq(4, 5);
    LOGI("eq success");

    example->setLoudness(10);
    LOGI("loudness success");

    example->setStub(40);
    LOGI("stub success");

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    LOGI("ExampleChain exit");
    LOGI("end");

    return 0;
}
