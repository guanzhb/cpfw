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

#include "RingArray.hpp"
#include "Log.hpp"

void test(cpfw::RingArray<int32_t, 5> &ringArray, int32_t index) {
    std::cout << "index:" + std::to_string(index)
                 + " -> " + std::to_string(ringArray[index]) << std::endl;
}

void print(cpfw::RingArray<int32_t, 5> &RingArray) {
    for (int32_t index=0; index<RingArray.size(); ++index) {
        std::cout << "index:" + std::to_string(index)
                 + " -> " + std::to_string(RingArray.at(index)) << std::endl;
    }
}

int main() {
    cpfw::RingArray<int32_t, 5> ringArray;
    int i = 0;
    for (auto &d : ringArray) {
        d = i++;
    }

    std::cout << "table:" << std::endl;
    print(ringArray);

    std::cout << "test:" << std::endl;
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
