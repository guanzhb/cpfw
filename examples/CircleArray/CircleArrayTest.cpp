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

#define TAG "CircleArrayTest"

#include "CircleArray.hpp"
#include "Log.hpp"

void test(cpfw::CircleArray<int32_t, 5> circleArray, int index) {
    std::cout << "index:" + std::to_string(index)
                 + " -> " + std::to_string(circleArray[index]) << std::endl;
}

void print(cpfw::CircleArray<int32_t, 5> circleArray) {
    auto &data = circleArray.data();
    for (int32_t index=0; index<data.size(); ++index) {
        std::cout << "index:" + std::to_string(index)
                 + " -> " + std::to_string(circleArray[index]) << std::endl;
    }
}

int main() {
    cpfw::CircleArray<int32_t, 5> circleArray;
    auto &data = circleArray.data();
    int i = 0;
    for (auto &d : data) {
        d = i++;
    }

    std::cout << "table:" << std::endl;
    print(circleArray);

    std::cout << "test:" << std::endl;
    test(circleArray, -10);
    test(circleArray, -5);
    test(circleArray, -4);
    test(circleArray, -1);
    test(circleArray, 0);
    test(circleArray, 4);
    test(circleArray, 5);
    test(circleArray, 9);

    return 0;
}
