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

#include <functional>
#include <memory>

#include "cpfw/base/include/DataStore.hpp"
#include "cpfw/base/include/ResponsibilityChain.hpp"

namespace cpfw {
class ExampleChain {
 public:
    ExampleChain();
    ~ExampleChain();

    int32_t setVolume(int32_t data);

    int32_t setFade(int32_t data);

    int32_t setEq(int32_t band, int32_t db);

    int32_t handleVolume();

    int32_t handleFade();

    int32_t handleEq();

    int32_t handleDuck();

    int32_t setVolumeWidget(int32_t data);

    int32_t setLoudnessWidget(int32_t data);

 private:
    using TINTERFACE = std::function<int32_t(void)>;
    std::shared_ptr<DataStore> mStore;
    std::shared_ptr<ResponsibilityChain> mResponsibilityChain;
};

}  // namespace cpfw

#endif  // EXAMPLES_INCLUDE_EXAMPLECHAIN_H_

