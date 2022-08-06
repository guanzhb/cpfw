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

#ifndef EXAMPLES_INCLUDE_EXAMPLECHAIN_H_
#define EXAMPLES_INCLUDE_EXAMPLECHAIN_H_

#include <memory>

#include "cpfw/core/include/Logic.h"

namespace cpfw {
class ExampleChain {
 public:
    ExampleChain();
    ~ExampleChain();

    int32_t setVolume(int32_t volume);

    int32_t setFade(int32_t data);

    int32_t setEq(std::string band, int32_t db);

    int32_t setLoudness(int32_t loudness);

    int32_t setStub(int32_t stub);

 private:
    std::shared_ptr<Logic> mLogic;
};

}  // namespace cpfw

#endif  // EXAMPLES_INCLUDE_EXAMPLECHAIN_H_

