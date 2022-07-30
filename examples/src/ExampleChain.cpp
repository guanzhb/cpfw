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

static int32_t handle(
        DataStore *store, const std::string &funcName, const std::vector<int32_t> &values) {
    std::cout << funcName;
    std::for_each(values.begin(), values.end(), [](auto d) -> void {
        std::cout << " " << d;
    });
    std::cout << std::endl;
    return 0;
}

class StubWidget : public Widget {
 public:
    StubWidget(std::string name, std::shared_ptr<DataStore> store)
        : Widget(name, store) {
    }

    int32_t action() override {
        uint32_t type = static_cast<uint32_t>(ElementType::PUBLIC);
        auto values = parseProfile(
            getDataStore()->getProfile(getName()), type, getName(), getDataStore());
        return handle(getDataStore().get(), getName() + " override action ", values);
    }
};

ExampleChain::ExampleChain() {
    mStore = std::make_shared<DataStore>();

    auto sv = std::make_shared<Widget>("volume", mStore);
    mStore->addWidget("volume", sv);
    auto sl = std::make_shared<Widget>("loudness", mStore);
    mStore->addWidget("loudness", sl);
    auto sf = std::make_shared<Widget>("fade", mStore);
    mStore->addWidget("fade", sf);
    auto se = std::make_shared<Widget>("equalizer", mStore);
    mStore->addWidget("equalizer", se);
    auto sd = std::make_shared<Widget>("duck", mStore);
    mStore->addWidget("duck", sd);

    auto ss = std::make_shared<StubWidget>("stub", mStore);
    mStore->addWidget("stub", ss);

    mStore->addInvokeChain("volume", {"equalizer", "duck"});
    mStore->addInvokeChain("equalizer", {"fade", "stub"});
    mStore->addInvokeChain("loudness", {"volume"});

    mStore->addDataConvert("loudness", 22, 99999);

    Element ele;
    ele.current = 3;
    ele.type = static_cast<uint32_t>(ElementType::PUBLIC);
    Profile vol;
    vol.elements.emplace("default", ele);
    mStore->addProfile("volume", vol);

    Profile fade;
    ele.current = 4;
    fade.elements.emplace("default", ele);
    mStore->addProfile("fade", fade);

    Profile equalizer;
    ele.current = 5;
    equalizer.elements.emplace("gain_100hz", ele);
    ele.current = 30;
    equalizer.elements.emplace("gain_200hz", ele);
    mStore->addProfile("equalizer", equalizer);

    Profile duck;
    ele.current = 323;
    duck.elements.emplace("default", ele);
    mStore->addProfile("duck", duck);

    ele.type = static_cast<uint32_t>(ElementType::PUBLIC)
        | static_cast<uint32_t>(ElementType::NEED_CONVERT);

    Profile lw;
    ele.current = 22;
    lw.elements.emplace("default", ele);
    mStore->addProfile("loudness", lw);

    Profile stub;
    ele.current = 555;
    stub.elements.emplace("default", ele);
    mStore->addProfile("stub", stub);

    Condition condition1 = Condition("duck", "in_range", "volume", "default", 0, 40);
    Condition condition2 = Condition("duck", "out_range", "equalizer", "gain_100hz", 0, 40);
    mStore->addCondition("duck", {{"and", condition1}, {"or", condition2}});

    mResponsibilityChain = std::make_shared<ResponsibilityChain>(mStore);
}

ExampleChain::~ExampleChain() {
}

int32_t ExampleChain::setVolume(int32_t volume) {
    mStore->setProfile("volume", volume);
    return mResponsibilityChain->invokeChain("volume");
}

int32_t ExampleChain::setFade(int32_t fade) {
    mStore->setProfile("fade", fade);
    return mResponsibilityChain->invokeChain("fade");
}

int32_t ExampleChain::setEq(std::string band, int32_t db) {
    mStore->setProfile("equalizer", band, db);
    return mResponsibilityChain->invokeChain("equalizer");
}

int32_t ExampleChain::setLoudness(int32_t loudness) {
    mStore->setProfile("loudness", loudness);
    return mResponsibilityChain->invokeChain("loudness");
}

int32_t ExampleChain::setStub(int32_t stub) {
    mStore->setProfile("stub", stub);
    return mResponsibilityChain->invokeChain("stub");
}

}  // namespace cpfw

