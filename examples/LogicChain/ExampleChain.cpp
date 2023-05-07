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
#define LOG_TAG "Example"

#include "ExampleChain.h"

#include <memory>
#include <string>
#include <vector>

#include "Base.h"
#include "Log.hpp"
#include "TimeUtils.h"

namespace cpfw {

static int32_t func0() {
    LOGI("func0");
    return 0;
}

static std::string parseValue(std::vector<int32_t> &value) {
    std::string tmp;
    std::for_each(value.begin(), value.end(), [&tmp] (auto &data) {
        tmp.append(":");
        tmp.append(std::to_string(data));
    });
    return tmp;
}

static int32_t funcv1(std::vector<int32_t> &value) {
    LOGI("funcv1 %s", parseValue(value).c_str());
    return 0;
}

static int32_t funcv2(std::vector<int32_t> &value) {
    LOGI("funcv2 %s", parseValue(value).c_str());
    return 0;
}

static int32_t handle(
        DataStore *store, const std::string &funcName,
        std::vector<int32_t> &value) {
    LOGI("%s", funcName.c_str());
    LOGI("handle %s", parseValue(value).c_str());
    return 0;
}

static void onReply(const std::string &widgetName,
        const std::vector<TElementPairWithName> &elementPair, const int32_t status) {
    LOGI("reply: widget:%s, status:%d", widgetName.c_str(), status);
}

class WidgetStub : public Widget {
 public:
    WidgetStub(std::string name, uint32_t id) : Widget(name, id) {
    }

    int32_t action() override {
        LOGI("widget override %s action", getName().c_str());
        uint32_t type = ElementType::PUBLIC;
        auto values = parseProfile(
            getDataStore()->getProfile(getId()), type, getId(), getDataStore());
        int32_t ret = handle(getDataStore().get(), getName() + " override action ", values);
        LOGI("WidgetStub action status:%d", ret);
        return ret;
    }
};

class ClassFunc1 {
 public:
    int32_t func(std::vector<int32_t> &value) {
        LOGI("class func %s", parseValue(value).c_str());
        return 0;
    }
};

ExampleChain::ExampleChain() {
    mLogic = std::make_shared<Logic>("./exampleChain.xml");

    auto lambda_func1 = [&](std::vector<int32_t> &value) -> int32_t {
        LOGI("lambda_funcv1 %s", parseValue(value).c_str());
        return 0;
    };

    ClassFunc1 classFunc;

    auto sv = std::make_shared<Widget>("volume", 11221U, funcv1);
    mLogic->addWidget(sv);
    auto sl = std::make_shared<Widget>("loudness", 11225U, funcv2);
    mLogic->addWidget(sl);
    auto sf = std::make_shared<Widget>("fade", 11222U, std::bind(&ClassFunc1::func, classFunc, std::placeholders::_1));
    mLogic->addWidget(sf);
    auto se = std::make_shared<Widget>("equalizer", 11223U, lambda_func1);
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

std::map<uint32_t, int32_t> ExampleChain::getProfile(const uint32_t widgetId,
        std::vector<uint32_t> elementId) {
    return mLogic->getProfile(widgetId, elementId);
}

std::map<uint32_t, int32_t> ExampleChain::getProfile(const std::string widgetName,
        std::vector<std::string> elementName) {
    return mLogic->getProfile(widgetName, elementName);
}

}  // namespace cpfw

static std::string parseValue(std::map<uint32_t, int32_t> v) {
    std::string tmp;
    for (auto s : v) {
        tmp.append(", ");
        tmp.append(std::to_string(s.first) + ":" + std::to_string(s.second));
    }
    return tmp;
}

int main() {
    using namespace cpfw;
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

    LOGI("get stub with name:%s", parseValue(example->getProfile("stub")).c_str());
    LOGI("get stub with id:%s", parseValue(example->getProfile(11226U)).c_str());

    LOGI("get fade with name:%s", parseValue(example->getProfile("fade")).c_str());
    LOGI("get fade with id:%s", parseValue(example->getProfile(11222U)).c_str());

    LOGI("get fade with name:%s", parseValue(example->getProfile("fade", {"default1"})).c_str());
    LOGI("get fade with id:%s", parseValue(example->getProfile(11222U, {2})).c_str());

    LOGI("ExampleChain exit");
    LOGI("end");

    return 0;
}
