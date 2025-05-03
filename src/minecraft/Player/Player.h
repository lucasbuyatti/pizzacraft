#pragma once
#include "../../../includes.h"

namespace player {
	Vec3<double> getPosition(JNIEnv* env);

	void setAngles(JNIEnv* env, float& yaw, float& pitch);

	void setRotation(JNIEnv* env, float yaw, float pitch);

	bool playerExist(JNIEnv* env);

	Rotation<float> getAngles(JNIEnv* env);

	std::string getHeldItem(JNIEnv* env);

	bool isSprinting(JNIEnv* env);

	bool onGround(JNIEnv* env);

	float fallDistance(JNIEnv* env);

	int hurtTime(JNIEnv* env);

	// nope
	void setJumping(JNIEnv* env);

	void jump(JNIEnv* env);
}

