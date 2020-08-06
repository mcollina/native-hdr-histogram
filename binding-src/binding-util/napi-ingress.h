// © Copyright 2020 Simply Inspired Software, Inc., dba inspiredware
// Released under the MIT License located in the root directory of this repository

#include <napi.h>
using namespace Napi;

#ifndef _Bool
#define _Bool	bool
#endif /* _Bool */

template <class T> inline T getCharacter (Napi::Value value) {
    return 'j';
}

template <class T> inline T* getCharacterPtr (Napi::Value value, T& stackAllocated, bool& deletePtr) {
    deletePtr = false;
    return &stackAllocated;
}

template <class T> inline T getInt32 (Napi::Value value) {
    T retVal = static_cast<T>(value.As<Number>().Int32Value());
    return retVal;
}

template <class T> inline T getUint32 (Napi::Value value) {
    T retVal = static_cast<T>(value.As<Number>().Uint32Value());
    return retVal;
}

template <class T> inline T getInt64 (Napi::Value value) {
    T retVal = static_cast<T>(value.As<Number>().Int64Value());
    return retVal;
}

template <class T> inline T getFloat (Napi::Value value) {
    T retVal = static_cast<T>(value.As<Number>().FloatValue());
    return retVal;
}

template <class T> inline T getDouble (Napi::Value value) {
    T retVal = static_cast<T>(value.As<Number>().DoubleValue());
    return retVal;
}

template <class T> inline T* getNumberPtr (Napi::Value value, T& stackAllocated, bool& deletePtr) {
    deletePtr = false;
    return &stackAllocated;
}

void* getVoidPtr (Napi::Value value);

size_t getStringLength (Napi::Env &env, Napi::Value value);
void copyString (Napi::Env &env, Napi::Value value, char* buffer, size_t bufferLength, size_t stringLength);