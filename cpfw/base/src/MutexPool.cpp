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

#include "MutexPool.h"

#include <bit>
#include <tuple>

#include "Log.hpp"

namespace cpfw {

void MutexPool::bindMutex(uint32_t id, uint8_t mutexId) {
    mIdTable.emplace(id, mutexId);
    mMutexTable.emplace(std::piecewise_construct, std::make_tuple(mutexId), std::make_tuple());
}

std::shared_mutex& MutexPool::getMutex(uint32_t id) {
    uint8_t mutexId = 0;
    if (auto idItor = mIdTable.find(id); idItor != mIdTable.end()) {
        mutexId = idItor->second;
    } else {
        mutexId = std::popcount(id);
    }
    if (auto mutexItor = mMutexTable.find(mutexId); mutexItor == mMutexTable.end()) {
        mMutexTable.emplace(std::piecewise_construct, std::make_tuple(mutexId), std::make_tuple());
    }
    return mMutexTable[mutexId];
}

}  // namespace cpfw

