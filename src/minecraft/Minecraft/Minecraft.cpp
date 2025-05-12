#include "Minecraft.h"

int minecraft::displayHeight(JNIEnv* env) {
	static jfieldID displayHeightField = jni_cache::fieldCache["displayHeight"];
	jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jint height = env->GetIntField(theMinecraft, displayHeightField);
	if (CheckJNIException(env)) return 0;

	return static_cast<int>(height);
}

int minecraft::displayWidth(JNIEnv* env) {
	static jfieldID displayWidthField = jni_cache::fieldCache["displayWidth"];
	jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jint width = env->GetIntField(theMinecraft, displayWidthField);
	if (CheckJNIException(env)) return 0;

	return static_cast<int>(width);
}

bool minecraft::inGameHasFocus(JNIEnv* env) {
	static jfieldID inGameHasFocusField = jni_cache::fieldCache["inGameHasFocus"];
	jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jboolean inGameHasFocus = env->GetBooleanField(theMinecraft, inGameHasFocusField);
	if (CheckJNIException(env)) return false;

	return static_cast<bool>(inGameHasFocus);
}

void minecraft::rightClickDelayTimer(JNIEnv* env, int delay) {
	static jfieldID rightClickDelayField = jni_cache::fieldCache["rightClickDelayTimer"];
	jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	env->SetIntField(theMinecraft, rightClickDelayField, static_cast<jint>(delay));
	CheckJNIException(env);
}

jobject minecraft::pointedEntity(JNIEnv* env) {
	static jfieldID pointedEntityField = jni_cache::fieldCache["pointedEntity"];
	jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jobject entity = env->GetObjectField(theMinecraft, pointedEntityField);
	if (CheckJNIException(env) || !entity) {
		return nullptr;
	}

	return entity;
}

bool minecraft::game_settings::keyBindAttack_isKeyDown(JNIEnv* env) {
	static jfieldID gameSettingsField = jni_cache::fieldCache["gameSettings"];
	static jfieldID keyBindAttackField = jni_cache::fieldCache["keyBindAttack"];
	static jmethodID isKeyDownMethod = jni_cache::methodCache["isKeyDown"];
	jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jobject gameSettings = env->GetObjectField(theMinecraft, gameSettingsField);
	if (CheckJNIException(env) || !gameSettings) {
		if (gameSettings) env->DeleteLocalRef(gameSettings);
		return false;
	}

	jobject keyBindAttack = env->GetObjectField(gameSettings, keyBindAttackField);
	if (CheckJNIException(env) || !keyBindAttack) {
		env->DeleteLocalRef(gameSettings);
		if (keyBindAttack) env->DeleteLocalRef(keyBindAttack);
		return false;
	}

	jboolean isKeyDown = env->CallBooleanMethod(keyBindAttack, isKeyDownMethod);

	env->DeleteLocalRef(gameSettings);
	env->DeleteLocalRef(keyBindAttack);

	return static_cast<bool>(isKeyDown);
}

// nope
bool minecraft::game_settings::keyBindAttack_isPressed(JNIEnv* env) {
	static jfieldID gameSettingsField = jni_cache::fieldCache["gameSettings"];
	static jfieldID keyBindAttackField = jni_cache::fieldCache["keyBindAttack"];
	static jmethodID isPressedMethod = jni_cache::methodCache["isPressed"];
	jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jobject gameSettings = env->GetObjectField(theMinecraft, gameSettingsField);
	if (CheckJNIException(env) || !gameSettings) {
		if (gameSettings) env->DeleteLocalRef(gameSettings);
		return false;
	}

	jobject keyBindAttack = env->GetObjectField(gameSettings, keyBindAttackField);
	if (CheckJNIException(env) || !keyBindAttack) {
		env->DeleteLocalRef(gameSettings);
		if (keyBindAttack) env->DeleteLocalRef(keyBindAttack);
		return false;
	}

	jboolean isPressed = env->CallBooleanMethod(keyBindAttack, isPressedMethod);

	env->DeleteLocalRef(gameSettings);
	env->DeleteLocalRef(keyBindAttack);

	return static_cast<bool>(isPressed);
}

std::string minecraft::typeOfHit(JNIEnv* env) {
	static jfieldID objectMouseOverField = jni_cache::fieldCache["objectMouseOver"];
	static jfieldID typeOfHitField = jni_cache::fieldCache["typeOfHit"];
	jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jobject objectMouseOver = env->GetObjectField(theMinecraft, objectMouseOverField);
	if (CheckJNIException(env) || !objectMouseOver) {
		if (objectMouseOver) env->DeleteLocalRef(objectMouseOver);
		return "";
	}

	jobject typeOfHitEnum = env->GetObjectField(objectMouseOver, typeOfHitField);
	if (CheckJNIException(env) || !typeOfHitEnum) {
		env->DeleteLocalRef(objectMouseOver);
		if (typeOfHitEnum) env->DeleteLocalRef(typeOfHitEnum);
		return "";
	}

	jclass enumClass = env->GetObjectClass(typeOfHitEnum);
	if (CheckJNIException(env) || !enumClass) {
		env->DeleteLocalRef(objectMouseOver);
		env->DeleteLocalRef(typeOfHitEnum);
		return "";
	}

	jmethodID nameMethod = env->GetMethodID(enumClass, "name", "()Ljava/lang/String;");
	if (CheckJNIException(env) || !nameMethod) {
		env->DeleteLocalRef(objectMouseOver);
		env->DeleteLocalRef(typeOfHitEnum);
		env->DeleteLocalRef(enumClass);
		return "";
	}

	jstring enumName = static_cast<jstring>(env->CallObjectMethod(typeOfHitEnum, nameMethod));
	if (CheckJNIException(env) || !enumName) {
		env->DeleteLocalRef(objectMouseOver);
		env->DeleteLocalRef(typeOfHitEnum);
		env->DeleteLocalRef(enumClass);
		return "";
	}

	const char* nameChars = env->GetStringUTFChars(enumName, nullptr);
	std::string result(nameChars);
	env->ReleaseStringUTFChars(enumName, nameChars);

	env->DeleteLocalRef(objectMouseOver);
	env->DeleteLocalRef(typeOfHitEnum);
	env->DeleteLocalRef(enumClass);
	env->DeleteLocalRef(enumName);

	return result;
}

void minecraft::leftClickCounter(JNIEnv* env, int value) {
	static jfieldID leftClickCounterFieldId = jni_cache::fieldCache["leftClickCounter"];
	jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	env->SetIntField(theMinecraft, leftClickCounterFieldId, static_cast<jint>(value));
	CheckJNIException(env);
}

bool minecraft::game_settings::keyBindForward_isKeyDown(JNIEnv* env) {
	static jfieldID gameSettingsField = jni_cache::fieldCache["gameSettings"];
	static jfieldID keyBindForwardField = jni_cache::fieldCache["keyBindForward"];
	static jmethodID isKeyDownMethod = jni_cache::methodCache["isKeyDown"];
	static jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	if (!env || !theMinecraft || !gameSettingsField || !keyBindForwardField || !isKeyDownMethod) {
		return false;
	}

	jobject gameSettings = env->GetObjectField(theMinecraft, gameSettingsField);
	if (CheckJNIException(env) || !gameSettings) {
		if (gameSettings) env->DeleteLocalRef(gameSettings);
		return false;
	}

	jobject keyBindForward = env->GetObjectField(gameSettings, keyBindForwardField);
	if (CheckJNIException(env) || !keyBindForward) {
		env->DeleteLocalRef(gameSettings);
		if (keyBindForward) env->DeleteLocalRef(keyBindForward);
		return false;
	}

	jboolean isKeyDown = env->CallBooleanMethod(keyBindForward, isKeyDownMethod);
	if (CheckJNIException(env)) {
		env->DeleteLocalRef(gameSettings);
		env->DeleteLocalRef(keyBindForward);
		return false;
	}

	env->DeleteLocalRef(gameSettings);
	env->DeleteLocalRef(keyBindForward);

	return static_cast<bool>(isKeyDown);
}

void minecraft::packets::sendPacket(JNIEnv* env, jobject packet) {
	static jmethodID getNetHandler = jni_cache::methodCache["getNetHandler"];
	static jmethodID addToSendQueue = jni_cache::methodCache["addToSendQueue"];
	static jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	if (!theMinecraft || !packet || !getNetHandler || !addToSendQueue) return;

	jobject netHandler = env->CallObjectMethod(theMinecraft, getNetHandler);
	if (CheckJNIException(env) || !netHandler) {
		if (netHandler) env->DeleteLocalRef(netHandler);
		return;
	}

	env->CallVoidMethod(netHandler, addToSendQueue, packet);
	if (CheckJNIException(env)) {
		if (netHandler) env->DeleteLocalRef(netHandler);
		return;
	}

	env->DeleteLocalRef(netHandler); // limpieza

}
