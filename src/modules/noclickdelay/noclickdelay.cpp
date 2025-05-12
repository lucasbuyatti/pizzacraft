#include "../modules.h"


void modules::noclickdelay::ncd() {
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

		if (modules::config::cfg.ncd_cfg.enable.load()) {


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


			minecraft::leftClickCounter(env, 0);

		}

		if (modules::config::cfg.des_cfg.destruct.load()) {
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void modules::noclickdelay::ncdUI() {
	bool enable = modules::config::cfg.ncd_cfg.enable.load();

	ImGui::Checkbox("NoClickDelay", &enable);
	modules::config::cfg.ncd_cfg.enable.store(enable);

}