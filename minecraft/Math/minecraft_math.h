#pragma once
#include "../../includes.h"

template <typename T>
struct Vec3 {
	T x, y, z;
};

template <typename T>
struct Vec2 {
	T x, y;
};

template <typename T>
struct Rotate {
	T yaw, pitch;

	void normalize() {
		while (yaw >= 180) yaw -= 360;
		while (yaw < -180) yaw += 360;
	}
};