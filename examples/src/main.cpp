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
