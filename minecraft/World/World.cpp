#include "World.h"

jobject theWorld(JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	jobject minecraftInstance = theMinecraft(env, map);
	if (CheckJNIException(env) || !minecraftInstance) return nullptr;

	jclass minecraftClass = env->GetObjectClass(minecraftInstance);
	if (CheckJNIException(env) || !minecraftClass) return nullptr;

	jfieldID worldFieldId = env->GetFieldID(
		minecraftClass,
		"theWorld",
		"Lnet/minecraft/client/multiplayer/WorldClient;"
	);
	if (CheckJNIException(env) || !worldFieldId) return nullptr;

	jobject tempTheWorld = env->GetObjectField(minecraftInstance, worldFieldId);
	jobject theWorld_ = env->NewGlobalRef(tempTheWorld);
	env->DeleteLocalRef(tempTheWorld);

	return theWorld_;

}