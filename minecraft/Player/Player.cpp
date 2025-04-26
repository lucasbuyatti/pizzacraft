#include "Player.h"

jobject thePlayer(JNIEnv* env, std::unordered_map<std::string, jclass> map) {

	jobject mcInstance = theMinecraft(env, map);
	if (CheckJNIException(env) || !mcInstance) return nullptr;

	jclass MinecraftClass = env->GetObjectClass(mcInstance);
	if (CheckJNIException(env) || !MinecraftClass) return nullptr;

	jfieldID thePlayerFieldId = env->GetFieldID(
		MinecraftClass,
		"thePlayer",
		"Lnet/minecraft/client/entity/EntityPlayerSP;"
	);
	if (CheckJNIException(env) || !thePlayerFieldId) return nullptr;

	jobject tempThePlayer = env->GetObjectField(mcInstance, thePlayerFieldId);
	if (CheckJNIException(env) || !tempThePlayer) return nullptr;

	jobject thePlayer = env->NewGlobalRef(tempThePlayer);

	env->DeleteLocalRef(MinecraftClass);
	env->DeleteLocalRef(tempThePlayer);
	env->DeleteGlobalRef(mcInstance);

	return thePlayer;
}

Rotate<float> getAngles(JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	jobject playerInstance = thePlayer(env, map);
	if (CheckJNIException(env) || !playerInstance) return { 0.f,0.f };

	jclass entityPlayerSPClass = env->GetObjectClass(playerInstance);
	if (CheckJNIException(env) || !entityPlayerSPClass) return { 0.f,0.f };

	jfieldID yawFieldId = env->GetFieldID(
		entityPlayerSPClass,
		"rotationYaw",
		"F"
	);
	if (CheckJNIException(env) || !yawFieldId) return { 0.f,0.f };

	jfieldID pitchFieldId = env->GetFieldID(
		entityPlayerSPClass,
		"rotationPitch",
		"F"
	);
	if (CheckJNIException(env) || !yawFieldId) return { 0.f,0.f };

	jfloat yawObj = env->GetFloatField(playerInstance, yawFieldId);
	if (CheckJNIException(env) || !yawObj) return { 0,0 };
	jfloat pitchObj = env->GetFloatField(playerInstance, pitchFieldId);
	if (CheckJNIException(env) || !pitchObj) return { 0,0 };

	env->DeleteLocalRef(entityPlayerSPClass);
	env->DeleteGlobalRef(playerInstance);

	return Rotate{ static_cast<float>(yawObj), static_cast<float>(pitchObj) };
}

// entityClass is not the class
Rotate<float> getPrevAngles(JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	jobject playerInstance = thePlayer(env, map);
	if (CheckJNIException(env) || !playerInstance) return { 0.f,0.f };

	jclass entityClass = getClass("net.minecraft.entity.Entity", env, map);
	if (CheckJNIException(env) || !entityClass) return{ 0.f,0.f };

	jfieldID prevRotationYawFieldId = env->GetFieldID(
		entityClass,
		"prevRotationYaw",
		"F"
	);
	if (CheckJNIException(env) || !prevRotationYawFieldId) return{ 0.f,0.f };

	jfieldID prevRotationPitchFieldId = env->GetFieldID(
		entityClass,
		"prevRotationPitch",
		"F"
	);
	if (CheckJNIException(env) || !prevRotationPitchFieldId) return{ 0.f,0.f };

	jfloat prevRotationYaw = env->GetFloatField(playerInstance, prevRotationYawFieldId);
	if (CheckJNIException(env)) return{ 0.f,0.f };
	jfloat prevRotationPitch = env->GetFloatField(playerInstance, prevRotationPitchFieldId);
	if (CheckJNIException(env)) return{ 0.f,0.f };

	env->DeleteLocalRef(entityClass);
	env->DeleteGlobalRef(playerInstance);

	return Rotate{ static_cast<float>(prevRotationYaw), static_cast<float>(prevRotationPitch) };
}

// entityClass is not the class
void setPrevAngles(float& yaw, float& pitch, JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	jobject playerInstance = thePlayer(env, map);
	if (CheckJNIException(env) || !playerInstance) return;

	jclass entityClass = getClass("net.minecraft.entity.Entity", env, map);
	if (CheckJNIException(env) || !entityClass) return;

	jfieldID prevRotationYawFieldId = env->GetFieldID(entityClass, "prevRotationYaw", "F");
	if (CheckJNIException(env) || !prevRotationYawFieldId) return;
	jfieldID prevRotationPitchFieldId = env->GetFieldID(entityClass, "prevRotationPitch", "F");
	if (CheckJNIException(env) || !prevRotationPitchFieldId) return;

	env->SetFloatField(playerInstance, prevRotationYawFieldId, static_cast<jfloat>(yaw));
	if (CheckJNIException(env)) return;

	env->SetFloatField(playerInstance, prevRotationPitchFieldId, static_cast<jfloat>(pitch));
	if (CheckJNIException(env)) return;

	env->DeleteLocalRef(entityClass);
	env->DeleteGlobalRef(playerInstance);

}

void setAngles(float& yaw, float& pitch, JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	jobject playerInstance = thePlayer(env, map);
	if (CheckJNIException(env) || !playerInstance) return;

	//jclass entityClass = getClass("net.minecraft.entity.Entity", env, map);
	jclass entityPlayerSPClass = env->GetObjectClass(playerInstance);
	if (CheckJNIException(env) || !entityPlayerSPClass) return;

	jmethodID setAnglesMethodId = env->GetMethodID(entityPlayerSPClass, "setAngles", "(FF)V");
	if (CheckJNIException(env) || !setAnglesMethodId) return;

	env->CallVoidMethod(playerInstance, setAnglesMethodId, yaw, pitch);

	//jfieldID yawFieldId = env->GetFieldID(
	//	entityClass,
	//	"rotationYaw",
	//	"F"
	//);
	//if (CheckJNIException(env) || !yawFieldId) return;

	//jfieldID pitchFieldId = env->GetFieldID(
	//	entityClass,
	//	"rotationPitch",
	//	"F"
	//);
	//if (CheckJNIException(env) || !yawFieldId) return;

	//env->DeleteLocalRef(entityClass);

	//Rotate<float> angle = getAngles(env, map);     angle.normalize();
	//Rotate<float> prevAngle = getPrevAngles(env, map); prevAngle.normalize();

	//float f = angle.pitch;  // rotationPitch viejo
	//float f1 = angle.yaw;    // rotationYaw viejo

	//// 2) Calcula nuevos
	//float newYaw = f1 + yaw * 0.15f;
	//float newPitch = f - pitch * 0.15f;
	//newPitch = std::clamp(newPitch, -90.0f, 90.0f);

	//// 3) Setea primero los rotation
	//env->SetFloatField(playerInstance, yawFieldId, newYaw);
	//if (CheckJNIException(env)) return;
	//env->SetFloatField(playerInstance, pitchFieldId, newPitch);
	//if (CheckJNIException(env)) return;

	//// 4) Ahora calcula y setea los prevRotation*
	//float updatedPrevYaw = prevAngle.yaw + (newYaw - f1);
	//float updatedPrevPitch = prevAngle.pitch + (newPitch - f);

	//setPrevAngles(updatedPrevYaw, updatedPrevPitch, env, map);

	//env->SetFloatField(playerInstance, yawFieldId, static_cast<jfloat>(newYaw));
	//if (CheckJNIException(env)) return;
	//env->SetFloatField(playerInstance, pitchFieldId, static_cast<jfloat>(newPitch));
	//if (CheckJNIException(env)) return;

	env->DeleteGlobalRef(playerInstance);

}

// changed but not tested
Vec3<double> getPosition(JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	jobject playerInstance = thePlayer(env, map);
	if (CheckJNIException(env) || !playerInstance) return { 0.f,0.f ,0.f };

	jclass entityPlayerSPClass = getClass("net.minecraft.entity.Entity", env, map);
	if (CheckJNIException(env) || !entityPlayerSPClass) return { 0.f,0.f ,0.f };

	jfieldID posXFieldId = env->GetFieldID(entityPlayerSPClass, "posX", "D");
	if (CheckJNIException(env) || !posXFieldId) return { 0.f,0.f ,0.f };

	jfieldID posYFieldId = env->GetFieldID(entityPlayerSPClass, "posY", "D");
	if (CheckJNIException(env) || !posYFieldId) return { 0.f,0.f ,0.f };

	jfieldID posZFieldId = env->GetFieldID(entityPlayerSPClass, "posZ", "D");
	if (CheckJNIException(env) || !posZFieldId) return { 0.f,0.f ,0.f };

	jdouble posX = env->GetDoubleField(playerInstance, posXFieldId);
	if (CheckJNIException(env)) return { 0.f,0.f ,0.f };
	jdouble posY = env->GetDoubleField(playerInstance, posYFieldId);
	if (CheckJNIException(env)) return { 0.f,0.f ,0.f };
	jdouble posZ = env->GetDoubleField(playerInstance, posZFieldId);
	if (CheckJNIException(env)) return { 0.f,0.f ,0.f };

	env->DeleteGlobalRef(playerInstance);
	env->DeleteLocalRef(entityPlayerSPClass);

	return Vec3{ static_cast<double>(posX), static_cast<double>(posY), static_cast<double>(posZ) };

}

// Work, but is weird
bool onGround(JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	jobject playerInstance = thePlayer(env, map);
	if (CheckJNIException(env) || !playerInstance) return false;

	//jclass entityClass = getClass("net.minecraft.entity.Entity", env, map);
	jclass entityPlayerSPClass = env->GetObjectClass(playerInstance);
	if (CheckJNIException(env) || !entityPlayerSPClass) return false;

	jfieldID onGroundFieldId = env->GetFieldID(entityPlayerSPClass, "onGround", "Z");
	if (CheckJNIException(env) || !onGroundFieldId) return false;

	jboolean onGround_ = env->GetBooleanField(playerInstance, onGroundFieldId);
	if (CheckJNIException(env)) return false;

	env->DeleteGlobalRef(playerInstance);
	env->DeleteLocalRef(entityPlayerSPClass);

	return !static_cast<bool>(onGround_);

}

// entityClass is not the class
bool isAirBorne(JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	jobject playerInstance = thePlayer(env, map);
	if (CheckJNIException(env) || !playerInstance) return false;

	jclass entityClass = getClass("net.minecraft.entity.Entity", env, map);
	if (CheckJNIException(env) || !entityClass) return false;

	jfieldID isAirBorneFieldId = env->GetFieldID(entityClass, "isAirBorne", "Z");
	if (CheckJNIException(env) || !isAirBorneFieldId) return false;

	jboolean isAirBorne_ = env->GetBooleanField(playerInstance, isAirBorneFieldId);
	if (CheckJNIException(env)) return false;

	env->DeleteGlobalRef(playerInstance);
	env->DeleteLocalRef(entityClass);

	return static_cast<bool>(isAirBorne_);
}

bool isSprinting(JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	jobject playerInstance = thePlayer(env, map);
	if (CheckJNIException(env) || !playerInstance) return false;

	jclass entityPlayerSPClass = env->GetObjectClass(playerInstance);
	if (CheckJNIException(env) || !entityPlayerSPClass) return false;

	jmethodID isSprintingMethodId = env->GetMethodID(entityPlayerSPClass, "isSprinting", "()Z");
	if (CheckJNIException(env) || !isSprintingMethodId) return false;

	jboolean isSprinting_ = env->CallBooleanMethod(playerInstance, isSprintingMethodId);
	if (CheckJNIException(env)) return false;

	env->DeleteGlobalRef(playerInstance);
	env->DeleteLocalRef(entityPlayerSPClass);

	return static_cast<bool>(isSprinting_);
}

int hurtTime(JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	jobject playerInstance = thePlayer(env, map);
	if (CheckJNIException(env) || !playerInstance) return -1;

	jclass entityPlayerSPClass = env->GetObjectClass(playerInstance);
	if (CheckJNIException(env) || !entityPlayerSPClass) return -1;

	jfieldID hurtTimeFieldId = env->GetFieldID(entityPlayerSPClass, "hurtTime", "I");
	if (CheckJNIException(env) || !hurtTimeFieldId) return -1;

	jint hurtTime = env->GetIntField(playerInstance, hurtTimeFieldId);

	return static_cast<int>(hurtTime);
}

std::string typeOfHit(JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	jobject mcInstance = theMinecraft(env, map);
	if (CheckJNIException(env) || !mcInstance) return "";

	jclass minecraftClass = env->GetObjectClass(mcInstance);
	if (CheckJNIException(env) || !minecraftClass) return "";

	jfieldID objectMouseOverFieldId = env->GetFieldID(minecraftClass, "objectMouseOver", "Lnet/minecraft/util/MovingObjectPosition;");
	if (CheckJNIException(env) || !objectMouseOverFieldId) return "";

	jobject objectMouseOver = env->GetObjectField(mcInstance, objectMouseOverFieldId);
	if (CheckJNIException(env) || !objectMouseOver) return "";

	jclass objectMouseOverClass = env->GetObjectClass(objectMouseOver);
	if (CheckJNIException(env) || !objectMouseOverClass) return "";

	jfieldID typeOfHitFieldId = env->GetFieldID(objectMouseOverClass, "typeOfHit", "Lnet/minecraft/util/MovingObjectPosition$MovingObjectType;");
	if (CheckJNIException(env) || !typeOfHitFieldId) return "";

	jobject typeOfHitEnum = env->GetObjectField(objectMouseOver, typeOfHitFieldId);
	if (CheckJNIException(env) || !typeOfHitEnum) return "";

	jclass enumClass = env->GetObjectClass(typeOfHitEnum);
	if (CheckJNIException(env) || !enumClass) return "";

	jmethodID nameMethod = env->GetMethodID(enumClass, "name", "()Ljava/lang/String;");
	if (CheckJNIException(env) || !nameMethod) return "";

	jstring enumName = (jstring)env->CallObjectMethod(typeOfHitEnum, nameMethod);
	if (CheckJNIException(env) || !enumName) return "";

	const char* nameChars = env->GetStringUTFChars(enumName, nullptr);
	std::string result(nameChars);
	env->ReleaseStringUTFChars(enumName, nameChars);

	env->DeleteGlobalRef(mcInstance);
	env->DeleteLocalRef(enumName);
	env->DeleteLocalRef(typeOfHitEnum);
	env->DeleteLocalRef(enumClass);
	env->DeleteLocalRef(objectMouseOver);
	env->DeleteLocalRef(objectMouseOverClass);

	return result;
}

std::string getHeldItemName(JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	jobject playerInstance = thePlayer(env, map);
	if (CheckJNIException(env) || !playerInstance) return "";

	jclass entityPlayerClass = getClass("net.minecraft.entity.player.EntityPlayer", env, map);
	if (CheckJNIException(env) || !entityPlayerClass) return "";

	jmethodID getHeldItemMethodId = env->GetMethodID(
		entityPlayerClass,
		"getHeldItem",
		"()Lnet/minecraft/item/ItemStack;"
	);
	if (CheckJNIException(env) || !getHeldItemMethodId) return "";

	jobject callgetHeldItem = env->CallObjectMethod(playerInstance, getHeldItemMethodId);
	if (CheckJNIException(env) || !callgetHeldItem) return "";

	jclass itemStackClass = env->GetObjectClass(callgetHeldItem);
	if (CheckJNIException(env) || !itemStackClass) return "";

	jmethodID getItemMethodId = env->GetMethodID(itemStackClass, "getItem", "()Lnet/minecraft/item/Item;");
	if (CheckJNIException(env) || !getItemMethodId) {
		env->DeleteLocalRef(itemStackClass);
		return "";
	}
	env->DeleteLocalRef(itemStackClass);

	jobject callgetItem = env->CallObjectMethod(callgetHeldItem, getItemMethodId);
	if (CheckJNIException(env) || !callgetItem) {
		env->DeleteLocalRef(callgetHeldItem);
		return "";
	}
	env->DeleteLocalRef(callgetHeldItem);

	jclass itemClass = env->GetObjectClass(callgetItem);
	if (CheckJNIException(env) || !itemClass) {
		env->DeleteLocalRef(callgetItem);
		return "";
	}

	jmethodID getRegistryNameMethodId = env->GetMethodID(itemClass, "getRegistryName", "()Ljava/lang/String;");
	if (CheckJNIException(env) || !getRegistryNameMethodId) {
		env->DeleteLocalRef(itemClass);
		env->DeleteLocalRef(callgetItem);
		return "";
	}
	env->DeleteLocalRef(itemClass);

	jstring registryNameString = (jstring)env->CallObjectMethod(callgetItem, getRegistryNameMethodId);
	if (CheckJNIException(env) || !registryNameString) {
		env->DeleteLocalRef(callgetItem);
		return "";
	}
	env->DeleteLocalRef(callgetItem);

	const char* heldItemName = env->GetStringUTFChars(registryNameString, nullptr);
	std::string result(heldItemName);
	env->ReleaseStringUTFChars(registryNameString, heldItemName);
	env->DeleteLocalRef(registryNameString);
	env->DeleteLocalRef(entityPlayerClass);
	env->DeleteGlobalRef(playerInstance);

	return result;
}

std::string getUsername(JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	jobject mcInstance = theMinecraft(env, map);
	if (CheckJNIException(env) || !mcInstance) return "";

	jclass MinecraftClass = env->GetObjectClass(mcInstance);
	if (CheckJNIException(env) || !MinecraftClass) {
		env->DeleteGlobalRef(mcInstance);
		return "";
	}

	jfieldID sessionFieldId = env->GetFieldID(MinecraftClass, "session", "Lnet/minecraft/util/Session;");
	if (CheckJNIException(env) || !sessionFieldId) {
		env->DeleteLocalRef(MinecraftClass);
		env->DeleteGlobalRef(mcInstance);
		return "";
	}

	jobject sessionInstance = env->GetObjectField(mcInstance, sessionFieldId);
	if (CheckJNIException(env) || !sessionInstance) {
		env->DeleteLocalRef(MinecraftClass);
		env->DeleteGlobalRef(mcInstance);
		return "";
	}
	env->DeleteLocalRef(MinecraftClass);

	jclass SessionClass = env->GetObjectClass(sessionInstance);
	if (CheckJNIException(env) || !SessionClass) {
		env->DeleteLocalRef(sessionInstance);
		env->DeleteGlobalRef(mcInstance);
		return "";
	}

	jfieldID usernameFieldId = env->GetFieldID(SessionClass, "username", "Ljava/lang/String;");
	if (CheckJNIException(env) || !usernameFieldId) {
		env->DeleteLocalRef(SessionClass);
		env->DeleteLocalRef(sessionInstance);
		env->DeleteGlobalRef(mcInstance);
		return "error";
	}
	env->DeleteLocalRef(SessionClass);

	jstring usernameJString = (jstring)env->GetObjectField(sessionInstance, usernameFieldId);
	if (CheckJNIException(env) || !usernameJString) {
		env->DeleteLocalRef(sessionInstance);
		env->DeleteGlobalRef(mcInstance);
		return "error";
	}

	const char* usernameChars = env->GetStringUTFChars(usernameJString, nullptr);
	if (CheckJNIException(env) || !usernameChars) {
		env->DeleteLocalRef(usernameJString);
		env->DeleteLocalRef(sessionInstance);
		env->DeleteGlobalRef(mcInstance);
		return "error";
	}

	std::string result(usernameChars);
	env->ReleaseStringUTFChars(usernameJString, usernameChars);
	env->DeleteLocalRef(usernameJString);
	env->DeleteLocalRef(sessionInstance);
	env->DeleteGlobalRef(mcInstance);

	return result;

}

