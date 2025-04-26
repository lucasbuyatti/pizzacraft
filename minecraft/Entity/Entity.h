#pragma once
#include "../../includes.h"

void deletePlayerEntities(std::vector<jobject>& playerEntities, JNIEnv* env);

int sizePlayerEntities(JNIEnv* env, std::unordered_map<std::string, jclass> map);

void playerEntitiesManager(std::vector<jobject>& playerEntities, JNIEnv* env, std::unordered_map<std::string, jclass> map);




