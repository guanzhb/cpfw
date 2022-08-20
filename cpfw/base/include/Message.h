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

#ifndef CPFW_BASE_INCLUDE_MESSAGE_H__
#define CPFW_BASE_INCLUDE_MESSAGE_H__

#include <functional>
#include <memory>
#include <string>

#include "cpfw/base/include/Bundle.h"

namespace cpfw {

class Message {
 public:
//    Message() {}
//    ~Message() {}

    uint64_t mWhat;
    int32_t mArg1;
    int32_t mArg2;

    std::string mWidgetName;
    std::string mElementName;
    int32_t mValue;

//    std::unique_ptr<Bundle> mBundle;

    std::function<void(int32_t/*status*/)> mCallback;
};

}  // namesapce cpfw

#endif  // CPFW_BASE_INCLUDE_MESSAGE_H__

