#include "Entity.h"

void playerEntitiesManager(std::vector<jobject>& playerEntities, JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	
	int currSize = sizePlayerEntities(env, map);

	if (playerEntities.size() == currSize) return;

	deletePlayerEntities(playerEntities, env);
	
	jobject worldInstance = theWorld(env, map);
	if (CheckJNIException(env) || !worldInstance) return;

	jclass worldClass = env->GetObjectClass(worldInstance);
	if (CheckJNIException(env) || !worldClass) return;

	jfieldID playerEntitiesFieldId = env->GetFieldID(worldClass, "playerEntities", "Ljava/util/List;");
	if (CheckJNIException(env) || !playerEntitiesFieldId) return;

	jobject playerEntitiesList = env->GetObjectField(worldInstance, playerEntitiesFieldId);
	if (CheckJNIException(env) || !playerEntitiesList) return;

	jclass listClass = env->FindClass("java/util/List");
	if (CheckJNIException(env) || !listClass) return;

	jmethodID getMethod = env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");
	if (CheckJNIException(env) || !getMethod) return;

	int size = sizePlayerEntities(env, map);

	for (int i = 0; i < size; i++) {
		jobject player = env->CallObjectMethod(playerEntitiesList, getMethod, i);
		if (CheckJNIException(env) || !player) continue;


		jobject globalPlayer = env->NewGlobalRef(player);
		playerEntities.push_back(globalPlayer);

		env->DeleteLocalRef(player);
	}

	env->DeleteLocalRef(playerEntitiesList);
	env->DeleteLocalRef(worldClass);
	env->DeleteLocalRef(listClass);
	env->DeleteGlobalRef(worldInstance);
}

int sizePlayerEntities(JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	jobject worldInstance = theWorld(env, map);
	if (CheckJNIException(env) || !worldInstance) return 0;

	jclass worldClass = env->GetObjectClass(worldInstance);
	if (CheckJNIException(env) || !worldClass) return 0;

	jfieldID playerEntitiesFieldId = env->GetFieldID(worldClass, "playerEntities", "Ljava/util/List;");
	if (CheckJNIException(env) || !playerEntitiesFieldId) return 0;

	jobject playerEntitiesList = env->GetObjectField(worldInstance, playerEntitiesFieldId);
	if (CheckJNIException(env) || !playerEntitiesList) return 0;

	jmethodID sizeMethodId = env->GetMethodID(env->GetObjectClass(playerEntitiesList), "size", "()I");
	if (CheckJNIException(env) || !sizeMethodId) return 0;

	jint size = env->CallIntMethod(playerEntitiesList, sizeMethodId);
	if (CheckJNIException(env) || !size) return 0;

	env->DeleteLocalRef(playerEntitiesList);
	env->DeleteLocalRef(worldClass);
	env->DeleteGlobalRef(worldInstance);

	return static_cast<int>(size);
}

void deletePlayerEntities(std::vector<jobject>& playerEntities, JNIEnv* env) {
	for (auto& player : playerEntities) {
		env->DeleteGlobalRef(player);
	}
	playerEntities.clear();
}

