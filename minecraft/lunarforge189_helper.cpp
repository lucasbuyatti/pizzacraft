#include "lunarforge189_helper.h"

extern jvmtiEnv* jvmti;

void getLoadedClasses(JNIEnv* env, std::unordered_map<std::string, jclass>& map) {

	jclass lang = env->FindClass("java/lang/Class");
	if (CheckJNIException(env) || !lang) return;
	jmethodID getName = env->GetMethodID(lang, "getName", "()Ljava/lang/String;");
	if (CheckJNIException(env) || !getName) return;

	jclass* classes;
	jint amount;

	jvmtiError getLoadedClasses = jvmti->GetLoadedClasses(&amount, &classes);
	if (CheckJNIException(env) || getLoadedClasses != 0) return;

	for (int i = 0; i < amount; i++) {
		jstring className = (jstring)env->CallObjectMethod(classes[i], getName);

		const char* classNameStr = env->GetStringUTFChars(className, nullptr);

		env->ReleaseStringUTFChars(className, classNameStr);

		map.emplace(classNameStr, classes[i]);

		env->DeleteLocalRef(className);
	}

	env->DeleteLocalRef(lang);

	delete[] classes;
}

jclass getClass(const char* className, JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	jclass mcClass = nullptr;

	auto it = map.find(className);

	if (it != map.end()) {
		mcClass = (jclass)env->NewGlobalRef(it->second);
	}

	return mcClass;
}