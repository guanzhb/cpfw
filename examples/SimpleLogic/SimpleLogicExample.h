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

#ifndef EXAMPLES_SIMPLELOGIC_SIMPLELOGICEXAMPLE_H_
#define EXAMPLES_SIMPLELOGIC_SIMPLELOGICEXAMPLE_H_

#include "SimpleLogic.h"

namespace cpfw {

class SimpleLogicExample : public SimpleLogic {
 public:
    SimpleLogicExample(const std::string &name,
        std::shared_ptr<SimpleLogicHelper> helper);
};

}  // namespace cpfw

#endif  // EXAMPLES_SIMPLELOGIC_SIMPLELOGICEXAMPLE_H_

