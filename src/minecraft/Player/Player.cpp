#include "Player.h"

Vec3<double> player::getPosition(JNIEnv* env) {
	static jfieldID thePlayerField = jni_cache::fieldCache["thePlayer"];
	static jfieldID posXField = jni_cache::fieldCache["posX"];
	static jfieldID posYField = jni_cache::fieldCache["posY"];
	static jfieldID posZField = jni_cache::fieldCache["posZ"];
	static jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jobject playerInstance = env->GetObjectField(theMinecraft, thePlayerField);
	if (CheckJNIException(env) || !playerInstance) return { 0.0, 0.0, 0.0 };

	jdouble posX = env->GetDoubleField(playerInstance, posXField);
	jdouble posY = env->GetDoubleField(playerInstance, posYField);
	jdouble posZ = env->GetDoubleField(playerInstance, posZField);

	env->DeleteLocalRef(playerInstance);

	return Vec3<double>{ static_cast<double>(posX), static_cast<double>(posY), static_cast<double>(posZ) };
}

// setAngles dont work in Lunar Client
void player::setAngles(JNIEnv* env, float& yaw, float& pitch) {
	jobject playerInstance = env->GetObjectField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["thePlayer"]);
	if (CheckJNIException(env) || !playerInstance) return;

	env->CallVoidMethod(playerInstance, jni_cache::methodCache["setAngles"]);

	env->DeleteLocalRef(playerInstance);
}

void player::setRotation(JNIEnv* env, float yaw, float pitch) {
	static jfieldID thePlayerField = jni_cache::fieldCache["thePlayer"];
	static jfieldID rotationYawField = jni_cache::fieldCache["rotationYaw"];
	static jfieldID rotationPitchField = jni_cache::fieldCache["rotationPitch"];
	static jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jobject playerInstance = env->GetObjectField(theMinecraft, thePlayerField);
	if (CheckJNIException(env) || !playerInstance) return;

	env->SetFloatField(playerInstance, rotationYawField, yaw);
	env->SetFloatField(playerInstance, rotationPitchField, pitch);

	env->DeleteLocalRef(playerInstance);
}

bool player::playerExist(JNIEnv* env) {
	static jfieldID thePlayerField = jni_cache::fieldCache["thePlayer"];
	static jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jobject playerInstance = env->GetObjectField(theMinecraft, thePlayerField);
	if (CheckJNIException(env) || !playerInstance) {
		if (playerInstance) env->DeleteLocalRef(playerInstance);
		return false;
	}

	env->DeleteLocalRef(playerInstance);
	return true;
}

Rotation<float> player::getAngles(JNIEnv* env) {
	static jfieldID thePlayerField = jni_cache::fieldCache["thePlayer"];
	static jfieldID rotationYawField = jni_cache::fieldCache["rotationYaw"];
	static jfieldID rotationPitchField = jni_cache::fieldCache["rotationPitch"];
	static jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jobject playerInstance = env->GetObjectField(theMinecraft, thePlayerField);
	if (CheckJNIException(env) || !playerInstance) return { 0.f, 0.f };

	jfloat yaw = env->GetFloatField(playerInstance, rotationYawField);
	jfloat pitch = env->GetFloatField(playerInstance, rotationPitchField);

	env->DeleteLocalRef(playerInstance);

	return Rotation<float>{ static_cast<float>(yaw), static_cast<float>(pitch) };
}

std::string player::getHeldItem(JNIEnv* env) {
	static jfieldID thePlayerField = jni_cache::fieldCache["thePlayer"];
	static jmethodID getHeldItemMethod = jni_cache::methodCache["getHeldItem"];
	static jmethodID getItemMethod = jni_cache::methodCache["getItem"];
	static jmethodID getRegistryNameMethod = jni_cache::methodCache["getRegistryName"];
	static jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jobject playerInstance = env->GetObjectField(theMinecraft, thePlayerField);
	if (CheckJNIException(env) || !playerInstance) return "";

	jobject heldItem = env->CallObjectMethod(playerInstance, getHeldItemMethod);
	if (CheckJNIException(env) || !heldItem) {
		env->DeleteLocalRef(playerInstance);
		return "";
	}

	jobject item = env->CallObjectMethod(heldItem, getItemMethod);
	if (CheckJNIException(env) || !item) {
		env->DeleteLocalRef(playerInstance);
		env->DeleteLocalRef(heldItem);
		return "";
	}

	jstring registryName = (jstring)env->CallObjectMethod(item, getRegistryNameMethod);
	if (CheckJNIException(env) || !registryName) {
		env->DeleteLocalRef(playerInstance);
		env->DeleteLocalRef(heldItem);
		env->DeleteLocalRef(item);
		return "";
	}

	const char* cstr = env->GetStringUTFChars(registryName, nullptr);
	std::string heldItemStr = cstr ? std::string(cstr) : "";
	env->ReleaseStringUTFChars(registryName, cstr);

	env->DeleteLocalRef(playerInstance);
	env->DeleteLocalRef(heldItem);
	env->DeleteLocalRef(item);
	env->DeleteLocalRef(registryName);

	return heldItemStr;
}

bool player::isSprinting(JNIEnv* env) {
	static jfieldID thePlayerField = jni_cache::fieldCache["thePlayer"];
	static jmethodID isSprintingMethod = jni_cache::methodCache["isSprinting"];
	static jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jobject playerInstance = env->GetObjectField(theMinecraft, thePlayerField);
	if (CheckJNIException(env) || !playerInstance) return false;

	jboolean result = env->CallBooleanMethod(playerInstance, isSprintingMethod);
	if (CheckJNIException(env)) {
		env->DeleteLocalRef(playerInstance);
		return false;
	}

	env->DeleteLocalRef(playerInstance);
	return static_cast<bool>(result);
}

bool player::onGround(JNIEnv* env) {
	static jfieldID thePlayerField = jni_cache::fieldCache["thePlayer"];
	static jfieldID onGroundField = jni_cache::fieldCache["onGround"];
	static jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jobject playerInstance = env->GetObjectField(theMinecraft, thePlayerField);
	if (CheckJNIException(env) || !playerInstance) return false;

	jboolean result = env->GetBooleanField(playerInstance, onGroundField);
	if (CheckJNIException(env)) {
		env->DeleteLocalRef(playerInstance);
		return false;
	}

	env->DeleteLocalRef(playerInstance);
	return static_cast<bool>(result);
}

float player::fallDistance(JNIEnv* env) {
	static jfieldID thePlayerField = jni_cache::fieldCache["thePlayer"];
	static jfieldID fallDistanceField = jni_cache::fieldCache["fallDistance"];
	static jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jobject playerInstance = env->GetObjectField(theMinecraft, thePlayerField);
	if (CheckJNIException(env) || !playerInstance) return 0.f;

	jfloat result = env->GetFloatField(playerInstance, fallDistanceField);
	if (CheckJNIException(env)) {
		env->DeleteLocalRef(playerInstance);
		return 0.f;
	}

	env->DeleteLocalRef(playerInstance);
	return static_cast<float>(result);
}

int player::hurtTime(JNIEnv* env) {
	static jfieldID thePlayerField = jni_cache::fieldCache["thePlayer"];
	static jfieldID hurtTimeField = jni_cache::fieldCache["hurtTime"];
	static jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jobject playerInstance = env->GetObjectField(theMinecraft, thePlayerField);
	if (CheckJNIException(env) || !playerInstance) {
		if (playerInstance) env->DeleteLocalRef(playerInstance);
		return 0;
	}

	jint result = env->GetIntField(playerInstance, hurtTimeField);
	if (CheckJNIException(env)) {
		env->DeleteLocalRef(playerInstance);
		return 0;
	}

	env->DeleteLocalRef(playerInstance);
	return static_cast<int>(result);
}

void player::setJumping(JNIEnv* env) {
	static jfieldID thePlayerField = jni_cache::fieldCache["thePlayer"];
	static jfieldID movementInputField = jni_cache::fieldCache["movementInput"];
	static jfieldID jumpField = jni_cache::fieldCache["jump"];
	static jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jobject playerInstance = env->GetObjectField(theMinecraft, thePlayerField);
	if (CheckJNIException(env) || !playerInstance) return;

	jobject movementInput = env->GetObjectField(playerInstance, movementInputField);
	if (CheckJNIException(env) || !movementInput) {
		env->DeleteLocalRef(playerInstance);
		return;
	}

	env->SetBooleanField(movementInput, jumpField, true);

	env->DeleteLocalRef(playerInstance);
	env->DeleteLocalRef(movementInput);
}

void player::jump(JNIEnv* env) {
	static jfieldID thePlayerField = jni_cache::fieldCache["thePlayer"];
	static jmethodID jumpMethod = jni_cache::methodCache["jump"];
	static jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jobject playerInstance = env->GetObjectField(theMinecraft, thePlayerField);
	if (CheckJNIException(env) || !playerInstance) return;

	env->CallVoidMethod(playerInstance, jumpMethod);
	if (CheckJNIException(env)) {
		env->DeleteLocalRef(playerInstance);
		return;
	}

	env->DeleteLocalRef(playerInstance);
}

void player::attackEntity(jobject me, jobject target, JNIEnv* env) {
	if (!me || !target || !env) return;

	static jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];
	static jfieldID playerControllerField = jni_cache::fieldCache["playerController"];
	static jmethodID attackEntityMethod = jni_cache::methodCache["attackEntity"];

	jobject playerControllerObj = env->GetObjectField(theMinecraft, playerControllerField);
	if (CheckJNIException(env) || !playerControllerObj) {
		return;
	}

	env->CallVoidMethod(playerControllerObj, attackEntityMethod, me, target);
	if (CheckJNIException(env)) {
		if (playerControllerObj) env->DeleteLocalRef(playerControllerObj);
		return;
	}

	env->DeleteLocalRef(playerControllerObj);

}

jobject player::thePlayer(JNIEnv* env) {
	static jfieldID thePlayerField = jni_cache::fieldCache["thePlayer"];
	static jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];

	jobject playerInstance = env->GetObjectField(theMinecraft, thePlayerField);
	if (CheckJNIException(env) || !playerInstance) {
		if (playerInstance) env->DeleteLocalRef(playerInstance);
		return nullptr;
	}

	return playerInstance;
}

int player::ticksExisted(JNIEnv* env) {
	static jfieldID thePlayerField = jni_cache::fieldCache["thePlayer"];
	static jobject theMinecraft = jni_cache::jobjectCache["theMinecraft"];
	static jfieldID ticksExistedField = jni_cache::fieldCache["ticksExisted"];

	if (!theMinecraft || !thePlayerField || !ticksExistedField) return -1;

	jobject playerInstance = env->GetObjectField(theMinecraft, thePlayerField);
	if (CheckJNIException(env) || !playerInstance) {
		if (playerInstance) env->DeleteLocalRef(playerInstance);
		return -1;
	}

	jint ticks = env->GetIntField(playerInstance, ticksExistedField);
	if (CheckJNIException(env)) {
		if (playerInstance) env->DeleteLocalRef(playerInstance);
		return -1;
	}

	env->DeleteLocalRef(playerInstance);
	return static_cast<int>(ticks);
}
