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
#ifndef CPFW_BASE_INCLUDE_SINGLETON_H_
#define CPFW_BASE_INCLUDE_SINGLETON_H_

#include <mutex>
#include <utility>

namespace cpfw {

/**
 *  singleton template.
 *  Note:
 *    1. create only once, if ctor with different parameters,
 *       still use the first parameter, ignore the second ctor.
 *       e.g.:
 *         class SingletonA : public Singleton<SingletonA> {
 *          public:
 *            SingletonA(int32_t a) : mA(a), mB(0) {}
 *            SingleTonA(int32_t a, uint8_t b) : mA(a), mB(b) {}
 *            int32_t mA;
 *            uint8_t mB;
 *         };
 *
 *         auto sa = SingletonA::getInstance(1);
 *         // sa->mA: 1, sa->mB: 0
 *         auto sb = SingletonA::getInstance(2, 3);
 *         // sb->mA: 1, sb->mB; 0
 *         // sa->mA: 1, sa->mB; 0
 *
 *      why: if use the second ctor, how to deal the former instance?
 *           there is no appropriate solution.
 *           when the member parameter has already be used by others, how?
 *         auto sa = SingletonA::getInstance(1);
 *         int32_t tmpa = sa->a;
 *         auto sb = SingletonA::getInstance(2, 3);
 *         // how to deal tmpa? only user has enough knowledge.
 *
 */
template<typename T>
class Singleton {
 public:
    template<typename... Args>
    static T* getInstance(Args&&... args) {
        std::call_once(gFlag, createInstance<Args...>, std::forward<Args>(args)...);
        return gInstance;
    }

 protected:
    Singleton(void) {}
    virtual ~Singleton(void) {}

 private:
    template<typename... Args>
    static void createInstance(Args&&... args) {
        static T instance(std::forward<Args>(args)...);
        gInstance = &instance;
    }

 private:
    static std::once_flag gFlag;
    static T *gInstance;

};

template<typename T>
std::once_flag Singleton<T>::gFlag;

template<typename T>
T* Singleton<T>::gInstance = nullptr;

}  // namespace cpfw

#endif  // CPFW_BASE_INCLUDE_SINGLETON_H_

