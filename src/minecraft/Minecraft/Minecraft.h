#pragma once
#include "../../../includes.h"

namespace minecraft {
	int displayHeight(JNIEnv* env);

	int displayWidth(JNIEnv* env);

	bool inGameHasFocus(JNIEnv* env);

	void rightClickDelayTimer(JNIEnv* env, int delay);

	std::string typeOfHit(JNIEnv* env);

	void leftClickCounter(JNIEnv* env, int value);

	jobject pointedEntity(JNIEnv* env);

	namespace game_settings {
		bool keyBindAttack_isKeyDown(JNIEnv* env);

		bool keyBindAttack_isPressed(JNIEnv* env);

		bool keyBindForward_isKeyDown(JNIEnv* env);
	}

	namespace packets {
	
		void sendPacket(JNIEnv* env, jobject packet);



	}
}

