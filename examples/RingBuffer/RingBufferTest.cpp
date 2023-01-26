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

#define TEST  // used in RingBuffer

#define LOG_TAG "RingBufferTest"

#include <iostream>

#include "Log.hpp"

#include "RingBuffer.hpp"

using namespace cpfw;

constexpr std::size_t N = 12;
using T = char;

void print(std::string str, std::vector<T> buffer) {
    for (auto &s : buffer) {
        str.push_back(s);
        str += " ";
    }
    LOGD("%s", str.c_str());
}

void print(std::string str, cpfw::RingBuffer<T, N> &ringBuffer) {
    auto &data = ringBuffer.get();
    for (std::size_t index=0; index<data.size(); ++index) {
        str.push_back(data[index]);
        str += " ";
    }
    LOGD("%s", str.c_str());
    str = "po: ";
    for (std::size_t index=0; index<data.size(); ++index) {
        if (index == ringBuffer.getHeadPos()) {
            str += "H ";
        } else if (index == ringBuffer.getTailPos()) {
            str += "T ";
        } else {
            str += "  ";
        }
    }
    LOGD("%s", str.c_str());
}

void test(cpfw::RingBuffer<T, N> &ringBuffer,
          std::vector<T> in, std::size_t readSize) {
    print("in: ", in);
    ringBuffer.write(in);
    print("rb: ", ringBuffer);
    std::vector<T> out = std::vector(readSize, '0');
    ringBuffer.read(out, out.size());
    print("ou: ", out);
}

int main() {
    cpfw::RingBuffer<T, N> ringBuffer;

    std::cout << "test:" << std::endl;
    test(ringBuffer, {'1', '2', '3'}, 3);
    test(ringBuffer, {'4', '5', '6', '7'}, 2);
    test(ringBuffer, {'8', '9', 'a', 'b', 'c'}, 5);
    test(ringBuffer, {'d', 'e', 'f'}, 4);
    test(ringBuffer, {'g', 'h', 'i'}, 4);

    return 0;
}

