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
#define LOG_TAG "ExampleTLV"

#include <memory>
#include <string>
#include <vector>

#include "TLV.hpp"
#include "Log.hpp"

using namespace cpfw;

typedef struct {
    int32_t cmd;
    int32_t type;
} TTag;

typedef struct {
    int32_t param1;
    int32_t param2;
    int32_t param3;
} TValue;

typedef struct {
    int32_t param1;
    int32_t param2;
    int32_t param3;
    int32_t param4;
} TValue1;

int main() {
    TTag tag = {1, 2};
    TValue value = {3, 4, 5};
    TLV<TTag, uint8_t> tlv(&tag, (uint8_t)sizeof(value), (uint8_t*)&value);

    TTag *t = tlv.getTag();

    LOGI("test header valueSize and value")
    LOGI("tlv before setValue size:%ld", tlv.getTotalSize());
    LOGI("tlv ref tag:cmd:%d", t->cmd);
    LOGI("tlv ref tag:type:%d", t->type);

    TValue *ret = tlv.getValue<TValue>();
    LOGI("tlv ref value:param1:%d", ret->param1);
    LOGI("tlv ref value:param2:%d", ret->param2);
    LOGI("tlv ref value:param3:%d", ret->param3);

    ret->param3 = 678;

    TValue *retValue = tlv.getValue<TValue>();
    LOGI("tlv value:param1:%d", retValue->param1);
    LOGI("tlv value:param2:%d", retValue->param2);
    LOGI("tlv value:param3:%d", retValue->param3);

    LOGI("tlv value:param1:%d", ret->param1);
    LOGI("tlv value:param2:%d", ret->param2);
    LOGI("tlv value:param3:%d", ret->param3);

    retValue->param1 = 234;
    LOGI("tlv ref value:param1:%d", ret->param1);
    LOGI("tlv ref value:param2:%d", ret->param2);
    LOGI("tlv ref value:param3:%d", ret->param3);

    LOGI("\ntest header")
    TLV<TTag, uint8_t> tlv1(&tag);
    t = tlv1.getTag();
    t->cmd = 123;

    LOGI("tlv1 before setValue");
    LOGI("tlv1 ref tag:cmd:%d", t->cmd);
    LOGI("tlv1 ref tag:type:%d", t->type);
    TValue1 *retValue1 = tlv1.getValue<TValue1>();
    if (retValue1 != nullptr) {
        LOGI("tlv1 ref value:param1:%d", retValue1->param1);
        LOGI("tlv1 ref value:param2:%d", retValue1->param2);
        LOGI("tlv1 ref value:param3:%d", retValue1->param3);
        LOGI("tlv1 ref value:param4:%d", retValue1->param4);
    } else {
        LOGI("tlv1 ref value size fault");
    }

    TValue1 value1 = {31, 41, 51, 61};
    LOGI("tlv1 before setValue total:%ld", tlv1.getTotalSize());
    tlv1.setValue(&value1, sizeof(value1));
    LOGI("tlv1 after setValue total:%ld", tlv1.getTotalSize());
    t = tlv1.getTag();
    retValue1 = tlv1.getValue<TValue1>();
    LOGI("tlv1 ref tag:cmd:%d", t->cmd);
    LOGI("tlv1 ref tag:type:%d", t->type);
    if (retValue1 != nullptr) {
        LOGI("tlv1 ref value:param1:%d", retValue1->param1);
        LOGI("tlv1 ref value:param2:%d", retValue1->param2);
        LOGI("tlv1 ref value:param3:%d", retValue1->param3);
        LOGI("tlv1 ref value:param4:%d", retValue1->param4);
    } else {
        LOGI("tlv1 ref value size fault");
    }

    LOGI("\ntest serialize to and from constract")
    std::vector<uint8_t> buf = requestBuffer(tlv1.getTotalSize());
    tlv1.serializeTo(&buf[0], buf.capacity());
    TLV<TTag, uint8_t> tlv2(reinterpret_cast<uint8_t*>(&buf[0]), buf.capacity());
    LOGI("tlv2 unserialize from tlv1 size:%ld", tlv2.getTotalSize());
    t = tlv2.getTag();
    LOGI("tlv2 ref tag:cmd:%d", t->cmd);
    LOGI("tlv2 ref tag:type:%d", t->type);
    LOGI("tlv2 ref tag:cmd:%d", t->cmd);
    LOGI("tlv2 ref tag:type:%d", t->type);
    retValue1 = tlv2.getValue<TValue1>();
    if (retValue1 != nullptr) {
        LOGI("tlv2 ref value:param1:%d", retValue1->param1);
        LOGI("tlv2 ref value:param2:%d", retValue1->param2);
        LOGI("tlv2 ref value:param3:%d", retValue1->param3);
        LOGI("tlv2 ref value:param4:%d", retValue1->param4);
    } else {
        LOGI("tlv2 ref value size fault");
    }

    LOGI("\ntest serialize to and from func")
    TLV<TTag, uint8_t> tlv3(tlv2.getTotalSize());
    tlv3.unserializeFrom(reinterpret_cast<uint8_t*>(&buf[0]), buf.size());
    LOGI("tlv3 ref tag:cmd:%d", t->cmd);
    LOGI("tlv3 ref tag:type:%d", t->type);
    TValue1 *retValue2 = tlv2.getValue<TValue1>();
    if (retValue2 != nullptr) {
        LOGI("tlv3 ref value:param1:%d", retValue2->param1);
        LOGI("tlv3 ref value:param2:%d", retValue2->param2);
        LOGI("tlv3 ref value:param3:%d", retValue2->param3);
        LOGI("tlv3 ref value:param4:%d", retValue2->param4);
    } else {
        LOGI("tlv3 ref value size fault");
    }

    return 0;
}
