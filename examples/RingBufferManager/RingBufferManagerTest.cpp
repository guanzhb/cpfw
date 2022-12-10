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

#define TAG "RingBufferManagerTest"

#define TEST  // used in RingBufferManager

#include <string>

#include "Log.hpp"
#include "RingBufferManager.hpp"

using namespace cpfw;

constexpr std::size_t N = 12;
using T = char;

cpfw::RingBufferManager<T, N> ringBufferManager {std::string("test")};

void print(std::string str, std::vector<T> buffer) {
    for (auto &s : buffer) {
        str.push_back(s);
        str.append(" ");
    }
    LOGI("%s", str.c_str());
}

void print(std::string str, cpfw::RingBufferManager<T, N> &ringBufferManager) {
    auto &data = ringBufferManager.get().get();
    for (int32_t index=0; index<data.size(); ++index) {
        str.push_back(data[index]);
        str.append(std::string(" "));
    }
    LOGI("%s", str.c_str());
    str = "po: ";
    for (int32_t index=0; index<data.size(); ++index) {
        if (index == ringBufferManager.get().getHeadPos()) {
            str.append("H ");
        } else if (index == ringBufferManager.get().getTailPos()) {
            str.append("T ");
        } else {
            str.append("  ");
        }
    }
    LOGI("%s", str.c_str());
}

void testWrite(cpfw::RingBufferManager<T, N> &ringBufferManager, std::vector<T> in) {
    print("in: ", in);
    ringBufferManager.write(in);
    print("rb: ", ringBufferManager);
}

static int gIndex = 0;

static int32_t readIntra(std::vector<T> &readBuffer, int32_t readSize) {
    print("ou: ", readBuffer);
    return 0;
}

static int32_t writeIntra() {
    switch (gIndex) {
    case 0:
        testWrite(ringBufferManager, {'1', '2', '3'});
        break;
    case 1:
        testWrite(ringBufferManager, {'4', '5', '6', '7'});
        break;
    case 2:
        testWrite(ringBufferManager, {'8', '9', 'a', 'b', 'c'});
        break;
    case 3:
        testWrite(ringBufferManager, {'d', 'e', 'f'});
        break;
    case 4:
        testWrite(ringBufferManager, {'g', 'h', 'i'});
        break;
    default:
        break;
    }
    ++gIndex;
    return 0;
}

int main() {
    LOGD("test:");
    ringBufferManager.registerWrite(writeIntra, 20);
    // ringBufferManager.registerRead(readIntra, 10, 500);  // overrun
    ringBufferManager.registerRead(readIntra, 1, 50); // underrun

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    return 0;
}

