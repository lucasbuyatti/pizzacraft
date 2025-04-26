#pragma once
#include <iostream>
#include <Windows.h>
#include <thread>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <unordered_map>
#include <math.h>
#include  <algorithm>

// ImGui
#include "imgui/include/imgui.h"
#include "imgui/include/backends/imgui_impl_win32.h"
#include "imgui/include/backends/imgui_impl_opengl3.h"

// Hooks
#include "minhook/includes/include/MinHook.h"
#include "hooks/render_hook.h"

// jvm / jni / jvmti / helper
#include "jvm/include/jni.h"
#include "jvm/include/jvmti.h"
#include "jvm/jni_helper.h"

// Minecraft
#include "minecraft/Math/minecraft_math.h"
#include "minecraft/lunarforge189_helper.h"
#include "minecraft/Minecraft/Minecraft.h"
#include "minecraft/Player/Player.h"
#include "minecraft/World/World.h"
#include "minecraft/Entity/Entity.h"