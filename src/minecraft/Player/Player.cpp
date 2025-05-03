#include "Player.h"

Vec3<double> player::getPosition(JNIEnv* env) {

	jobject playerInstance = env->GetObjectField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["thePlayer"]);
	if (CheckJNIException(env) || !playerInstance) return { 0.f,0.f,0.f };

	jdouble posX = env->GetDoubleField(playerInstance, jni_cache::fieldCache["posX"]);
	jdouble posY = env->GetDoubleField(playerInstance, jni_cache::fieldCache["posY"]);
	jdouble posZ = env->GetDoubleField(playerInstance, jni_cache::fieldCache["posZ"]);

	env->DeleteLocalRef(playerInstance);

	return Vec3{ static_cast<double>(posX), static_cast<double>(posY), static_cast<double>(posZ) };

}

// setAngles dont work in Lunar Client
void player::setAngles(JNIEnv* env, float& yaw, float& pitch) {
	jobject playerInstance = env->GetObjectField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["thePlayer"]);
	if (CheckJNIException(env) || !playerInstance) return;

	env->CallVoidMethod(playerInstance, jni_cache::methodCache["setAngles"]);

	env->DeleteLocalRef(playerInstance);
}

void player::setRotation(JNIEnv* env, float yaw, float pitch) {

	jobject playerInstance = env->GetObjectField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["thePlayer"]);
	if (CheckJNIException(env) || !playerInstance) return;

	env->SetFloatField(playerInstance, jni_cache::fieldCache["rotationYaw"], yaw);
	env->SetFloatField(playerInstance, jni_cache::fieldCache["rotationPitch"], pitch);
}

bool player::playerExist(JNIEnv* env) {
	jobject playerInstance = env->GetObjectField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["thePlayer"]);
	if (CheckJNIException(env) || !playerInstance) {
		env->DeleteLocalRef(playerInstance);
		return false;
	}

	env->DeleteLocalRef(playerInstance);

	return true;
}

Rotation<float> player::getAngles(JNIEnv* env) {
	jobject playerInstance = env->GetObjectField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["thePlayer"]);
	if (CheckJNIException(env) || !playerInstance) return { 0.f,0.f };

	jfloat yaw = env->GetFloatField(playerInstance, jni_cache::fieldCache["rotationYaw"]);
	jfloat pitch = env->GetFloatField(playerInstance, jni_cache::fieldCache["rotationPitch"]);

	env->DeleteLocalRef(playerInstance);

	return Rotation{ static_cast<float>(yaw),static_cast<float>(pitch) };
}

std::string player::getHeldItem(JNIEnv* env) {
	jobject playerInstance = env->GetObjectField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["thePlayer"]);
	if (CheckJNIException(env) || !playerInstance) return "";

	jobject callgetHeldItem = env->CallObjectMethod(playerInstance, jni_cache::methodCache["getHeldItem"]);
	if (CheckJNIException(env) || !callgetHeldItem) {
		env->DeleteLocalRef(playerInstance);
		return "";
	}

	jobject callGetItem = env->CallObjectMethod(callgetHeldItem, jni_cache::methodCache["getItem"]);
	if (CheckJNIException(env) || !callGetItem) {
		env->DeleteLocalRef(playerInstance);
		env->DeleteLocalRef(callgetHeldItem);
		return "";
	}

	jstring registryNameString = (jstring)env->CallObjectMethod(callGetItem, jni_cache::methodCache["getRegistryName"]);
	if (CheckJNIException(env) || !registryNameString) {
		env->DeleteLocalRef(playerInstance);
		env->DeleteLocalRef(callgetHeldItem);
		env->DeleteLocalRef(callGetItem);
		return "";
	}

	const char* cstr = env->GetStringUTFChars(registryNameString, nullptr);
	std::string heldItem = cstr ? std::string(cstr) : "";
	env->ReleaseStringUTFChars(registryNameString, cstr);

	env->DeleteLocalRef(playerInstance);
	env->DeleteLocalRef(callgetHeldItem);
	env->DeleteLocalRef(callGetItem);
	env->DeleteLocalRef(registryNameString);

	return heldItem;

}

bool player::isSprinting(JNIEnv* env) {

	jobject playerInstance = env->GetObjectField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["thePlayer"]);
	if (CheckJNIException(env) || !playerInstance) return false;

	jboolean isSprinting = env->CallBooleanMethod(playerInstance, jni_cache::methodCache["isSprinting"]);
	if (CheckJNIException(env)) {
		env->DeleteLocalRef(playerInstance);
		return false;
	}

	env->DeleteLocalRef(playerInstance);

	return static_cast<bool>(isSprinting);
}

bool player::onGround(JNIEnv* env) {
	jobject playerInstance = env->GetObjectField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["thePlayer"]);
	if (CheckJNIException(env) || !playerInstance) return false;

	jboolean onGround = env->GetBooleanField(playerInstance, jni_cache::fieldCache["onGround"]);
	if (CheckJNIException(env)) {
		env->DeleteLocalRef(playerInstance);
		return false;
	}

	env->DeleteLocalRef(playerInstance);

	return static_cast<bool>(onGround);
}

float player::fallDistance(JNIEnv* env) {
	jobject playerInstance = env->GetObjectField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["thePlayer"]);
	if (CheckJNIException(env) || !playerInstance) {
		env->DeleteLocalRef(playerInstance);
		return 0.f;
	}

	jfloat fallDistance = env->GetFloatField(playerInstance, jni_cache::fieldCache["fallDistance"]);
	if (CheckJNIException(env)) {
		env->DeleteLocalRef(playerInstance);
		return 0.f;
	}

	env->DeleteLocalRef(playerInstance);

	return static_cast<float>(fallDistance);
}

int player::hurtTime(JNIEnv* env) {
	jobject playerInstance = env->GetObjectField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["thePlayer"]);
	if (CheckJNIException(env) || !playerInstance) {
		env->DeleteLocalRef(playerInstance);
		return 0;
	}

	jint hurtTime = env->GetIntField(playerInstance, jni_cache::fieldCache["hurtTime"]);
	if (CheckJNIException(env)) {
		env->DeleteLocalRef(playerInstance);
		return 0;
	}

	env->DeleteLocalRef(playerInstance);

	return static_cast<int>(hurtTime);
}

void player::setJumping(JNIEnv* env) {
	jobject playerInstance = env->GetObjectField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["thePlayer"]);
	if (CheckJNIException(env) || !playerInstance) return;

	jobject movementInput = env->GetObjectField(playerInstance, jni_cache::fieldCache["movementInput"]);
	if (CheckJNIException(env) || !movementInput) {
		env->DeleteLocalRef(playerInstance);
		return;
	}

	env->SetBooleanField(movementInput, jni_cache::fieldCache["jump"], true);

	env->DeleteLocalRef(playerInstance);
	env->DeleteLocalRef(movementInput);
}

void player::jump(JNIEnv* env) {
	jobject playerInstance = env->GetObjectField(jni_cache::jobjectCache["theMinecraft"], jni_cache::fieldCache["thePlayer"]);
	if (CheckJNIException(env) || !playerInstance) {
		env->DeleteLocalRef(playerInstance);
		return;
	}

	env->CallVoidMethod(playerInstance, jni_cache::methodCache["jump"]);
	if (CheckJNIException(env)) {
		env->DeleteLocalRef(playerInstance);
		return;
	}

	env->DeleteLocalRef(playerInstance);
}