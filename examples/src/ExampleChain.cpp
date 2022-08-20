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
#include "examples/include/ExampleChain.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "cpfw/base/include/Base.h"
#include "cpfw/base/include/Utils.hpp"

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

class WidgetStub : public Widget {
 public:
    WidgetStub(std::string name) : Widget(name) {
    }

    int32_t action() override {
        uint32_t type = static_cast<uint32_t>(ElementType::PUBLIC);
        auto values = parseProfile(
            getDataStore()->getProfile(getName()), type, getName(), getDataStore());
        return handle(getDataStore().get(), getName() + " override action ", values);
    }
};

ExampleChain::ExampleChain() {
    mLogic = std::make_shared<Logic>("./configs/exampleChain.xml");

    auto sv = std::make_shared<WidgetStub>("volume");
    mLogic->addWidget(sv);
    auto sl = std::make_shared<WidgetStub>("loudness");
    mLogic->addWidget(sl);
    auto sf = std::make_shared<WidgetStub>("fade");
    mLogic->addWidget(sf);
    auto se = std::make_shared<WidgetStub>("equalizer");
    mLogic->addWidget(se);
    auto sd = std::make_shared<WidgetStub>("duck");
    mLogic->addWidget(sd);
    auto ss = std::make_shared<WidgetStub>("stub");
    mLogic->addWidget(ss);
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
    return mLogic->setProfile("stub", stub);
}

}  // namespace cpfw

int main() {
    std::shared_ptr<cpfw::ExampleChain> example = std::make_shared<cpfw::ExampleChain>();
    std::cout << "start " << std::endl;

    std::cout << std::endl;
    example->setVolume(30);
    std::cout << "volume success" << std::endl;

    std::cout << std::endl;
    example->setFade(50);
    std::cout << "fade success" << std::endl;

    std::cout << std::endl;
    example->setEq("gain_100hz", 100);
    std::cout << "eq success" << std::endl;

    std::cout << std::endl;
    example->setLoudness(120);
    std::cout << "loudness success" << std::endl;

    std::cout << std::endl;
    example->setVolume(300);
    std::cout << "volume success" << std::endl;

    std::cout << std::endl;
    example->setEq("gain_100hz", 100);
    std::cout << "eq success" << std::endl;

    std::cout << std::endl;
    example->setLoudness(10);
    std::cout << "loudness success" << std::endl;

    std::cout << std::endl;
    example->setStub(10);
    std::cout << "stub success" << std::endl;

    return 0;
}
