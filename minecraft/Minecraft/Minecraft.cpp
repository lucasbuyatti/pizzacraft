#include "Minecraft.h"

// Liberar memoria fuera.
jobject theMinecraft(JNIEnv* env, std::unordered_map<std::string, jclass> map) {

	jclass MinecraftClass = getClass("net.minecraft.client.Minecraft", env, map);
	if (CheckJNIException(env) || !MinecraftClass) return nullptr;

	jmethodID getMinecraft = env->GetStaticMethodID(
		MinecraftClass,
		"getMinecraft",
		"()Lnet/minecraft/client/Minecraft;"
	);

	jobject localMinecraftInstance = env->CallStaticObjectMethod(
		MinecraftClass,
		getMinecraft
	);

	if (MinecraftClass) env->DeleteGlobalRef(MinecraftClass);

	if (CheckJNIException(env) || !MinecraftClass) return nullptr;

	jobject mcInstance = env->NewGlobalRef(localMinecraftInstance);

	if (localMinecraftInstance) env->DeleteLocalRef(localMinecraftInstance);

	return mcInstance;

}

bool inGameHasFocus(JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	jobject minecraftInstance = theMinecraft(env, map);
	if (CheckJNIException(env) || !minecraftInstance) return false;

	jclass minecraftClass = env->GetObjectClass(minecraftInstance);
	if (CheckJNIException(env) || !minecraftClass) return false;

	jfieldID gameFocusFieldId = env->GetFieldID(minecraftClass, "inGameHasFocus", "Z");
	if (CheckJNIException(env) || !gameFocusFieldId) return false;

	jboolean inGameHasFocus_ = env->GetBooleanField(minecraftInstance, gameFocusFieldId);

	return static_cast<bool>(inGameHasFocus_);
}