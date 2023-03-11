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
#include "DumpUtils.h"

#include "Log.hpp"

namespace cpfw {

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

