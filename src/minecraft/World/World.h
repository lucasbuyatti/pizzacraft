#pragma once
#include "../../../includes.h"

namespace world {
	namespace entity {

		namespace player {
			void playerEntitiesManager(JNIEnv* env, std::vector<jobject>& playerEntities);

			std::vector<Vec3<double>> playersPos(JNIEnv* env, std::vector<jobject> playerEntities);
		}


	}

	bool worldExist(JNIEnv* env);

}

