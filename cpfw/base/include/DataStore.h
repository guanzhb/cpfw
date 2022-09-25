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

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <utility>

#include "Base.h"
#include "Condition.h"
#include "Convert.h"
#include "ExpressionPool.h"
#include "Widget.h"

namespace cpfw {

class Widget;

using TINVOKE_CHAIN = std::vector<uint32_t/*child id*/>;
using TINVOKE_CONDITION = std::pair<ExpressionEnum/*and, or*/,
                                    std::vector<Condition>>;

/**
 * @brief database of framework, who is configuried by file or user.
 * no logic here, only data.
 */
class DataStore : public std::enable_shared_from_this<DataStore>{
 public:
    static const TINVOKE_CHAIN EMPTY_INVOKE_CHAIN;
    static const TINVOKE_CONDITION EMPTY_CONDITION;
    static Profile EMPTY_PROFILE;
    static const std::vector<Convert> EMPTY_CONVERT;
    static const uint32_t EMPTY_BIND;

    DataStore();

    ~DataStore();

    std::optional<std::shared_ptr<Widget>> getWidget(const uint32_t widgetId);
    std::optional<std::shared_ptr<Widget>> getWidget(const std::string &widgetName);

    const TINVOKE_CHAIN& getChain(const uint32_t parentId);
    const TINVOKE_CHAIN& getChain(const std::string &parentName);

    Profile& getProfile(const uint32_t widgetId);
    Profile& getProfile(const std::string &widgetName);

    void setProfile(const uint32_t widgetId, int32_t value);
    void setProfile(const std::string &widgetName, int32_t value);
    void setProfile(const uint32_t widgetId, const uint32_t elementId, int32_t value);
    void setProfile(const std::string &widgetName,
            const std::string &elementName, int32_t value);

    int32_t getConvertedData(const uint32_t contextId, int32_t origin);
    int32_t getConvertedData(const std::string &context, int32_t origin);

    const std::vector<Convert>& getConvertTable(const uint32_t widgetId);
    const std::vector<Convert>& getConvertTable(const std::string &widgetName);

    const TINVOKE_CONDITION& getCondition(const uint32_t widgetId);
    const TINVOKE_CONDITION& getCondition(const std::string &widgetName);

    const uint32_t getBind(const uint32_t widgetId);
    const uint32_t getBind(const std::string &widgetName);

    std::optional<uint32_t> getIdWithStr(const std::string &name);

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
     * @param parent parent widget id
     * @param children children widget id follow the parent
     */
    void addInvokeChain(const uint32_t parent, const TINVOKE_CHAIN children);

    /**
     * @brief add profile which binds to it's unique wifget
     *
     * @param name widget id
     * @param profile data binds to the widget
     */
    void addProfile(const uint32_t widgetId, const Profile profile);

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

    void addDataConvert(
            const std::string context, std::vector<Convert> convert);
    /**
     * @brief add condition data.
     *
     * @param widgetName
     * @param condition check data and expression(or, and)
     */
    void addCondition(const std::string widgetName, TINVOKE_CONDITION condition);

    void addBind(const uint32_t widgetId, const uint32_t  bindId);

    void addStrIdPair(std::string name, uint32_t id);

 private:

    // every widget binds to a unique name
    std::map<uint32_t/*widget id*/, std::shared_ptr<Widget>> mWidgetTable;
    // every invoker can invoke multiple invokers
    std::map<uint32_t/*parent id*/, TINVOKE_CHAIN> mInvokeChainTable;
    // every widget binds to a profile, which have multi elements
    std::map<uint32_t/*widget id*/, Profile> mProfileTable;
    std::map<uint32_t/*widget id*/, uint32_t/*bind action*/> mBindTable;
    // original data can be converted to the target data for different context
    std::map<uint32_t/*widget id*/, std::map<int32_t, int32_t>> mDataMapTable;
    std::map<uint32_t/*widget id*/, std::vector<Convert>> mConvertTable;
    // used to check before action
    std::map<uint32_t/*widget id*/, TINVOKE_CONDITION> mConditionTable;
    // bind name to id
    std::map<std::string/*value str*/, uint32_t/*value id*/> mStrToIdTable;
};

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_DATASTORE_HPP_

