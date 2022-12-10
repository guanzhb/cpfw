
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

#ifndef CPFW_BASE_INCLUDE_LOG_HPP__
#define CPFW_BASE_INCLUDE_LOG_HPP__

#ifndef TAG
#define TAG ""
#endif

#include <stdarg.h>
#include <stdio.h>

#include "Utils.h"

namespace cpfw {

#ifdef __others__

#else
    #define LOG_OUT(level, format, ...) do { \
        auto timeMs = getCurrentTimeMs(); \
        printf("[%-12s] [%-s] [%ld] " format"\n", TAG, level, timeMs, ##__VA_ARGS__); \
    } while (0)

    #define LOGV(format, ...) LOG_OUT("V", format, ##__VA_ARGS__);
    #define LOGD(format, ...) LOG_OUT("D", format, ##__VA_ARGS__);
    #define LOGI(format, ...) LOG_OUT("I",  format, ##__VA_ARGS__);
    #define LOGW(format, ...) LOG_OUT("W",  format, ##__VA_ARGS__);
    #define LOGE(format, ...) LOG_OUT("E", format, ##__VA_ARGS__);
    #define LOGF(format, ...) LOG_OUT("F", format, ##__VA_ARGS__);
#endif

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_LOG_HPP__
