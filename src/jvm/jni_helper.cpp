#include "jni_helper.h"

JavaVM* jvm = nullptr;

jint GetJVMInstance(JNIEnv*& env) {
    jint count = 0;
    jint getJavaVMs = JNI_GetCreatedJavaVMs(&jvm, 1, &count);

    if (getJavaVMs != JNI_OK || count == 0) {
        return JNI_ERR;
    }

    if (jvm->AttachCurrentThread((void**)&env, nullptr) != JNI_OK) {
        return JNI_ERR;
    }

    if (!env) {
        return JNI_ERR;
    }

    return JNI_OK;
}

jint GetJVMTIInstance(jvmtiEnv*& jvmti) {

    if (!jvm) {
        jint count = 0;
        jint getJavaVMs = JNI_GetCreatedJavaVMs(&jvm, 1, &count);

        if (getJavaVMs != JNI_OK || count == 0) return JNI_ERR;
    }

    return jvm->GetEnv((void**)&jvmti, JVMTI_VERSION_1_2);
}

jboolean methodIsNative(jmethodID method, jvmtiEnv* jvmti) {
    jboolean isNative = JNI_FALSE;

    jvmtiError error = jvmti->IsMethodNative(method, &isNative);
    if (error != JVMTI_ERROR_NONE) return JNI_ERR;

    isNative ? isNative = JNI_TRUE : isNative = JNI_FALSE;

    return isNative;
}

void getByteCode(jmethodID method, jvmtiEnv* jvmti) {

    jint bytecodeLength;
    unsigned char* bytecodes;

    jvmtiCapabilities capabilities = { 0 };
    capabilities.can_get_bytecodes = 1;

    jvmtiError capError = jvmti->AddCapabilities(&capabilities);
    if (capError != JVMTI_ERROR_NONE) {
        char* errorName;
        jvmti->GetErrorName(capError, &errorName);

        jvmti->Deallocate((unsigned char*)errorName);
    }

    jvmtiCapabilities addedCaps;
    jvmti->GetCapabilities(&addedCaps);
    if (addedCaps.can_get_bytecodes != 1) {
        return;
    }

    jvmtiError err = jvmti->GetBytecodes(method, &bytecodeLength, &bytecodes); // Los bytecodes son...
    if (err != JVMTI_ERROR_NONE) {
        char* errName;
        jvmti->GetErrorName(err, &errName);
        jvmti->Deallocate((unsigned char*)errName);
    }
    else {

        for (int i = 0; i < bytecodeLength; i++) {
            printf("%02X \n", bytecodes[i]);
        }
        jvmti->Deallocate(bytecodes);
    }
}

bool CheckJNIException(JNIEnv* env) {
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        return true;
    }
    return false;
}

void GetClassFieldsFromClass(jclass theClass, JNIEnv* env, jvmtiEnv* jvmti) {
    jint fieldCount = 0;
    jfieldID* fields = nullptr;

    jvmtiError err = jvmti->GetClassFields(theClass, &fieldCount, &fields);
    if (err != JVMTI_ERROR_NONE || fields == nullptr) {
        printf("GetClassFields failed: %d\n", err);
        return;
    }

    jclass fieldClass = env->FindClass("java/lang/reflect/Field");
    jclass classClass = env->FindClass("java/lang/Class");
    jmethodID getDeclaredFields = env->GetMethodID(classClass, "getDeclaredFields", "()[Ljava/lang/reflect/Field;");
    jmethodID getName = env->GetMethodID(fieldClass, "getName", "()Ljava/lang/String;");

    if (!getDeclaredFields || !getName) {
        printf("Failed to get method IDs\n");
        return;
    }

    jobjectArray javaFields = (jobjectArray)env->CallObjectMethod(theClass, getDeclaredFields);
    jsize len = env->GetArrayLength(javaFields);

    for (jint i = 0; i < len; i++) {
        jobject fieldObj = env->GetObjectArrayElement(javaFields, i);
        jstring name = (jstring)env->CallObjectMethod(fieldObj, getName);
        const char* fieldNameStr = env->GetStringUTFChars(name, nullptr);

        printf("Field[%d]: %s\n", i, fieldNameStr);

        env->ReleaseStringUTFChars(name, fieldNameStr);
        env->DeleteLocalRef(name);
        env->DeleteLocalRef(fieldObj);
    }

    jvmti->Deallocate((unsigned char*)fields);
}