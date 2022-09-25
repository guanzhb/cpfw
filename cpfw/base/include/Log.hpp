
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

#include <initializer_list>
#include <iostream>
#include <iomanip>
#include <mutex>
#include <stdarg.h>

#include "Utils.h"

enum class LOG_LEVEL : uint8_t {
    VERBOSE = 0,
    INFO,
    DEBUG,
    WARN,
    ERROR,
};

#define LOGV(...) cpfw::LogOut(TAG, LOG_LEVEL::VERBOSE, __VA_ARGS__);
#define LOGI(...) cpfw::LogOut(TAG, LOG_LEVEL::INFO, __VA_ARGS__);
#define LOGD(...) cpfw::LogOut(TAG, LOG_LEVEL::DEBUG, __VA_ARGS__);
#define LOGW(...) cpfw::LogOut(TAG, LOG_LEVEL::WARN, __VA_ARGS__);
#define LOGE(...) cpfw::LogOut(TAG, LOG_LEVEL::ERROR, __VA_ARGS__);

namespace cpfw {

template<typename ...Args>
void LogOut(const std::string &tag, LOG_LEVEL level, Args... args) {
    auto print = [] (auto i) { std::cout << i << " ";};
    std::cout << std::left << std::setw(12) << "timestamp:" + std::to_string(getCurrentTimeMs()) + " ";
    std::cout << std::left << std::setw(12) << "[" + tag + "] ";
    std::cout << std::right << std::setw(6);
    switch (level) {
    case LOG_LEVEL::VERBOSE: std::cout << "[VERBOSE] "; break;
    case LOG_LEVEL::INFO: std::cout << "[INFO] "; break;
    case LOG_LEVEL::DEBUG: std::cout << "[DEBUG] "; break;
    case LOG_LEVEL::WARN: std::cout << "[WARN] "; break;
    case LOG_LEVEL::ERROR: std::cout << "[ERROR] "; break;
    default: break;
    };
    std::initializer_list<int>{(print(args), 0)...};
    std::cout << std::endl;
}

}  // namespace cpfw
   //
#endif  // CPFW_BASE_INCLUDE_LOG_HPP__
