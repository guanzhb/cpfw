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

#ifndef CPFW_BASE_INCLUDE_DATASTORE_H_
#define CPFW_BASE_INCLUDE_DATASTORE_H_

#include <map>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <optional>
#include <utility>

#include "cpfw/base/include/Base.h"
#include "cpfw/base/include/Widget.h"
#include "cpfw/base/include/Condition.h"
#include "cpfw/base/include/ExpressionPool.h"

namespace cpfw {

class Widget;

using TINVOKE_CHAIN = std::vector<std::string/*child name*/>;
using TINVOKE_CONDITION = std::pair<ExpressionEnum/*and, or*/, std::vector<Condition>>;

/**
 * @brief database of framework, who is configuried by file or user.
 * no logic here, only data.
 */
class DataStore : public std::enable_shared_from_this<DataStore>{
 public:
    static const TINVOKE_CHAIN EMPTY_INVOKE_CHAIN;
    static const TINVOKE_CONDITION EMPTY_CONDITION;
    static Profile EMPTY_PROFILE;
    DataStore();

    ~DataStore();

    std::optional<std::shared_ptr<Widget>> getWidget(const std::string &name);

    const TINVOKE_CHAIN& getChain(const std::string &parentName);

    Profile& getProfile(const std::string &name);
    void setProfile(const std::string &profileName, int32_t value);
    void setProfile(const std::string &profileName,
            const std::string &elementName, int32_t value);

    int32_t getConvertedData(std::string context, int32_t origin);

    const TINVOKE_CONDITION& getCondition(const std::string widgetName);

 public:
    /**
     * @brief bind every widget to it's unique name.
     * invoke in constructor.
     *
     * @param widget the widget who can be invoked.
     */
    void addWidget(std::shared_ptr<Widget> widget);

    /**
     * @brief add invoke chain.
     * every parent can have multiple children.
     * such as: when set volume, we can then set effect and deal duck logic.
     *
     * @param parent parent widget name
     * @param children children widget name follow the parent
     */
    void addInvokeChain(std::string parent, const TINVOKE_CHAIN children);

    /**
     * @brief add profile which binds to it's unique wifget
     *
     * @param name widget name
     * @param profile data binds to the widget
     */
    void addProfile(const std::string name, const Profile profile);

    /**
     * @brief add data table to convert.
     * such as: we can convert msgId 0x1111 to msgId 0x2222.
     *
     * @param contxt data context
     * @param origin original data
     * @param target converted data
     */
    void addDataConvert(
            const std::string context, int32_t origin, int32_t target);

    /**
     * @brief add condition data.
     *
     * @param widgetName
     * @param condition check data and expression(or, and)
     */
    void addCondition(const std::string widgetName, TINVOKE_CONDITION condition);

 private:

    // every widget binds to a unique name
    std::map<std::string/*unique name*/, std::shared_ptr<Widget>> mWidgetTable;
    // every invoker can invoke multiple invokers
    std::map<std::string/*parent name*/, TINVOKE_CHAIN> mInvokeChainTable;
    // every widget binds to a profile, which have multi elements
    std::map<std::string/*widget name*/, Profile> mProfileTable;
    // original data can be converted to the target data for different context
    std::map<std::string/*context*/, std::map<int32_t, int32_t>> mDataConvertTable;
    // used to check before action
    std::map<std::string/*condition name*/, TINVOKE_CONDITION> mConditionTable;
};

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_DATASTORE_HPP_

