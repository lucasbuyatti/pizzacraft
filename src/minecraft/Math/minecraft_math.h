#pragma once
#include "../../../includes.h"

template <typename T>
struct Vec3 {
	T x, y, z;

	Vec3<T> operator-(const Vec3<T>& other) const {
		return {
			x - other.x,
			y - other.y,
			z - other.z
		};
	};

};

template <typename T>
struct Vec2 {
	T x, y;
};

template <typename T>
struct Rotation {
	T yaw, pitch;

	void normalize() {
		while (yaw >= 180) yaw -= 360;
		while (yaw < -180) yaw += 360;
	}
};