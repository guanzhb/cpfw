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
#ifndef CPFW_BASE_INCLUDE_PARAMETERS_BASE_H_
#define CPFW_BASE_INCLUDE_PARAMETERS_BASE_H_

#include <functional>
#include <map>
#include <string>

namespace cpfw {

const char DELIM_KEY_VALUES = '=';
const char DELIM_VALUES = ',';

class ParametersBase {
 public:
    ParametersBase();
    virtual ~ParametersBase();

    virtual int32_t setParameters(const std::string &kv);
    virtual int32_t setParameters(const std::string &key,
                                  const std::string &values);

    virtual int32_t getParameters(const std::string &kv, std::string &result);
    virtual int32_t getParameters(const std::string &key,
                                  const std::string &values,
                                  std::string &result);

 protected:
    std::pair<std::string/*key*/, std::string/*value*/>
        parseKv(const std::string &kv);
    uint8_t calculateValueCount(const std::string &values);

 protected:
    using TSetHandle = std::pair<std::vector<uint8_t>/*valid value count*/,
                                 std::function<int32_t(const std::string&/*values*/)>>;
    using TGetHandle = std::pair<std::vector<uint8_t>/*valid value count*/,
                                 std::function<int32_t(const std::string&/*values*/,
                                                       std::string&/*result*/)>>;
    std::map<std::string, TSetHandle> mSetTable;
    std::map<std::string, TGetHandle> mGetTable;
};

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_PARAMETERS_BASE_H_

