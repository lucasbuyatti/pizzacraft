#pragma once
#include "../../includes.h"

void getLoadedClasses(JNIEnv* env, jvmtiEnv* jvmti, std::unordered_map<std::string, jclass>& map);

namespace jni_cache {

    extern std::unordered_map<std::string, jmethodID> methodCache;
    extern std::unordered_map<std::string, jfieldID> fieldCache;
    extern std::unordered_map<std::string, jclass> jclassCache;
    extern std::unordered_map<std::string, jobject> jobjectCache;

    void setCache(JNIEnv* env, std::unordered_map<std::string, jclass> map);

    void init_cache(JNIEnv* env, std::unordered_map<std::string, jclass> map);

    void delete_cache(JNIEnv* env);
}