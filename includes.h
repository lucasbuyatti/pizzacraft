#pragma once
#include <iostream>
#include <winsock2.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <thread>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <unordered_map>
#define _USE_MATH_DEFINES
#include <math.h>
#include  <algorithm>
#include <random>
#include <chrono>
#include <mutex>
#include <fstream>
#include <windowsx.h>
#include <dwmapi.h>
#include <numbers>

#include "src/json/json.hpp"
#include "src/json/json_fwd.hpp"

// jvm / jni / jvmti / helper
#include "src/jvm/include/jni.h"
#include "src/jvm/include/jvmti.h"
#include "src/jvm/jni_helper.h" 

// ImGui
#include "src/imgui/include/imgui.h"
#include "src/imgui/include/backends/imgui_impl_win32.h"
#include "src/imgui/include/backends/imgui_impl_opengl3.h"

// Hooks
#include "src/minhook/includes/include/MinHook.h"
#include "src/hooks/render_hook.h"
#include "src/hooks/winsock_hook.h"

// Minecraft
#include "src/minecraft/Math/minecraft_math.h"
#include "src/minecraft/lunarforge189_helper.h"
#include "src/minecraft/Minecraft/Minecraft.h"
#include "src/minecraft/Player/Player.h"
#include "src/minecraft/World/World.h"
#include "src/minecraft/Blocks/blocks.h"

// Modules
#include "src/modules/modules.h"

// Render
#include "src/render/render.h"

#define GET_CURRENT_TIME() ([]() { \
    time_t now = time(nullptr); \
    struct tm tstruct; \
    static char buf[80]; \
    localtime_s(&tstruct, &now); \
    strftime(buf, sizeof(buf), "%X", &tstruct); \
    return buf; \
}())

// inline log
#define inlog(...) do { \
    printf(__VA_ARGS__); \
} while(0)

// break line log
#define flog(...) do { \
    printf("[+][%s][%s:%d] ", GET_CURRENT_TIME(), __FUNCTION__, __LINE__); \
    printf(__VA_ARGS__); \
    printf("\n"); \
} while(0)

// error log
#define elog(...) do { \
    fprintf(stderr, "[-][%s][%s:%d] ", GET_CURRENT_TIME(), __FUNCTION__, __LINE__); \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n"); \
} while(0)

// Debug log for OutputDebugStringA
#define dlog(...) do { \
    char buffer[1024]; \
    snprintf(buffer, sizeof(buffer), "[+][%s][%s:%d] ", GET_CURRENT_TIME(), __FUNCTION__, __LINE__); \
    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), __VA_ARGS__); \
    OutputDebugStringA(buffer); \
} while(0)

// Error log for OutputDebugStringA
#define delog(...) do { \
    char buffer[1024]; \
    snprintf(buffer, sizeof(buffer), "[-][%s][%s:%d] ", GET_CURRENT_TIME(), __FUNCTION__, __LINE__); \
    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), __VA_ARGS__); \
    OutputDebugStringA(buffer); \
} while(0)

