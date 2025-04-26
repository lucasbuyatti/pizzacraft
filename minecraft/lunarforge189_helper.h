#include "../includes.h"

void getLoadedClasses(JNIEnv* env, std::unordered_map<std::string, jclass>& map);

jclass getClass(const char* className, JNIEnv* env, std::unordered_map<std::string, jclass> map);
