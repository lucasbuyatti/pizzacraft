#include "lunarforge189_helper.h"


void getLoadedClasses(JNIEnv* env, jvmtiEnv* jvmti, std::unordered_map<std::string, jclass>& map) {

	jclass lang = env->FindClass("java/lang/Class");
	if (CheckJNIException(env) || !lang) return;
	jmethodID getName = env->GetMethodID(lang, "getName", "()Ljava/lang/String;");
	if (CheckJNIException(env) || !getName) return;

	jclass* classes;
	jint amount;

	jvmtiError getLoadedClasses = jvmti->GetLoadedClasses(&amount, &classes);
	if (CheckJNIException(env) || getLoadedClasses != 0) return;

	const char* onlymc = "net.minecraft";

	for (int i = 0; i < amount; i++) {
		jstring className = (jstring)env->CallObjectMethod(classes[i], getName);

		const char* classNameStr = env->GetStringUTFChars(className, nullptr);

		if (strncmp(classNameStr, onlymc, strlen(onlymc)) == 0) {
			map.emplace(classNameStr, (jclass)env->NewGlobalRef(classes[i]));
			
		}

		env->ReleaseStringUTFChars(className, classNameStr);
		env->DeleteLocalRef(className);
	}

	env->DeleteLocalRef(lang);

	delete[] classes;
}

jclass getClass(const char* className, JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	jclass mcClass = nullptr;

	int tries = 0;
	while (mcClass == nullptr && tries < 3) {
		auto it = map.find(className);

		if (it != map.end()) {
			mcClass = it->second;
			break;
		}
		else {
			jclass tempmcClass = env->FindClass(className);
			if (tempmcClass != nullptr) {
				mcClass = (jclass)env->NewGlobalRef(tempmcClass);
				env->DeleteLocalRef(tempmcClass);
				break;
			}
		}
		tries++;
	}

	return mcClass;
}

void cacheClass(const char* className, JNIEnv* env, std::unordered_map<std::string, jclass> map) {
	jclass theClass = getClass(className, env, map);
	if (!CheckJNIException(env) && theClass) {
		jni_cache::jclassCache[className] = (jclass)env->NewGlobalRef(theClass);
		env->DeleteLocalRef(theClass);
	}
	else {
		printf("Error loading class: %s\n", className);
	}
}

void cacheStaticField(const char* className, const char* name, const char* signature, JNIEnv* env) {
	jfieldID theStaticField = env->GetStaticFieldID(jni_cache::jclassCache[className], name, signature);
	if (!CheckJNIException(env) && theStaticField) {
		jni_cache::fieldCache[name] = theStaticField;
	}
	else {
		printf("Error getting static field id: %s\n", name);
	}
}

void cacheField(const char* className, const char* name, const char* signature, JNIEnv* env) {
	jfieldID theField = env->GetFieldID(jni_cache::jclassCache[className], name, signature);
	if (!CheckJNIException(env) && theField) {
		jni_cache::fieldCache[name] = theField;
	}
	else {
		printf("Error getting field id: %s\n", name);
	}
}

void cacheMethod(const char* className, const char* name, const char* signature, JNIEnv* env) {
	jmethodID theMethod = env->GetMethodID(jni_cache::jclassCache[className], name, signature);
	if (!CheckJNIException(env) && theMethod) {
		jni_cache::methodCache[name] = theMethod;
	}
	else {
		printf("Error getting method id: %s\n", name);
	}
}

void cacheStaticObject(const char* className, const char* fieldName, JNIEnv* env) {
	jobject staticObject = env->GetStaticObjectField(
		jni_cache::jclassCache[className],
		jni_cache::fieldCache[fieldName]
	);
	if (!CheckJNIException(env) && staticObject) {
		jni_cache::jobjectCache[fieldName] = env->NewGlobalRef(staticObject);
		env->DeleteLocalRef(staticObject);
	}
	else {
		printf("Error getting static object field id: %s\n", fieldName);
	}
}

namespace jni_cache {

	std::unordered_map<std::string, jmethodID> methodCache{};
	std::unordered_map<std::string, jfieldID> fieldCache{};
	std::unordered_map<std::string, jclass> jclassCache{};
	std::unordered_map<std::string, jobject> jobjectCache{};

	void setCache(JNIEnv* env, std::unordered_map<std::string, jclass> map) {

		if (env != nullptr) {
			cacheClass("net.minecraft.client.Minecraft", env, map);
			cacheClass("net.minecraft.client.multiplayer.PlayerControllerMP", env, map);
			cacheClass("net.minecraft.world.World", env, map);
			cacheClass("net.minecraft.client.multiplayer.WorldClient", env, map);
			cacheClass("net.minecraft.entity.Entity", env, map);
			cacheClass("net.minecraft.client.entity.EntityPlayerSP", env, map);
			cacheClass("net.minecraft.client.entity.AbstractClientPlayer", env, map);
			cacheClass("net.minecraft.util.MovingObjectPosition", env, map);
			cacheClass("net.minecraft.client.settings.GameSettings", env, map);
			cacheClass("net.minecraft.client.multiplayer.ServerData", env, map);
			cacheClass("net.minecraft.entity.EntityLivingBase", env, map);
			cacheClass("net.minecraft.entity.player.EntityPlayer", env, map);
			cacheClass("net.minecraft.item.ItemStack", env, map);
			cacheClass("net.minecraft.item.Item", env, map);
			cacheClass("net.minecraft.util.MovementInput", env, map);
			cacheClass("net.minecraft.client.settings.KeyBinding", env, map);
			cacheClass("java/util/List", env, map);

			cacheStaticField("net.minecraft.client.Minecraft", "theMinecraft", "Lnet/minecraft/client/Minecraft;", env);
			cacheStaticField("net.minecraft.client.Minecraft", "debugFPS", "I", env);
			cacheField("net.minecraft.client.Minecraft", "theWorld", "Lnet/minecraft/client/multiplayer/WorldClient;", env);
			cacheField("net.minecraft.client.Minecraft", "thePlayer", "Lnet/minecraft/client/entity/EntityPlayerSP;", env);
			cacheField("net.minecraft.client.Minecraft", "pointedEntity", "Lnet/minecraft/entity/Entity;", env);
			cacheField("net.minecraft.client.Minecraft", "displayWidth", "I", env);
			cacheField("net.minecraft.client.Minecraft", "displayHeight", "I", env);
			cacheField("net.minecraft.client.Minecraft", "session", "Lnet/minecraft/util/Session;", env);
			cacheField("net.minecraft.client.Minecraft", "gameSettings", "Lnet/minecraft/client/settings/GameSettings;", env);
			cacheField("net.minecraft.client.Minecraft", "mouseHelper", "Lnet/minecraft/util/MouseHelper;", env);
			cacheField("net.minecraft.client.Minecraft", "rightClickDelayTimer", "I", env);
			cacheField("net.minecraft.client.Minecraft", "serverName", "Ljava/lang/String;", env);
			cacheField("net.minecraft.client.Minecraft", "serverPort", "I", env);
			cacheField("net.minecraft.client.Minecraft", "inGameHasFocus", "Z", env);

			cacheField("net.minecraft.entity.Entity", "posX", "D", env);
			cacheField("net.minecraft.entity.Entity", "posY", "D", env);
			cacheField("net.minecraft.entity.Entity", "posZ", "D", env);
			cacheField("net.minecraft.entity.Entity", "motionX", "D", env);
			cacheField("net.minecraft.entity.Entity", "motionY", "D", env);
			cacheField("net.minecraft.entity.Entity", "motionZ", "D", env);
			cacheField("net.minecraft.entity.Entity", "rotationYaw", "F", env);
			cacheField("net.minecraft.entity.Entity", "rotationPitch", "F", env);
			cacheField("net.minecraft.entity.Entity", "isDead", "Z", env);
			cacheField("net.minecraft.entity.Entity", "onGround", "Z", env); // dont work in lunar client
			cacheField("net.minecraft.entity.Entity", "fallDistance", "F", env);
			cacheField("net.minecraft.entity.Entity", "dimension", "I", env);

			cacheField("net.minecraft.client.entity.EntityPlayerSP", "movementInput", "Lnet/minecraft/util/MovementInput;", env);
			cacheField("net.minecraft.entity.EntityLivingBase", "hurtTime", "I", env);
			cacheField("net.minecraft.world.World", "playerEntities", "Ljava/util/List;", env);
			cacheField("net.minecraft.util.MovementInput", "jump", "Z", env);

			cacheField("net.minecraft.client.settings.GameSettings", "keyBindForward", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindLeft", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindBack", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindRight", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindJump", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindSneak", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindSprint", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindInventory", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindUseItem", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindDrop", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindAttack", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindPickBlock", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindChat", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindPlayerList", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindCommand", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindScreenshot", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindTogglePerspective", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindSmoothCamera", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindFullscreen", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindSpectatorOutlines", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindStreamStartStop", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindStreamPauseUnpause", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindStreamCommercials", "Lnet/minecraft/client/settings/KeyBinding;", env);
			cacheField("net.minecraft.client.settings.GameSettings", "keyBindStreamToggleMic", "Lnet/minecraft/client/settings/KeyBinding;", env);

			cacheMethod("net.minecraft.entity.Entity", "setAngles", "(FF)V", env); // dont work in lunar client
			cacheMethod("net.minecraft.entity.player.EntityPlayer", "getHeldItem", "()Lnet/minecraft/item/ItemStack;", env);
			cacheMethod("net.minecraft.item.ItemStack", "getItem", "()Lnet/minecraft/item/Item;", env);
			cacheMethod("net.minecraft.item.Item", "getRegistryName", "()Ljava/lang/String;", env);
			cacheMethod("net.minecraft.entity.Entity", "isSprinting", "()Z", env);
			cacheMethod("net.minecraft.entity.player.EntityPlayer", "jump", "()V", env);

			cacheMethod("net.minecraft.client.settings.KeyBinding", "isKeyDown", "()Z", env);
			cacheMethod("net.minecraft.client.settings.KeyBinding", "isPressed", "()Z", env);
			cacheMethod("net.minecraft.client.settings.KeyBinding", "getKeyCode", "()I", env);


			cacheStaticObject("net.minecraft.client.Minecraft", "theMinecraft", env);

		}

	}

	void init_cache(JNIEnv* env, std::unordered_map<std::string, jclass> map) {
		setCache(env, map);
	}

	void delete_cache(JNIEnv* env) {
		for (auto& [key, value] : jclassCache) {
			if (value != nullptr) {
				printf("JCLASS: %s | %p DELETED\n", key.c_str(), value);
				env->DeleteGlobalRef(value);
			}
		}

		for (auto& [key, value] : jobjectCache) {
			printf("JOBJECT: %s | %p DELETED\n", key.c_str(), value);
			env->DeleteGlobalRef(value);
		}

		jclassCache.clear();
		fieldCache.clear();
		methodCache.clear();
		jobjectCache.clear();
	}
}