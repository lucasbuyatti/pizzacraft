#pragma once
#include "../../includes.h"

jobject thePlayer(JNIEnv* env, std::unordered_map<std::string, jclass> map);

Rotate<float> getAngles(JNIEnv* env, std::unordered_map<std::string, jclass> map);

Rotate<float> getPrevAngles(JNIEnv* env, std::unordered_map<std::string, jclass> map);

void setPrevAngles(float& yaw, float& pitch, JNIEnv* env, std::unordered_map<std::string, jclass> map);

void setAngles(float& yaw, float& pitch, JNIEnv* env, std::unordered_map<std::string, jclass> map);

Vec3<double> getPosition(JNIEnv* env, std::unordered_map<std::string, jclass> map);

bool onGround(JNIEnv* env, std::unordered_map<std::string, jclass> map);

bool isAirBorne(JNIEnv* env, std::unordered_map<std::string, jclass> map);

bool isSprinting(JNIEnv* env, std::unordered_map<std::string, jclass> map);

int hurtTime(JNIEnv* env, std::unordered_map<std::string, jclass> map);

std::string typeOfHit(JNIEnv* env, std::unordered_map<std::string, jclass> map);

std::string getHeldItemName(JNIEnv* env, std::unordered_map<std::string, jclass> map);

std::string getUsername(JNIEnv* env, std::unordered_map<std::string, jclass> map);