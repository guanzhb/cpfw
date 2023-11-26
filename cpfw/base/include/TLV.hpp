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

#ifndef CPFW_BASE_INCLUDE_TLV_HPP_
#define CPFW_BASE_INCLUDE_TLV_HPP_

#include <cstring>
#include <vector>

namespace cpfw {

static std::vector<uint8_t> requestBuffer(const size_t totalSize) {
    return std::move(std::vector<uint8_t>(totalSize, 0));
}

/**
 * @brief TLV define.
 * no lock in, need user to deal competitive logic.
 */
template<typename TTAG, typename TVALUESIZE>
struct TLV {
 public:
    explicit TLV(const TVALUESIZE valueSize = 0) : TLV(NULL, valueSize) {
    }

    explicit TLV(const TTAG tag = {}, const TVALUESIZE valueSize = 0, const uint8_t *value = NULL)
            : TLV(&tag, valueSize, value) {
    }

    explicit TLV(const TTAG *tag = NULL, const TVALUESIZE valueSize = 0, const uint8_t * const value = NULL)
            : mBuffer(sizeof(*tag) + sizeof(valueSize) + valueSize, 0) {
        mBuffer.assign(sizeof(*tag) + sizeof(valueSize), 0);
        setTag(tag);
        setValueSize(valueSize);
        setValue(reinterpret_cast<const uint8_t *>(value), valueSize);
    }

    explicit TLV(const uint8_t * const buf, const size_t totalSize) {
        unserializeFrom(buf, totalSize);
    }

    TTAG* getTag() {
         return reinterpret_cast<TTAG*>(&mBuffer[0]);
    }

    int32_t setTag(const TTAG *tag) {
        if (tag != NULL) {
            *reinterpret_cast<TTAG *>(&mBuffer[0]) = *tag;
        }
        return 0;
    }

    TVALUESIZE getValueSize() {
        return *reinterpret_cast<TVALUESIZE *>(&mBuffer[sizeof(TTAG)]);
    }

    int32_t setValueSize(TVALUESIZE valueSize) {
        *reinterpret_cast<TVALUESIZE *>(&mBuffer[sizeof(TTAG)]) = valueSize;
        return 0;
    }

    template<typename TVALUE>
    TVALUE* getValue() {
        if (sizeof(TVALUE) > getValueSize()) {
            return nullptr;
        }
        return reinterpret_cast<TVALUE*>(&mBuffer[sizeof(TTAG) + sizeof(TVALUESIZE)]);
    }

    int32_t setValue(const uint8_t *value, const TVALUESIZE valueSize) {
        if (value != NULL) {
            mBuffer.erase(mBuffer.begin() + sizeof(TTAG) + sizeof(TVALUESIZE), mBuffer.end());
            mBuffer.insert(mBuffer.begin() + sizeof(TTAG) + sizeof(TVALUESIZE), value, value + valueSize);
            setValueSize(valueSize);
        }
        mBuffer.shrink_to_fit();
        return 0;
    }

    template<typename TVALUE>
    int32_t setValue(TVALUE *value, TVALUESIZE valueSize) {
        if (valueSize != getValueSize()) {
            setValue(reinterpret_cast<const uint8_t *>(value), valueSize);
        } else {
            memcpy((uint8_t *)getValue<TVALUE>(), value, valueSize);
        }
        return 0;
    }

    int32_t unserializeFrom(const uint8_t * const buf, size_t totalSize) {
        mBuffer.assign(buf, buf + totalSize);
        return 0;
    }

    int32_t serializeTo(uint8_t *buf, size_t totalSize) {
        memcpy(buf, &mBuffer[0], totalSize);
        return 0;
    }

    size_t getTotalSize() {
        return mBuffer.size();
    }

 private:
    std::vector<uint8_t> mBuffer;
};

} // namespace cpfw

#endif // CPFW_BASE_INCLUDE_TLV_HPP_

