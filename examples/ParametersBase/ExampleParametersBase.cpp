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
#define LOG_TAG "ExampleParametersBase"

#include <memory>
#include <string>
#include <vector>

#include "ParametersBase.h"
#include "Log.hpp"

using namespace cpfw;

namespace {

int32_t func1(const std::string &values) {
    LOGI("func1 values:%s", values.c_str());
    return 0;
}

}

class ParametersDerived : public ParametersBase {
 public:
    ParametersDerived() {
        mSetTable["set1"]
            = {{1}, std::bind(&ParametersDerived::set1, this, std::placeholders::_1)};
        mSetTable["set2"]
            = {{2}, std::bind(&ParametersDerived::set2, this, std::placeholders::_1)};
        mSetTable["set1And2"]
            = {{1,2}, std::bind(&ParametersDerived::set1And2, this, std::placeholders::_1)};
        mSetTable["func1"] = {{1}, func1};
        mSetTable["lambda1"] = {{1}, [&](const std::string &values) -> int32_t {
            LOGI("lambda1 values:%s", values.c_str());
            return 0;
        }};
        mGetTable["get1"]
            = {{1}, std::bind(&ParametersDerived::get1,
                              this,
                              std::placeholders::_1,
                              std::placeholders::_2)};
        mGetTable["get2"]
            = {{2}, std::bind(&ParametersDerived::get2,
                              this,
                              std::placeholders::_1,
                              std::placeholders::_2)};
    }

    int32_t set1(const std::string &values) {
        LOGI("set1 values:%s", values.c_str());
        return 0;
    }

    int32_t set2(const std::string &values) {
        LOGI("set2 values:%s", values.c_str());
        return 0;
    }

    int32_t set1And2(const std::string &values) {
        LOGI("set1And2 values:%s", values.c_str());
        return 0;
    }

    int32_t get1(const std::string &values, std::string &result) {
        LOGI("get1 values:%s", values.c_str());
        result = "result1";
        return 0;
    }

    int32_t get2(const std::string &values, std::string &result) {
        LOGI("get1 values:%s", values.c_str());
        result = "result0,result1";
        return 0;
    }
};

int main() {
    ParametersDerived pd;
    pd.setParameters("set1=value0");
    pd.setParameters("set1=value0,value1");
    pd.setParameters("set1=value0,value1,value2");
    pd.setParameters("set2=value0,value1");
    pd.setParameters("func1=value0");
    pd.setParameters("lambda1=value0");
    std::string result;
    pd.getParameters("get1=value0", result);
    LOGI("get1 result:%s", result.c_str());
    result.clear();
    pd.getParameters("get2=value0,value1", result);
    LOGI("get2 result:%s", result.c_str());
    result.clear();
    pd.getParameters("get2=value0,value1,value2", result);
    LOGI("get3 result:%s", result.c_str());

    return 0;
}
