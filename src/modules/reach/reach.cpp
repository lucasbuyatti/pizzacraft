#pragma warning(disable: 4244) // Possible loss of data (float - double)
#include "../modules.h"

void modules::reach::reach() {
	JNIEnv* env = nullptr;
	jvmtiEnv* jvmti = nullptr;

	if (GetJVMInstance(env) != JNI_OK || !env) {
		printf("GetJVMInstance error\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		return;
	}

	if (GetJVMTIInstance(jvmti) != JNI_OK || !jvmti) {
		printf("GetJVMTIInstance error\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		return;
	}


	std::random_device rd;
	std::mt19937 gen(rd());

	std::vector<jobject> playerEntities;
	std::vector<Vec3<double>> targetPositions;
	std::vector<int> targetHurtResistantTime;

	while (true) {
		if (modules::config::cfg.reach_cfg.enable.load()) {
			
			if (!env ||
				!world::worldExist(env) ||
				!player::playerExist(env) ||
				!minecraft::inGameHasFocus(env) ||
				player::hurtTime(env) > 0) {
				std::this_thread::sleep_for(std::chrono::milliseconds(25));
				continue;
			}

			world::entity::player::playerEntitiesManager(env, playerEntities);

			if (playerEntities.empty()) {
				std::this_thread::sleep_for(std::chrono::milliseconds(25));
				continue;
			}

			static int lastTick = player::ticksExisted(env);
			int currentTick = player::ticksExisted(env);

			jobject localPlayer = player::thePlayer(env);

			Vec3<double> me = player::getPosition(env);
			Rotation<float> myangles = player::getAngles(env);

			targetPositions = world::entity::player::playersPos(env, playerEntities);
			targetHurtResistantTime = world::entity::player::hurtResistantTime(env, playerEntities);

			size_t minSize = min(playerEntities.size(), targetPositions.size());

			for (size_t i = 0; i < min(minSize, targetHurtResistantTime.size()); ++i) {

				// Check if the entity is the local player
				if (env->IsSameObject(playerEntities[i], localPlayer)) {
					continue;
				}

				std::bernoulli_distribution skipAttack(modules::config::cfg.reach_cfg.skip_chance.load());
				if (skipAttack(gen)) continue;

				if (targetHurtResistantTime[i] != 0) continue;

				// Position of the entity
				const Vec3<double>& targetPos = targetPositions[i];
				// Direction to the entity
				Vec3 direction = targetPos - me;

				// Calculate the distance squared
				float distSq = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;

				// Min - Max distance
				float minDist = modules::config::cfg.reach_cfg.min_distance.load();
				float maxDist = modules::config::cfg.reach_cfg.max_distance.load();

				// Randomize the min and max distance
				std::uniform_real_distribution<> minDistDist(minDist - 0.2f, minDist + 0.2f);
				std::uniform_real_distribution<> maxDistDist(maxDist - 0.3f, maxDist + 0.3f);

				float valueMinDist = minDistDist(gen);
				float valueMaxDist = maxDistDist(gen);

				// Check if the distance is within the range
				if (distSq < valueMinDist * valueMinDist || distSq > valueMaxDist * valueMaxDist) {
					continue;
				}

				double targetYaw = atan2(direction.z, direction.x) * (180.0f / M_PI) - 90;
				double targetPitch = -atan2(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z)) * (180.0 / M_PI);

				double deltaYaw = targetYaw - myangles.yaw;
				double deltaPitch = targetPitch - myangles.pitch;

				// Normalize deltaYaw and deltaPitch
				while (deltaYaw > 180) deltaYaw -= 360;
				while (deltaYaw < -180) deltaYaw += 360;


				std::uniform_real_distribution<> fovDist(5.0f, 12.f);
				float fovThreshold = fovDist(gen);

				float fovToTarget = sqrt(deltaYaw * deltaYaw + deltaPitch * deltaPitch);

				static auto lastAttackTime = std::chrono::high_resolution_clock::now();

				if (minecraft::game_settings::keyBindAttack_isKeyDown(env) &&
					minecraft::game_settings::keyBindForward_isKeyDown(env) && fovToTarget < fovThreshold) {

					auto now = std::chrono::high_resolution_clock::now();
					auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastAttackTime).count();

					std::uniform_int_distribution<> attackDelay(150, 500);

					if (elapsed >= attackDelay(gen)) {
		
						std::uniform_int_distribution<> tickDist(1, 3);

						if (currentTick - lastTick >= tickDist(gen)) {
							player::attackEntity(localPlayer, playerEntities[i], env);
							lastTick = currentTick;
						}

						lastAttackTime = now;
					}
					break;
				}
			}


			for (jobject entity : playerEntities) {
				if (entity) env->DeleteLocalRef(entity);
			}
			playerEntities.clear();

			if (localPlayer) env->DeleteLocalRef(localPlayer);

			if (modules::config::cfg.des_cfg.destruct.load()) break;

			std::this_thread::sleep_for(std::chrono::milliseconds(25));


		}


	}
}



void modules::reach::reachUI() {
	bool enable = modules::config::cfg.reach_cfg.enable.load();
	float min_distance = modules::config::cfg.reach_cfg.min_distance.load();
	float max_distance = modules::config::cfg.reach_cfg.max_distance.load();
	float skip_chance = modules::config::cfg.reach_cfg.skip_chance.load();


	float distance[] = { min_distance, max_distance };


	ImGui::Checkbox("Reach", &enable);
	modules::config::cfg.reach_cfg.enable.store(enable);

	ImGui::Indent();

	ImGui::SliderFloat2("Distance", distance, 1.f, 6.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoRoundToFormat);
	
	if (distance[0] > distance[1]) {
		distance[0] = distance[1];
	}
	modules::config::cfg.reach_cfg.min_distance.store(distance[0]);
	modules::config::cfg.reach_cfg.max_distance.store(distance[1]);

	ImGui::SliderFloat("Skip chance", &skip_chance, 0.0f, 1.f, "%.2f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoRoundToFormat);
	modules::config::cfg.reach_cfg.skip_chance.store(skip_chance);

	ImGui::Unindent();


}