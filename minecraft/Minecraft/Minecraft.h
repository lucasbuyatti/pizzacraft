#pragma once
#include "../../includes.h"

jobject theMinecraft(JNIEnv* env, std::unordered_map<std::string, jclass> map);

bool inGameHasFocus(JNIEnv* env, std::unordered_map<std::string, jclass> map);