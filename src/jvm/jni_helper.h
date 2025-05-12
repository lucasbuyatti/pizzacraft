#pragma once
#include "../../includes.h"

jint GetJVMInstance(JNIEnv*& env);

jint GetJVMTIInstance(jvmtiEnv*& jvmti);

jint detachCurrThread(JNIEnv* env);

jboolean methodIsNative(jmethodID method, jvmtiEnv* jvmti);

void getByteCode(jmethodID method, jvmtiEnv* jvmti);

bool CheckJNIException(JNIEnv* env);

void GetClassFieldsFromClass(jclass theClass, JNIEnv* env, jvmtiEnv* jvmti);