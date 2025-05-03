#pragma once
#include "../../../includes.h"

namespace minecraft {
	int displayHeight(JNIEnv* env);

	int displayWidth(JNIEnv* env);

	bool inGameHasFocus(JNIEnv* env);

	void rightClickDelayTimer(JNIEnv* env, int delay);

	namespace game_settings {
		bool keyBindAttack_isKeyDown(JNIEnv* env);
	}
}

