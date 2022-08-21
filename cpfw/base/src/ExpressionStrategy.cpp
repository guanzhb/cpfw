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

#include "cpfw/base/include/ExpressionStrategy.h"

#include <iostream>

namespace cpfw {

int32_t ExpressionStrategyEqual::handle(
            const Condition &condition, std::shared_ptr<DataStore> dataStore) {
    std::cout << "ExpressionStrategyEqual handle" << std::endl;
    Profile &profile = dataStore->getProfile(condition.getProfileName());
    int32_t current = profile.elements[condition.getElementName()].current;
    return current == condition.getDefault() ? 0 : EINVAL;
}

int32_t ExpressionStrategyNotEqual::handle(
            const Condition &condition, std::shared_ptr<DataStore> dataStore) {
    std::cout << "ExpressionStrategyNotEqual handle" << std::endl;
    Profile &profile = dataStore->getProfile(condition.getProfileName());
    int32_t current = profile.elements[condition.getElementName()].current;
    return current != condition.getDefault() ? 0 : EINVAL;
}

int32_t ExpressionStrategyInRange::handle(
            const Condition &condition, std::shared_ptr<DataStore> dataStore) {
    std::cout << "ExpressionStrategyInRange handle" << std::endl;
    Profile &profile = dataStore->getProfile(condition.getProfileName());
    int32_t current = profile.elements[condition.getElementName()].current;
    return (current >= condition.getLeft() && current <= condition.getRight())
        ? 0 : EINVAL;
}

int32_t ExpressionStrategyOutRange::handle(
            const Condition &condition, std::shared_ptr<DataStore> dataStore) {
    std::cout << "ExpressionStrategyOutRange handle" << std::endl;
    Profile &profile = dataStore->getProfile(condition.getProfileName());
    int32_t current = profile.elements[condition.getElementName()].current;
    return (current < condition.getLeft() && current > condition.getRight())
        ? 0 : EINVAL;
}

int32_t ExpressionStrategyChange::handle(
            const Condition &condition, std::shared_ptr<DataStore> dataStore) {
    std::cout << "ExpressionStrategyChange handle" << std::endl;
    Profile &profile = dataStore->getProfile(condition.getProfileName());
    return (profile.elements[condition.getElementName()].flag) ? 0 : EINVAL;
}

}  // namespace cpfw

