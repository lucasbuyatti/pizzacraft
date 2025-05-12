#pragma once
#include "../../includes.h"

bool GUI_init(HWND wnd_handle);
void GUI_shutdown();
void GUI_draw();
bool GUI_getIsInit();
bool GUI_getDoDraw();
void GUI_setDoDraw(bool new_value);

// Hook-related declarations
typedef BOOL(__stdcall* TWglSwapBuffers) (HDC hDc);

static bool is_init_hook{};
bool Hook_init();
void Hook_shutdown();

bool Hook_getIsInit();
LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool __stdcall wglSwapBuffers_hook(HDC hDc);