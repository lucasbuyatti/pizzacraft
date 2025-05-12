#pragma warning(disable: 4244) // conversion from 'double' to 'float', possible loss of data
#include "../modules.h"

void modules::aimbot::aim() {
	JNIEnv* env = nullptr;

	jvmtiEnv* jvmti = nullptr;

	if (GetJVMInstance(env) != JNI_OK) {
		printf("GetJVMInstance error\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		return;
	}

	if (GetJVMTIInstance(jvmti) != JNI_OK) {
		printf("GetJVMTIInstance error\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		return;
	}

	if (!env) {
		printf("env error\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		return;
	}


	while (true) {

		std::vector<jobject> playerEntities;

		if (modules::config::cfg.aim_cfg.enable.load()) {

			if (!env) {
				std::this_thread::sleep_for(std::chrono::milliseconds(25));
				continue;
			}

			if (!world::worldExist(env)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(25));
				continue;
			}

			if (!player::playerExist(env)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(25));
				continue;
			}

			if (!minecraft::inGameHasFocus(env)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(25));
				continue;
			}

			world::entity::player::playerEntitiesManager(env, playerEntities);

			if (playerEntities.empty()) {
				std::this_thread::sleep_for(std::chrono::milliseconds(25));
				continue;
			}

			Vec3<double> me = player::getPosition(env);
			Rotation<float> myangles = player::getAngles(env);

			std::vector<Vec3<double>> target = world::entity::player::playersPos(env, playerEntities);

			for (size_t i = 0; i < min(playerEntities.size(), target.size()); i++) {

				Vec3 target_pos = Vec3{ target[i].x, target[i].y,target[i].z };

				Vec3 direction = target_pos - me;

				if (direction.x == 0 && direction.y == 0 && direction.z == 0) continue;

				float maxDist = modules::config::cfg.aim_cfg.distance.load();

				float distSq = direction.x * direction.x
					+ direction.y * direction.y
					+ direction.z * direction.z;

				float maxDistSq = maxDist * maxDist;

				if (distSq > maxDistSq) {
					continue;
				}

				double targetYaw = atan2(direction.z, direction.x) * (180.0f / M_PI) - 90;
				double targetPitch = -atan2(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z)) * (180.0 / M_PI);


				double deltaYaw = targetYaw - myangles.yaw;
				double deltaPitch = targetPitch - myangles.pitch;

				// normalize the deltaYaw
				while (deltaYaw > 180) deltaYaw -= 360;
				while (deltaYaw < -180) deltaYaw += 360;

				float fov = modules::config::cfg.aim_cfg.fov.load();
				float fovToTarget = sqrt(deltaYaw * deltaYaw + deltaPitch * deltaPitch);

				if (modules::config::cfg.aim_cfg.ignore_targeted.load()) {
					if (minecraft::typeOfHit(env) == "ENTITY") continue;
				}

				if (GetAsyncKeyState(modules::config::cfg.aim_cfg.key.load()) & 0x8000 &&
					fovToTarget < fov) {

					const float yawEpsilon = 1.5;
					if (std::abs(deltaYaw) <= yawEpsilon)
						break;

					const float smoothing = modules::config::cfg.aim_cfg.smooth.load();
					const float speed = modules::config::cfg.aim_cfg.speed.load();

					float step = (static_cast<float>(deltaYaw) * smoothing) * speed;

					float newYaw = myangles.yaw + step;

					player::setRotation(env, newYaw, myangles.pitch);
					break;
				}

			}

			

		}

		if (!playerEntities.empty()) {
			for (jobject o : playerEntities) env->DeleteLocalRef(o);
		}

		if (modules::config::cfg.des_cfg.destruct.load()) {
			break;
		}


		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}

}

void modules::aimbot::aimUI() {
	bool enable = modules::config::cfg.aim_cfg.enable.load();
	int key = modules::config::cfg.aim_cfg.key.load();
	float fov = modules::config::cfg.aim_cfg.fov.load();
	float smooth = modules::config::cfg.aim_cfg.smooth.load();
	float speed = modules::config::cfg.aim_cfg.speed.load();
	float distance = modules::config::cfg.aim_cfg.distance.load();
	bool ignore_targeted = modules::config::cfg.aim_cfg.ignore_targeted.load();

	const char* items[] = { "LEFT CLICK", "RIGHT CLICK", "X CLICK 1", "X CLICK 2", "MIDDLE CLICK"};
	static int actual = key;

	ImGui::Checkbox("Aimbot", &enable);
	modules::config::cfg.aim_cfg.enable.store(enable);

	ImGui::SameLine();

	ImGui::Combo("Button##aim", &actual, items, IM_ARRAYSIZE(items));
	switch (actual) {
	case 0: {
		modules::config::cfg.aim_cfg.key.store(VK_LBUTTON);
		break;
	}
	case 1: {
		modules::config::cfg.aim_cfg.key.store(VK_RBUTTON);
		break;
	}
	case 2: {
		modules::config::cfg.aim_cfg.key.store(VK_XBUTTON1);
		break;
	}
	case 3: {
		modules::config::cfg.aim_cfg.key.store(VK_XBUTTON2);
		break;
	}
	case 4: {
		modules::config::cfg.aim_cfg.key.store(VK_MBUTTON);
		break;
	}
	default:
		modules::config::cfg.aim_cfg.key.store(VK_LBUTTON);
		break;
	}


	ImGui::Indent();

	ImGui::SliderFloat("Fov", &fov, 0.f, 180.f);
	modules::config::cfg.aim_cfg.fov.store(fov);

	ImGui::SliderFloat("Speed", &speed, 1.f, 10.f);
	modules::config::cfg.aim_cfg.speed.store(speed);

	ImGui::SliderFloat("Smooth", &smooth, 0.f, 2.f);
	modules::config::cfg.aim_cfg.smooth.store(smooth);

	ImGui::SliderFloat("Distance", &distance, 3.f, 10.f);
	modules::config::cfg.aim_cfg.distance.store(distance);

	ImGui::Checkbox("Ignore Targeted", &ignore_targeted);
	modules::config::cfg.aim_cfg.ignore_targeted.store(ignore_targeted);

	ImGui::Unindent();
}