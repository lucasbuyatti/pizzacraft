#include "../modules.h"

void modules::fastplace::fplace() {
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

		if (modules::config::cfg.fplace_cfg.enable.load()) {


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

			if (modules::config::cfg.fplace_cfg.blocks_projectiles.load() && !blocks::isUsingBlocks(env)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(25));
				continue;
			}

			if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
				minecraft::rightClickDelayTimer(env, modules::config::cfg.fplace_cfg.delay.load());
			}

		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void modules::fastplace::fplaceUI() {

	bool enable = modules::config::cfg.fplace_cfg.enable.load();
	int delay = modules::config::cfg.fplace_cfg.delay.load();
	bool only_blocks_and_projectiles = modules::config::cfg.fplace_cfg.blocks_projectiles.load();

	ImGui::Checkbox("Fastplace", &enable);
	modules::config::cfg.fplace_cfg.enable.store(enable);

	ImGui::Indent();

	ImGui::Checkbox("Only Blocks & Projectiles", &only_blocks_and_projectiles);
	modules::config::cfg.fplace_cfg.blocks_projectiles.store(only_blocks_and_projectiles);

	ImGui::SliderInt("Delay", &delay, 0, 5);
	modules::config::cfg.fplace_cfg.delay.store(delay);

	ImGui::Unindent();


}