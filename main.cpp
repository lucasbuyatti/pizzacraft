#include "includes.h"

extern JavaVM* jvm;

//void main_thread(HMODULE hModule) {
//
//	if (!Hook_init())
//	{
//		while (!(GetAsyncKeyState(VK_END) & 0x8000)) {
//			std::this_thread::sleep_for(std::chrono::milliseconds(25));
//		}
//
//		Hook_shutdown();
//		FreeLibraryAndExitThread(hModule, 0);
//	}
//
//}

void modules_thread(HMODULE hModule) {

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

	std::unordered_map<std::string, jclass> map;

	getLoadedClasses(env, jvmti, map);

	jni_cache::init_cache(env, map);

	for (auto& [key, value] : jni_cache::jclassCache) {
		printf("JCLASS: Key: %s | Value: %p", key.c_str(), value);
	}

	for (auto& [key, value] : jni_cache::fieldCache) {
		printf("FIELDID: Key: %s | Value: %p", key.c_str(), value);
	}

	for (auto& [key, value] : jni_cache::methodCache) {
		printf("METHODID: Key: %s | Value: %p", key.c_str(), value);
	}

	for (auto& [key, value] : jni_cache::jobjectCache) {
		printf("JOBJECT: Key: %s | Value: %p", key.c_str(), value);
	}

	modules::config::load_config();

	std::thread imgui([]() { RunImGuiApp(); });
	imgui.detach();

	std::thread aim_thread([]() { modules::aimbot::aim(); });

	aim_thread.detach(); // Nunca termina, por ende crash

	std::thread vel_thread([]() { modules::velocity::vel(); });

	vel_thread.detach();

	std::thread click_thread([]() { modules::autoclick::click(); });

	click_thread.detach();

	std::thread fplace_thread([]() { modules::fastplace::fplace(); });

	fplace_thread.detach();

	while (!(GetAsyncKeyState(VK_END) & 0x8000)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}

	jni_cache::delete_cache(env);

	FreeLibraryAndExitThread(hModule, 0);

}



void debug_thread(HMODULE hModule) {

	FILE* fDummy;

	if (AllocConsole()) {
		freopen_s(&fDummy, "CONOUT$", "w", stdout);  // Redirige stdout a la consola
		SetConsoleTitleA("Debug Console");
	}

	/*JNIEnv* env = nullptr;

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

	std::unordered_map<std::string, jclass> map;

	getLoadedClasses(env, jvmti, map);

	for (auto& [key, val] : map) {
		printf("Map: %s | %p\n", key.c_str(), val);
	}

	jni_cache::init_cache(env, map);

	for (auto& [key, val] : jni_cache::jclassCache) {
		printf("JCLASS cache: %s | %p\n", key.c_str(), val);
	}

	for (auto& [key, val] : jni_cache::fieldCache) {
		printf("FIELDID cache: %s | %p\n", key.c_str(), val);
	}

	for (auto& [key, val] : jni_cache::methodCache) {
		printf("METHOD cache: %s | %p\n", key.c_str(), val);
	}

	for (auto& [key, val] : jni_cache::jobjectCache) {
		printf("JOBJECT cache: %s | %p\n", key.c_str(), val);
	}

	std::vector<jobject> playerEntities;

	while (true) {

		world::entity::player::playerEntitiesManager(env, playerEntities);

		auto playerPos = world::entity::player::playersPos(env, playerEntities);

		int players = 0;
		for (auto& pos : playerPos) {

			printf("Player[%u]: %f | %f | %f\n", players, pos.x, pos.y, pos.z);
			players++;
		}


		if (GetAsyncKeyState(VK_END) & 0x8000) {
			std::this_thread::sleep_for(std::chrono::milliseconds(25));
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}


	jni_cache::delete_cache(env);*/

	while (!(GetAsyncKeyState(VK_END) & 0x8000)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(25)); // Evita que el hilo consuma demasiados recursos
	}

	FreeLibraryAndExitThread(hModule, 0);

}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {

		DisableThreadLibraryCalls(hModule);

		std::thread thread_debug(debug_thread, hModule);
		thread_debug.detach();

		//std::thread thread_main(main_thread, hModule);
		//thread_main.detach();

		std::thread thread_modules(modules_thread, hModule);
		thread_modules.detach();

		break;
	}
	case DLL_PROCESS_DETACH: {

		FreeConsole();
		jvm->DetachCurrentThread();

		break;
	}

	}
	return TRUE;
}