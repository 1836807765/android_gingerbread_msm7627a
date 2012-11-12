/*
 * Copyright (C) 2007 The Android Open Source Project
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

#ifndef ANDROID_UTILS_SINGLETON_H
#define ANDROID_UTILS_SINGLETON_H

#include <stdint.h>
#include <sys/types.h>
#include <utils/threads.h>

namespace android {
// ---------------------------------------------------------------------------

template <typename TYPE, typename LOCK = Mutex>
class Singleton
{
public:
    static TYPE& getInstance() {
        Mutex::Autolock _l(sLock);
        TYPE* instance = sInstance;
        if (instance == 0) {
            instance = new TYPE();
            sInstance = instance;
        }
        return *instance;
    }
    
protected:
    ~Singleton() { };
    Singleton() { };

private:
    Singleton(const Singleton&);
    Singleton& operator = (const Singleton&);
    static LOCK  sLock;
    static TYPE* sInstance;
};

template <typename TYPE>
class Singleton<TYPE, NullMutex>
{
public:
    static TYPE& getInstance() {
        TYPE* instance = sInstance;
        if (instance == 0) {
            instance = new TYPE();
            sInstance = instance;
        }
        return *instance;
    }

protected:
    ~Singleton() { };
    Singleton() { };

private:
    Singleton(const Singleton&);
    Singleton& operator = (const Singleton&);
    static TYPE* sInstance;
};

/*
 * use ANDROID_SINGLETON_STATIC_INSTANCE(TYPE) in your implementation file
 * (eg: <TYPE>.cpp) to create the static instance of Singleton<>'s attributes,
 * and avoid to have a copy of them in each compilation units Singleton<TYPE>
 * is used.
 * NOTE: we use a version of Mutex ctor that takes a parameter, because
 * for some unknown reason using the default ctor doesn't emit the variable!
 */

#define ANDROID_SINGLETON_STATIC_INSTANCE(TYPE)                 \
    template class Singleton< TYPE >;                           \
    template<> Mutex Singleton< TYPE >::sLock(Mutex::PRIVATE);  \
    template<> TYPE* Singleton< TYPE >::sInstance(0);

#define ANDROID_SINGLETON_STATIC_INSTANCE_NO_LOCK(TYPE)                 \
   template class Singleton< TYPE, NullMutex >;                                   \
    template<> TYPE* Singleton< TYPE, NullMutex >::sInstance(0);


// ---------------------------------------------------------------------------
}; // namespace android

#endif // ANDROID_UTILS_SINGLETON_H
