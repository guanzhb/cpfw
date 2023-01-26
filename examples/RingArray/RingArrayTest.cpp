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

#define LOG_TAG "RingArrayTest"

#include "RingArray.hpp"
#include "Log.hpp"

using namespace cpfw;

void test(cpfw::RingArray<int32_t, 5> &ringArray, int32_t index) {
    LOGD("index:%d value:%d", index, ringArray[index]);
}

void print(cpfw::RingArray<int32_t, 5> &ringArray) {
    for (int32_t index=0; index<ringArray.size(); ++index) {
        LOGD("index:%d value:%d", index, ringArray.at(index));
    }
}

int main() {
    cpfw::RingArray<int32_t, 5> ringArray;
    int i = 0;
    for (auto &d : ringArray) {
        d = i++;
    }

    LOGD("table");
    print(ringArray);

    LOGD("test");
    test(ringArray, -10);
    test(ringArray, -5);
    test(ringArray, -4);
    test(ringArray, -1);
    test(ringArray, 0);
    test(ringArray, 4);
    test(ringArray, 5);
    test(ringArray, 9);

    return 0;
}
