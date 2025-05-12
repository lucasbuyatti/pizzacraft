#include "../modules.h"

void modules::autoclick::click() {
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
	double mean_ms, stddev_ms;

	while (true) {
		if (modules::config::cfg.click_cfg.enable.load()) {

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

			if (modules::config::cfg.click_cfg.ignore_blocks.load()) {
				if (minecraft::typeOfHit(env) == "BLOCK") {
					std::this_thread::sleep_for(std::chrono::milliseconds(25));
					continue;
				}
			}

			if (GetAsyncKeyState(modules::config::cfg.click_cfg.key.load()) & 0x8000) {
			
				int base_cps = modules::config::cfg.click_cfg.cps.load();
				int cps_jitter = std::uniform_int_distribution<>(-3, 2)(gen);
				int cps = std::clamp(base_cps + cps_jitter, 5, 20);

				double mean_ms = 1000.0 / static_cast<double>(cps);
				std::lognormal_distribution<> delay_dist(std::log(mean_ms), 0.25);
				int delay = static_cast<int>(std::clamp(delay_dist(gen), mean_ms * 0.5, mean_ms * 1.8));

				// DOWN
				INPUT inp = { 0 };
				inp.type = INPUT_MOUSE;
				inp.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
				SendInput(1, &inp, sizeof(inp));

				// Down duration con jitter
				int down_jitter = std::uniform_int_distribution<>(8, 16)(gen);
				std::this_thread::sleep_for(std::chrono::milliseconds(delay / 4 + down_jitter));

				// UP
				ZeroMemory(&inp, sizeof(inp));
				inp.type = INPUT_MOUSE;
				inp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
				SendInput(1, &inp, sizeof(inp));

				// Up duration con jitter
				int up_jitter = std::uniform_int_distribution<>(5, 14)(gen);
				std::this_thread::sleep_for(std::chrono::milliseconds(delay / 2 + up_jitter));

				// Pausa entre clicks (human-like)
				int rest_jitter = std::uniform_int_distribution<>(3, 10)(gen);
				std::this_thread::sleep_for(std::chrono::milliseconds(rest_jitter));
			}
			
		}

		if (modules::config::cfg.des_cfg.destruct.load()) {
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
}


void modules::autoclick::clickUI() {

	bool enable = modules::config::cfg.click_cfg.enable.load();
	int key = modules::config::cfg.click_cfg.key.load();
	int cps = modules::config::cfg.click_cfg.cps.load();
	bool ignore_blocks = modules::config::cfg.click_cfg.ignore_blocks.load();

	const char* items[] = { "LEFT CLICK", "RIGHT CLICK", "X CLICK 1", "X CLICK 2", "MIDDLE CLICK" };
	static int actual = key;

	ImGui::Checkbox("Autoclcik", &enable);
	modules::config::cfg.click_cfg.enable.store(enable);

	ImGui::SameLine();

	ImGui::Combo("Button##click", &actual, items, IM_ARRAYSIZE(items));
	switch (actual) {
	case 0: {
		modules::config::cfg.click_cfg.key.store(VK_LBUTTON);
		break;
	}
	case 1: {
		modules::config::cfg.click_cfg.key.store(VK_RBUTTON);
		break;
	}
	case 2: {
		modules::config::cfg.click_cfg.key.store(VK_XBUTTON1);
		break;
	}
	case 3: {
		modules::config::cfg.click_cfg.key.store(VK_XBUTTON2);
		break;
	}
	case 4: {
		modules::config::cfg.click_cfg.key.store(VK_MBUTTON);
		break;
	}
	default:
		modules::config::cfg.click_cfg.key.store(VK_XBUTTON1);
		break;
	}
	
	ImGui::Indent();

	ImGui::Checkbox("Ignore Blocks", &ignore_blocks);
	modules::config::cfg.click_cfg.ignore_blocks.store(ignore_blocks);

	ImGui::SliderInt("CPS", &cps, 1, 20);
	modules::config::cfg.click_cfg.cps.store(cps);

	ImGui::Unindent();

}