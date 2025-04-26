#pragma once
#include "../includes.h"

jint GetJVMInstance(JNIEnv*& env);

jint GetJVMTIInstance();

jboolean methodIsNative(jmethodID method);

void getByteCode(jmethodID method);

bool CheckJNIException(JNIEnv* env);

void GetClassFieldsFromClass(jclass theClass, JNIEnv* env);