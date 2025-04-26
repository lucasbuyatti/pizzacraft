#include "includes.h"

extern JavaVM* jvm;
extern jvmtiEnv* jvmti;

void main_thread(HMODULE hModule) {

	if (!Hook_init())
	{
		while (!(GetAsyncKeyState(VK_END) & 0x8000)) {
			std::this_thread::sleep_for(std::chrono::milliseconds(25));
		}

		Hook_shutdown();

		FreeLibraryAndExitThread(hModule, 0);
	}
	FreeLibraryAndExitThread(hModule, 0);
}

void modules_thread(HMODULE hModule) {


	FreeLibraryAndExitThread(hModule, 0);
}


// Cerrar todos los whiles antes de sacar el dll del proceso
void debug_thread(HMODULE hModule) {
	FILE* fDummy;
	if (AllocConsole()) {
		freopen_s(&fDummy, "CONOUT$", "w", stdout);  // Redirige stdout a la consola
		freopen_s(&fDummy, "CONOUT$", "w", stderr);  // Redirige stderr a la consola
		freopen_s(&fDummy, "CONIN$", "r", stdin);    // Redirige stdin a la consola
		SetConsoleTitleA("Debug Console");
	}


	JNIEnv* env = nullptr;

	GetJVMInstance(env);
	GetJVMTIInstance();

	std::unordered_map<std::string, jclass> map{ 0 };

	getLoadedClasses(env, map);

	std::vector<jobject> playerEntities;

	while (true) {

		playerEntitiesManager(playerEntities, env, map);

		for (int i = 0; i < playerEntities.size(); i++) {
			printf("Player[%u] %p", i, playerEntities.at(i));
		}

		if (GetAsyncKeyState(VK_END) & 0x8000) break;


		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	FreeLibraryAndExitThread(hModule, 0);

}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {
		std::thread thread_main(main_thread, hModule);
		std::thread thread_modules(modules_thread, hModule);
		std::thread thread_debug(debug_thread, hModule);

		thread_main.detach();
		thread_modules.detach();
		thread_debug.detach();


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