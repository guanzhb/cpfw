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

#include <memory>
#include <string>
#include <vector>

#include "Base.h"
#include "Log.hpp"
#include "Utils.h"

namespace cpfw {

static int32_t func0() {
    LOGI("func0");
    return 0;
}

static std::string parseValue(std::vector<int32_t> &v) {
    std::string tmp;
    for (auto s : v) {
        tmp.append(":");
        tmp.append(std::to_string(s));
    }
    return tmp;
}

static int32_t funcv1(std::vector<int32_t> &v) {
    LOGI("funcv1 " + parseValue(v));
    return 0;
}

static int32_t funcv2(std::vector<int32_t> &v) {
    LOGI("funcv2 " + parseValue(v));
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

static void onReply(const std::string &widgetName,
        const std::vector<TElementPairWithName> &elementPair, const int32_t status) {
    LOGI("reply: widget:" + widgetName + ", status:" + std::to_string(status));
}

class WidgetStub : public Widget {
 public:
    WidgetStub(std::string name, uint32_t id) : Widget(name, id) {
    }

    int32_t action() override {
        LOGI("widget override " + getName() + " action");
        uint32_t type = ElementType::PUBLIC;
        auto values = parseProfile(
            getDataStore()->getProfile(getId()), type, getId(), getDataStore());
        int32_t ret = handle(getDataStore().get(), getName() + " override action ", values);
        LOGI("WidgetStub action status: " + std::to_string(ret));
        return ret;
    }
};

ExampleChain::ExampleChain() {
    mLogic = std::make_shared<Logic>("./exampleChain.xml");

    auto sv = std::make_shared<Widget>("volume", 11221U, funcv1);
    mLogic->addWidget(sv);
    auto sl = std::make_shared<Widget>("loudness", 11225U, funcv2);
    mLogic->addWidget(sl);
    auto sf = std::make_shared<Widget>("fade", 11222U, funcv1);
    mLogic->addWidget(sf);
    auto se = std::make_shared<Widget>("equalizer", 11223U, funcv1);
    mLogic->addWidget(se);
    auto sd = std::make_shared<Widget>("duck", 11224U);
    mLogic->addWidget(sd);
    auto ss = std::make_shared<WidgetStub>("stub", 11226U);
    mLogic->addWidget(ss);
    auto sloop = std::make_shared<Widget>("loop", 11227U, funcv2);
    mLogic->addWidget(sloop);

    mLogic->registerCallback(onReply);
}

ExampleChain::~ExampleChain() {
}

int32_t ExampleChain::setVolume(int32_t volume) {
    return mLogic->setProfile(11221U, {{0U, volume}});
}

int32_t ExampleChain::setFade(int32_t fade) {
    return mLogic->setProfile(11222U, {{0U, fade}});
}

int32_t ExampleChain::setEq(uint32_t band, int32_t db) {
    return mLogic->setProfile(11223U, {{band, db}});
}

int32_t ExampleChain::setLoudness(int32_t loudness) {
    return mLogic->setProfileDelay(11225U, {{0U, loudness}, {1U, 15}}, 10U);
}

int32_t ExampleChain::setStub(int32_t stub) {
    LOGI("stub call");
    return mLogic->setProfileDelay(11226U, {{0U, stub}}, 10U, PostFlag::SYNC);
}

int32_t ExampleChain::setLoop(int32_t loop) {
    LOGI("loop call");
    return mLogic->setProfileDelay("loop", {{"default", loop}}, 500U, PostFlag::LOOP);
}

int32_t ExampleChain::delLoop() {
    LOGI("del loop call");
    return mLogic->setProfileDelay(11227U, {{0U, 0}}, 500U, PostFlag::DELETE_FORMER);
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

    example->setEq(0U, 100);
    LOGI("eq success");

    example->setLoudness(120);
    LOGI("loudness success");

    example->setVolume(20);
    LOGI("volume success");

    example->setEq(4U, 5);
    LOGI("eq success");

    example->setLoudness(10);
    LOGI("loudness success");

    example->setStub(40);
    LOGI("stub success");

    example->setLoop(40);
    LOGI("stub success");

    LOGI("sleep 2s begin");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000U));
    LOGI("sleep 2s over");

    example->delLoop();

    LOGI("sleep 5s begin");
    std::this_thread::sleep_for(std::chrono::milliseconds(5000U));
    LOGI("sleep 5s over");

    LOGI("ExampleChain exit");
    LOGI("end");

    return 0;
}
