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

#define LOG_TAG "Utils"

#include "Utils.h"

#include <sys/time.h>

#include <chrono>

#include "Base.h"
#include "Log.hpp"
#include "StrategyCalculate.h"

namespace cpfw {

Bundle parseProfile(const Profile &profile, uint32_t type,
        uint32_t widgetId, std::shared_ptr<DataStore> store) {
    Bundle ret;
    if (!store) {
        return ret;
    }
    auto &elements = profile.elements;
    uint32_t i = 0;
    std::for_each(elements.begin(), elements.end(),
        [&] (auto &data) -> void {
            auto &d = data.second;
            if (0 == (type & ElementType::PUBLIC) || (0 == (d.type & ElementType::PUBLIC))) {
                return;
            }
            int32_t current = d.current;
            if (0 != (d.type & ElementType::NEED_CONVERT)) {
                current = store->getConvertedData(widgetId, current);
                auto& converts = store->getConvertTable(widgetId);
                for (auto &c : converts) {
                    current = StrategyCalculatePool::getStrategy(c.expression)
                                  ->handle(widgetId, current, c, store);
                }
            }
            ret.set(std::to_string(i), current);
            ++i;
    });
    return ret;
}

uint64_t getCurrentTimeMs() {
    struct timespec stTimeSpec;
    clock_gettime(CLOCK_MONOTONIC, &stTimeSpec);
    using namespace std::chrono_literals;
    return stTimeSpec.tv_sec * std::chrono::microseconds(1s).count()
           + stTimeSpec.tv_nsec / std::chrono::nanoseconds(1s).count();
}

void dumpBytes(const uint8_t *data, const std::size_t bytes,
               const std::size_t dataInLine, const char *header) {
    constexpr char CODE[] = "0123456789ABCDEF";
    constexpr std::size_t DATA_WIDTH = 3U;
    const std::size_t MAX_DATA = dataInLine * DATA_WIDTH;

    std::string str(MAX_DATA, ' ');

    std::size_t indexInLine = 0U;
    for (std::size_t i=0; i<dataInLine; ++i) {
        str[indexInLine++] = CODE[i / 10];
        str[indexInLine] = CODE[i % 10];
        indexInLine += 2;
    }
    LOGD("<%s>  index : 0d %s", header, str.substr(0, indexInLine).c_str());

    indexInLine = 0U;
    std::size_t line = 0U;

    for (std::size_t i=0; i<bytes; ++i) {
        str[indexInLine++] = CODE[data[i] >> 0x4];
        str[indexInLine] = CODE[data[i] & 0xF];
        indexInLine += 2U;
        if (indexInLine == MAX_DATA) {
            LOGD("<%s> line[%lu]: 0x %s", header, line, str.substr(0, indexInLine).c_str());
            ++line;
            indexInLine = 0U;
        }
    }

    if (0 != indexInLine) {
        LOGD("<%s> line[%lu]: 0x %s", header, line, str.substr(0, indexInLine).c_str());
    }

}

}  // namespace cpfw
