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
#include <algorithm>
#include <array>
#include <vector>

namespace cpfw {

/**
 *  no lock in the RingerBuffer,
 *  users should ensure there is no resource competition when call the interface
 */
template<typename T, int32_t N>
class RingBuffer {
 public:
    RingBuffer() : mBuffer() {
    }

    int32_t write(const std::vector<T> &writeBuffer) {
        std::size_t writeSize = writeBuffer.size();
        std::size_t residualSize = N - mTailPos;

        if (residualSize < writeSize) {
            std::ranges::copy_n(
                writeBuffer.begin(), residualSize, mBuffer.begin()+mTailPos);
            std::ranges::copy_n(
                writeBuffer.begin()+residualSize, writeSize-residualSize, mBuffer.begin());
        } else {
            std::ranges::copy_n(writeBuffer.begin(), writeSize, mBuffer.begin()+mTailPos);
        }
        expand(writeSize);
        return 0;
    }

    int32_t read(std::vector<T> &readBuffer, std::size_t readSize) {
        readBuffer.resize(readSize);
        std::size_t residualSize = N - mHeadPos;
        if (residualSize < readSize) {
            std::ranges::copy_n(
                mBuffer.begin()+mHeadPos, residualSize, readBuffer.begin());
            std::ranges::copy_n(
                mBuffer.begin(), readSize-residualSize, readBuffer.begin()+residualSize);
        } else {
            std::ranges::copy_n(mBuffer.begin()+mHeadPos, readSize, readBuffer.begin());
        }
        shrink(readSize);
        return 0;
    }

    int32_t getIdleSize() {
        return N - mAvailableSize;
    }

    int32_t getAvailableSize() {
        return mAvailableSize;
    }

#ifdef TEST
    std::array<T, N>& get() {
        return mBuffer;
    }

    int32_t getHeadPos() {
        return mHeadPos;
    }

    int32_t getTailPos() {
        return mTailPos;
    }
#endif

 private:
    void expand(int32_t size) {
        mTailPos += size;
        if (mTailPos >= N) {
            mTailPos -= N;
        }
        mAvailableSize += size;
        if (mAvailableSize > N) {
            mAvailableSize = N;
        }
    }

    void shrink(int32_t size) {
        mHeadPos += size;
        if (mHeadPos >= N) {
            mHeadPos -= N;
        }
        mAvailableSize -= size;
        if (mAvailableSize < 0) {
            mAvailableSize = 0;
        }
    }

 private:
    std::array<T, N> mBuffer;
    int32_t mHeadPos = 0;
    int32_t mTailPos = 0;
    int32_t mAvailableSize = 0;
};

}  // namespace cpfw

