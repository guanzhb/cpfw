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

#include <memory>
#include <iostream>

int main() {
    std::shared_ptr<cpfw::ExampleChain> example = std::make_shared<cpfw::ExampleChain>();
    std::cout << "start " << std::endl;

    example->setVolume(30);

    std::cout << std::endl;
    example->setFade(50);

    std::cout << std::endl;
    example->setEq(90, 100);

    std::cout << std::endl;
    example->setVolumeWidget(90);

    std::cout << std::endl;
    example->setLoudnessWidget(120);

    return 0;
}
