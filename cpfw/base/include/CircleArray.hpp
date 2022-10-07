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

#include <array>
#include <stdint.h>

namespace cpfw {

/**
 *  only impl operator[], keep others same as array. 
 */
template<typename T, int32_t N>
class CircleArray {
 public:
    T& operator[](int32_t index) {
        index %= N;
        if (index < 0) {
            index += N;
        }
        return mArray[index];
    }

    std::array<T, N>& data() {
        return mArray;
    }

 private:
    std::array<T, N> mArray;
};

}  // namespace cpfw

