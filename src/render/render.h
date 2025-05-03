#pragma once
#include "../../includes.h"

// Estructura para datos de la ventana
struct WGL_WindowData {
    HDC hDC;
};



// Funciones públicas
int RunImGuiApp();  // Ejecuta el main loop de ImGui

// Helpers
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data);
void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data);
LRESULT WINAPI WndProc_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
