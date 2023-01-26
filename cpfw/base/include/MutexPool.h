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
#ifndef CPFW_BASE_INCLUDE_MUTEXPOOL_H_
#define CPFW_BASE_INCLUDE_MUTEXPOOL_H_

#include <condition_variable>
#include <map>
#include <shared_mutex>

namespace cpfw {

/**
 * lock pool to avoid lock action between different ids.
 */
class MutexPool {
 public:
     /*
      * assign lock for id
      */
     void bindMutex(uint32_t id, uint8_t mutexId);

     /*
      * get mutex instance with id.
      *   1. try to get binded mutex.
      *   2. get mutex with popcount count of id.
      */
     std::shared_mutex& getMutex(uint32_t id);

 private:
    std::map<uint32_t, uint8_t> mIdTable;
    std::map<uint8_t, std::shared_mutex> mMutexTable;
};  // MutexPool

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_MUTEXPOOL_H_

