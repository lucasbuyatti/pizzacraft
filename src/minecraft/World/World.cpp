#include "World.h"

int sizePlayerEntities(JNIEnv* env) {
	jobject worldInstance = env->GetObjectField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["theWorld"]);
	if (CheckJNIException(env) || !worldInstance) return 0;

	jobject playerEntitiesList = env->GetObjectField(worldInstance, jni_cache::fieldCache["playerEntities"]);
	if (CheckJNIException(env) || !playerEntitiesList) {
		env->DeleteLocalRef(worldInstance);
		return 0;
	}

	jmethodID sizeMethod = env->GetMethodID(jni_cache::jclassCache["java/util/List"], "size", "()I");
	if (CheckJNIException(env) || !sizeMethod) return 0;

	jint size = env->CallIntMethod(playerEntitiesList, sizeMethod);
	if (CheckJNIException(env) || !size) return 0;

	env->DeleteLocalRef(playerEntitiesList);
	env->DeleteLocalRef(worldInstance);

	return static_cast<int>(size);
}

void clearPlayerEntities(JNIEnv* env, std::vector<jobject>& playerEntities) {

	for (jobject& player : playerEntities) {
		if (player)env->DeleteGlobalRef(player);
	}
	playerEntities.clear();
}

void world::entity::player::playerEntitiesManager(JNIEnv* env, std::vector<jobject>& playerEntities) {

	for (jobject o : playerEntities) env->DeleteLocalRef(o);
	playerEntities.clear();

	int currSize = sizePlayerEntities(env);
	if (currSize <= 0)
		return;

	if (playerEntities.size() != static_cast<size_t>(currSize)) {
		clearPlayerEntities(env, playerEntities);
	}

	jobject worldInstance = env->GetObjectField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["theWorld"]);
	if (CheckJNIException(env) || !worldInstance) return;

	jobject playerEntitiesList = env->GetObjectField(worldInstance, jni_cache::fieldCache["playerEntities"]);
	if (CheckJNIException(env) || !playerEntitiesList) {
		env->DeleteLocalRef(worldInstance);
		return;
	}

	jmethodID getMethod = env->GetMethodID(jni_cache::jclassCache["java/util/List"], "get", "(I)Ljava/lang/Object;");
	if (CheckJNIException(env) || !getMethod) {
		env->DeleteLocalRef(playerEntitiesList);
		env->DeleteLocalRef(worldInstance);
		return;
	}

	for (int i = 0; i < currSize; i++) {
		jobject player = env->CallObjectMethod(playerEntitiesList, getMethod, i);
		if (CheckJNIException(env) || !player) continue;

		playerEntities.push_back(player);

	}

	env->DeleteLocalRef(worldInstance);
	env->DeleteLocalRef(playerEntitiesList);

}

std::vector<Vec3<double>> world::entity::player::playersPos(JNIEnv* env, std::vector<jobject> playerEntities) {

	std::vector<Vec3<double>> positions{};

	if (playerEntities.empty()) return positions;

	positions.reserve(playerEntities.size());
	for (auto& entities : playerEntities) {
		jdouble posX = env->GetDoubleField(entities, jni_cache::fieldCache["posX"]);
		if (CheckJNIException(env)) continue;
		jdouble posY = env->GetDoubleField(entities, jni_cache::fieldCache["posY"]);
		if (CheckJNIException(env)) continue;
		jdouble posZ = env->GetDoubleField(entities, jni_cache::fieldCache["posZ"]);
		if (CheckJNIException(env)) continue;

		positions.emplace_back(Vec3{ static_cast<double>(posX), static_cast<double>(posY), static_cast<double>(posZ) });
	}

	return positions;
}

bool world::worldExist(JNIEnv* env) {
	jobject worldInstance = env->GetObjectField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["theWorld"]);
	if (CheckJNIException(env) || !worldInstance) {
		env->DeleteLocalRef(worldInstance);
		return false;
	}

	env->DeleteLocalRef(worldInstance);

	return true;
}