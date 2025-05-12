#include "../modules.h"



void modules::velocity::vel() {

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

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(1, 100);

	while (true) {

		if (modules::config::cfg.vel_cfg.enable.load()) {

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

			static bool jumpDone = false;

			int currentHurt = player::hurtTime(env);
			float fallDistance = player::fallDistance(env);
			int chance = modules::config::cfg.vel_cfg.chance.load();

			if (!jumpDone && currentHurt >= 8 && fallDistance < 0.005f &&  dist(gen) <= chance) {

				INPUT input = { 0 };
				input.type = INPUT_KEYBOARD;
				input.ki.wVk = VK_SPACE;      
				input.ki.wScan = 0;           
				input.ki.dwFlags = 0;    
				input.ki.time = 0;
				input.ki.dwExtraInfo = 0;
				SendInput(1, &input, sizeof(INPUT));

				std::this_thread::sleep_for(std::chrono::milliseconds(25));

				input.ki.dwFlags = KEYEVENTF_KEYUP;
				SendInput(1, &input, sizeof(INPUT));
				jumpDone = true;
			}

			if (currentHurt == 0) {
				jumpDone = false;
			}


		}

		if (modules::config::cfg.des_cfg.destruct.load()) {
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}

}

void  modules::velocity::velUI() {

	bool enable = modules::config::cfg.vel_cfg.enable.load();
	int chance = modules::config::cfg.vel_cfg.chance.load();

	ImGui::Checkbox("Velocity", &enable);
	modules::config::cfg.vel_cfg.enable.store(enable);

	ImGui::Indent();

	ImGui::SliderInt("Chance", &chance, 0, 100);
	modules::config::cfg.vel_cfg.chance.store(chance);

	ImGui::Unindent();


}