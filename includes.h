#pragma once
#include <iostream>
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



