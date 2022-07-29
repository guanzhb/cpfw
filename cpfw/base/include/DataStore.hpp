/**
 * Copyright (C) 2020 The Cross Platform Framework Project
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

#ifndef CPFW_BASE_INCLUDE_DATASTORE_HPP_
#define CPFW_BASE_INCLUDE_DATASTORE_HPP_

#include <map>
#include <string>
#include <vector>
#include <functional>

#include "cpfw/base/include/Base.h"

namespace cpfw {

/**
 * @brief database of framework, who is configuried by file or user.
 * no logic here, only data.
 */
class DataStore {
 public:
    DataStore() {
    }

    ~DataStore() {
    }

    TINTERFACE getInterface(std::string interfaceName) {
        TINTERFACE ret = nullptr;
        if (mInterfaceTable.find(interfaceName) != mInterfaceTable.end()) {
            ret = mInterfaceTable[interfaceName];
        }
        return ret;
    }

    std::shared_ptr<WidgetBase> getWidget(std::string interfaceName) {
        std::shared_ptr<WidgetBase> ret = std::make_shared<WidgetBase>();
        if (mWidgetTable.find(interfaceName) != mWidgetTable.end()) {
            ret = mWidgetTable[interfaceName];
        }
        return ret;
    }

    std::vector<std::string> getChain(std::string parentName) {
        std::vector<std::string> ret;
        if (mInvokeChainTable.find(parentName) != mInvokeChainTable.end()) {
            ret = mInvokeChainTable[parentName];
        }
        return ret;
    }

    Profile* getProfile(std::string interfaceName) {
        Profile* ret = nullptr;
        if (mProfileTable.find(interfaceName) != mProfileTable.end()) {
            ret = &mProfileTable[interfaceName];
        }
        return ret;
    }

    int32_t getConvertedData(std::string &context, int32_t origin) {
        auto convert = mDataConvertTable.find(context);
        if (convert == mDataConvertTable.end()) {
            return origin;  // TODO(guanzhb) LODI
        }
        auto data = convert->second.find(origin);
        if (data == convert->second.end()) {
            return origin;  // TODO(guanzhb) LODI
        }
        return data->second;
    }

 public:
    /**
     * @brief bind every interface to it's unique name.
     * invoke in constructor.
     *
     * @param interfaceName interface name
     * @param interface the interface who can be invoked.
     */
    void addInterface(const std::string interfaceName, TINTERFACE interface) {
        mInterfaceTable.emplace(interfaceName, interface);
    }

    /**
     * @brief bind every widget to it's unique name.
     * invoke in constructor.
     *
     * @param widgetName widget name
     * @param widget the widget who can be invoked.
     */
    void addWidget(const std::string widgetName, std::shared_ptr<WidgetBase> widget) {
        mWidgetTable.emplace(widgetName, widget);
    }

    /**
     * @brief initialize invoke chain.
     * every parent can have multiple children.
     * such as: when set volume, we can then set effect and deal duck logic.
     *
     * @param parent parent interface
     * @param children children interface follow the parent
     */
    void initializeInvokeChain(
            std::string parent, const std::vector<std::string> children) {
        mInvokeChainTable.emplace(parent, children);
    }

    /**
     * @brief initialize profile which binds to it's unique interface.
     *
     * @param interfaceName interface name
     * @param profile data binds to the interface
     */
    void initializeProfile(const std::string interfaceName, const Profile profile) {
        mProfileTable.emplace(interfaceName, profile);
    }

    /**
     * @brief initialize data table to convert.
     * such as: we can convert msgId 0x1111 to msgId 0x2222.
     *
     * @param contxt data context
     * @param origin original data
     * @param target converted data
     */
    void initializeDataConvertTable(
            const std::string context, int32_t origin, int32_t target) {
        std::map<int32_t, int32_t> convert;
        convert.emplace(origin, target);
        mDataConvertTable.emplace(context, convert);
    }

 private:
    // every interface binds to a unique name
    std::map<std::string/*unique name*/, TINTERFACE> mInterfaceTable;
    // every widget binds to a unique name
    std::map<std::string/*unique name*/, std::shared_ptr<WidgetBase>> mWidgetTable;
    // every invoker can invoke multiple invokers
    std::map<std::string/*parent name*/, std::vector<std::string/*children name*/>>
                mInvokeChainTable;
    // every interface binds to a profile, which have multi elements
    std::map<std::string/*interface name*/, Profile> mProfileTable;
    // original data can be converted to the target data for different context
    std::map<std::string/*context*/, std::map<int32_t, int32_t>> mDataConvertTable;
};

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_DATASTORE_HPP_

