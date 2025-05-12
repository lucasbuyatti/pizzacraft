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
    static jfieldID theWorldField = jni_cache::fieldCache["theWorld"];
    static jfieldID playerEntitiesField = jni_cache::fieldCache["playerEntities"];
    static jclass listClass = jni_cache::jclassCache["java/util/List"];
    static jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];
    static jmethodID getMethod = env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");

    for (jobject o : playerEntities) env->DeleteLocalRef(o);
    playerEntities.clear();

    int currSize = sizePlayerEntities(env);
    if (currSize <= 0)
        return;

    if (playerEntities.size() != static_cast<size_t>(currSize)) {
        clearPlayerEntities(env, playerEntities);
    }

    jobject worldInstance = env->GetObjectField(theMinecraft, theWorldField);
    if (CheckJNIException(env) || !worldInstance) return;

    jobject playerEntitiesList = env->GetObjectField(worldInstance, playerEntitiesField);
    if (CheckJNIException(env) || !playerEntitiesList) {
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
    static jfieldID posXField = jni_cache::fieldCache["posX"];
    static jfieldID posYField = jni_cache::fieldCache["posY"];
    static jfieldID posZField = jni_cache::fieldCache["posZ"];

    std::vector<Vec3<double>> positions{};
    if (playerEntities.empty()) return positions;

    positions.reserve(playerEntities.size());
    for (auto& entity : playerEntities) {

        if (entity == nullptr) {
            continue; 
        }

        jdouble posX = env->GetDoubleField(entity, posXField);
        if (CheckJNIException(env)) continue;

        jdouble posY = env->GetDoubleField(entity, posYField);
        if (CheckJNIException(env)) continue;

        jdouble posZ = env->GetDoubleField(entity, posZField);
        if (CheckJNIException(env)) continue;

        positions.emplace_back(Vec3{ posX, posY, posZ });
    }

    return positions;
}

std::vector<int> world::entity::player::hurtResistantTime(JNIEnv* env, std::vector<jobject> playerEntities) {
    static jfieldID hurtResistantTimeField = jni_cache::fieldCache["hurtResistantTime"];

    std::vector<int> hurtResistantTime{};
    if (playerEntities.empty()) return hurtResistantTime;

    hurtResistantTime.reserve(playerEntities.size());
    for (auto& entity : playerEntities) {
        
        if (entity == nullptr) {
            continue;
        }

        jint value = env->GetIntField(entity, hurtResistantTimeField);
        if (CheckJNIException(env)) {
            continue;
        }


        hurtResistantTime.emplace_back(value);
    }

    return hurtResistantTime;
}

bool world::worldExist(JNIEnv* env) {
    static jfieldID theWorldField = jni_cache::fieldCache["theWorld"];
    static jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

    jobject worldInstance = env->GetObjectField(theMinecraft, theWorldField);
    if (CheckJNIException(env) || !worldInstance) {
        env->DeleteLocalRef(worldInstance);
        return false;
    }

    env->DeleteLocalRef(worldInstance);
    return true;
}
