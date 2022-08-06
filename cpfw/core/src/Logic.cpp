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
 * WITHOUT WARRANTIES OR DataParserS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cpfw/core/include/Logic.h"

#include <string>
#include <memory>
#include <iostream>

#include "cpfw/core/include/DataParser.h"

namespace cpfw {

Logic::Logic() {
}

Logic::Logic(std::string configurationFile) {
    mStore = std::make_shared<DataStore>();
    DataParser parser = DataParser(configurationFile, mStore);
    mResponsibilityChain = std::make_shared<ResponsibilityChain>(mStore);
}

Logic::~Logic() {
}

void Logic::addWidget(std::shared_ptr<Widget> widget) {
    mStore->addWidget(widget);
}

int32_t Logic::setProfile(const std::string &widgetName, int32_t value) {
    return setProfile(widgetName, "default", value);
}

int32_t Logic::setProfile(const std::string &widgetName,
        const std::string &elementName, int32_t value) {
    std::cout << "Logic[" << __func__ << "] " << widgetName
        << " -> " << elementName << std::endl;
    mStore->setProfile(widgetName, elementName, value);
    return mResponsibilityChain->invokeChain(widgetName);
}

int32_t Logic::getProfile(const std::string &widgetName) {
    return getProfile(widgetName, "default");
}

int32_t Logic::getProfile(const std::string &widgetName,
            const std::string &elementName) {
    return 0;
}

}  // namespace cpfw

