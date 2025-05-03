#include "Minecraft.h"


int minecraft::displayHeight(JNIEnv * env) {

	jint height = env->GetIntField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["displayHeight"]);
	if (CheckJNIException(env)) return 0;

	return static_cast<int>(height);
}

int minecraft::displayWidth(JNIEnv* env) {

	jint width = env->GetIntField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["displayWidth"]);
	if (CheckJNIException(env)) return 0;

	return static_cast<int>(width);
}

bool minecraft::inGameHasFocus(JNIEnv* env) {
	jboolean inGameHasFocus = env->GetBooleanField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["inGameHasFocus"]);
	if (CheckJNIException(env)) return false;

	return static_cast<bool>(inGameHasFocus);
}

void minecraft::rightClickDelayTimer(JNIEnv* env, int delay) {

	env->SetIntField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["rightClickDelayTimer"], static_cast<jint>(delay));
	if (CheckJNIException(env)) return;

}

bool minecraft::game_settings::keyBindAttack_isKeyDown(JNIEnv* env) {

	static auto theMinecraft = jni_cache::jobjectCache["theMinecraft"];
	static auto gameSettingsField = jni_cache::fieldCache["gameSettings"];
	static auto keyBindAttackField = jni_cache::fieldCache["keyBindAttack"]; 
	static auto isKeyDownMethod = jni_cache::methodCache["isKeyDown"]; 

	jobject gameSettings = env->GetObjectField(theMinecraft, gameSettingsField);
	if (CheckJNIException(env) || !gameSettings) {
		env->DeleteLocalRef(gameSettings);
		return false;
	}

	jobject keyBindAttack = env->GetObjectField(gameSettings, keyBindAttackField);
	if (CheckJNIException(env) || !keyBindAttack) {
		env->DeleteLocalRef(gameSettings);
		env->DeleteLocalRef(keyBindAttack);
		return false;
	}

	jboolean isKeyDown = env->CallBooleanMethod(keyBindAttack, isKeyDownMethod);

	env->DeleteLocalRef(gameSettings);
	env->DeleteLocalRef(keyBindAttack);

	return static_cast<bool>(isKeyDown);
}

