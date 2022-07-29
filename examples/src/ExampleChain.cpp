/**
 * Copyright (C) 2020 The Cross Platform Framework Project
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
#include "examples/include/ExampleChain.h"

#include <iostream>
#include <string>
#include <vector>

#include "cpfw/base/include/Utils.hpp"
#include "cpfw/base/include/Base.h"

namespace cpfw {

static int32_t handle(const std::string &funcName, const std::vector<int32_t> &values) {
    std::cout << funcName;
    std::for_each(values.begin(), values.end(), [](auto d) -> void {
        std::cout << " " << d;
    });
    std::cout << std::endl;
    return 0;
}

class VolumeWidget : public WidgetBase {
 public:
    VolumeWidget(std::shared_ptr<DataStore> store, const std::string &name)
        : WidgetBase(store, name) {
    }

    ~VolumeWidget() {}

    int32_t check() override {
        std::cout << getName() << " " << __func__ << std::endl;
        return 0;
    }

    int32_t action() override {
        uint32_t type = static_cast<uint32_t>(ElementType::PUBLIC);
        auto values = parseProfile(
            getDataStore()->getProfile(getName()), type, "volume", getDataStore());
        return handle(getName() + " action ", values);
    }

    int32_t swipe() override {
        std::cout << getName() << " " << __func__ << std::endl;
        return 0;
    }
};

class LoudnessWidget : public WidgetBase {
 public:
    LoudnessWidget(std::shared_ptr<DataStore> store, std::string name)
        : WidgetBase(store, name) {
    }

    ~LoudnessWidget() {}

    int32_t check() override {
        std::cout << getName() << " " << __func__ << std::endl;
        return 0;
    }

    int32_t action() override {
        uint32_t type = static_cast<uint32_t>(ElementType::PUBLIC)
                            | static_cast<uint32_t>(ElementType::NEED_CONVERT);
        auto values = parseProfile(
            getDataStore()->getProfile(getName()), type, "loudnessWidget", getDataStore());
        return handle(getName() + " action ", values);
    }

    int32_t swipe() override {
        std::cout << getName() << " " << __func__ << std::endl;
        return 0;
    }
};

ExampleChain::ExampleChain() {
    mStore = std::make_shared<DataStore>();

    mStore->addInterface("handleVolume", std::bind(&ExampleChain::handleVolume, this));
    mStore->addInterface("handleFade", std::bind(&ExampleChain::handleFade, this));
    mStore->addInterface("handleEq", std::bind(&ExampleChain::handleEq, this));
    mStore->addInterface("handleDuck", std::bind(&ExampleChain::handleDuck, this));

    mStore->addWidget("volumeWidget", std::make_shared<VolumeWidget>(mStore, "volumeWidget"));
    mStore->addWidget("loudnessWidget", std::make_shared<LoudnessWidget>(mStore, "loudnessWidget"));

    mStore->initializeInvokeChain("handleVolume", {"handleEq", "handleDuck"});
    mStore->initializeInvokeChain("handleEq", {"handleFade"});
    mStore->initializeInvokeChain("volumeWidget", {"handleVolume"});
    mStore->initializeInvokeChain("loudnessWidget", {"volumeWidget"});

    mStore->initializeDataConvertTable("loudnessWidget", 22, 99999);

    Element ele;
    ele.current = 3;
    ele.type = static_cast<uint32_t>(ElementType::PUBLIC);
    Profile vol;
    vol.elements.emplace("default", ele);
    mStore->initializeProfile("handleVolume", vol);

    Profile eq;
    ele.current = 4;
    eq.elements.emplace("default", ele);
    mStore->initializeProfile("handleFade", eq);

    Profile fade;
    ele.current = 5;
    fade.elements.emplace("band", ele);
    ele.current = 6;
    fade.elements.emplace("db", ele);
    mStore->initializeProfile("handleEq", fade);

    Profile duck;
    ele.current = 323;
    duck.elements.emplace("default", ele);
    mStore->initializeProfile("handleDuck", duck);

    ele.current = 2121;
    Profile vw;
    vw.elements.emplace("default", ele);
    ele.current = 32;
    vw.elements.emplace("a", ele);
    ele.current = 21;
    vw.elements.emplace("d", ele);
    ele.current = 4444;
    vw.elements.emplace("de", ele);
    ele.current = 555;
    vw.elements.emplace("dult", ele);
    mStore->initializeProfile("volumeWidget", vw);

    ele.current = 2121;
    ele.type = static_cast<uint32_t>(ElementType::PUBLIC)
        | static_cast<uint32_t>(ElementType::NEED_CONVERT);
    Profile lw;
    lw.elements.emplace("default", ele);
    ele.current = 22;
    lw.elements.emplace("a", ele);
    ele.current = 33;
    lw.elements.emplace("d", ele);
    ele.current = 22;
    lw.elements.emplace("de", ele);
    ele.current = 555;
    lw.elements.emplace("dult", ele);
    mStore->initializeProfile("loudnessWidget", lw);

    mResponsibilityChain = std::make_shared<ResponsibilityChain>(mStore);
}

ExampleChain::~ExampleChain() {
}

int32_t ExampleChain::setVolume(int32_t data) {
    auto volume = mStore->getProfile("handleVolume");
    volume->elements["default"].current  = data;

    return mResponsibilityChain->invokeChain("handleVolume");
}

int32_t ExampleChain::setFade(int32_t data) {
    auto effect = mStore->getProfile("handleFade");
    effect->elements["default"].current = data;

    return mResponsibilityChain->invokeChain("handleFade");
}

int32_t ExampleChain::setEq(int32_t band, int32_t db) {
    auto eq = mStore->getProfile("handleEq");
    eq->elements["band"].current  = band;
    eq->elements["db"].current  = db;

    return mResponsibilityChain->invokeChain("handleEq");
}

int32_t ExampleChain::setVolumeWidget(int32_t data) {
    auto volume = mStore->getProfile("volumeWidget");
    volume->elements["default"].current  = data;

    return mResponsibilityChain->invokeChain("volumeWidget");
}

int32_t ExampleChain::setLoudnessWidget(int32_t data) {
    auto volume = mStore->getProfile("loudnessWidget");
    volume->elements["default"].current  = data;

    return mResponsibilityChain->invokeChain("loudnessWidget");
}

int32_t ExampleChain::handleVolume() {
    uint32_t type = static_cast<uint32_t>(ElementType::PUBLIC);
    auto values = parseProfile(
        mStore->getProfile("handleVolume"), type, "volume", mStore);
    return handle(__func__, values);
}

int32_t ExampleChain::handleFade() {
    uint32_t type = static_cast<uint32_t>(ElementType::PUBLIC);
    auto values = parseProfile(
        mStore->getProfile("handleFade"), type, "handleFade", mStore);
    return handle(__func__, values);
}

int32_t ExampleChain::handleEq() {
    uint32_t type = static_cast<uint32_t>(ElementType::PUBLIC);
    auto values = parseProfile(
        mStore->getProfile("handleEq"), type, "handleEq", mStore);
    return handle(__func__, values);
}

int32_t ExampleChain::handleDuck() {
    uint32_t type = static_cast<uint32_t>(ElementType::PUBLIC);
    auto values = parseProfile(
        mStore->getProfile("handleDuck"), type, "handleDuck", mStore);
    return handle(__func__, values);
}

}  // namespace cpfw

