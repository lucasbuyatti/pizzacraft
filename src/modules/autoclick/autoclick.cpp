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

			int cps = modules::config::cfg.click_cfg.cps.load();

			if (GetAsyncKeyState(modules::config::cfg.click_cfg.key.load()) & 0x8000 || minecraft::game_settings::keyBindAttack_isKeyDown(env)) {
				mean_ms = 1000.0 / cps;
				stddev_ms = mean_ms * 0.1;       
				std::normal_distribution<> dist(mean_ms, stddev_ms);

				int delay = max(1, static_cast<int>(dist(gen)));

				// DOWN
				INPUT inp = { 0 };
				inp.type = INPUT_MOUSE;
				inp.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
				SendInput(1, &inp, sizeof(inp));

				std::this_thread::sleep_for(std::chrono::milliseconds(delay / 2));
				ZeroMemory(&inp, sizeof(inp));

				// UP
				inp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
				SendInput(1, &inp, sizeof(inp));

				std::this_thread::sleep_for(std::chrono::milliseconds(delay / 2));
			}
			
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
	

}


void modules::autoclick::clickUI() {

	bool enable = modules::config::cfg.click_cfg.enable.load();
	int key = modules::config::cfg.click_cfg.key.load();
	int cps = modules::config::cfg.click_cfg.cps.load();

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

	ImGui::SliderInt("CPS", &cps, 1, 20);
	modules::config::cfg.click_cfg.cps.store(cps);

	ImGui::Unindent();

}