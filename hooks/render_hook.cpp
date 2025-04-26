#include "render_hook.h"

extern bool is_init_gui{};
extern bool do_draw{false};

extern bool is_init_hook;
extern HWND wnd_handle;
extern WNDPROC origin_wndproc;
extern void* p_swap_buffers;
extern TWglSwapBuffers origin_wglSwapBuffers;

bool GUI_init(HWND wnd_handle)
{
    if (is_init_gui)
        return false;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(wnd_handle);
    ImGui_ImplOpenGL3_Init();

    is_init_gui = true;

    return false;
}

void GUI_shutdown()
{
    if (!is_init_gui)
        return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    is_init_gui = false;
}

void GUI_draw()
{
    if (!do_draw)
        return;


    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("OpenGL-Hk");
    {
        ImGui::Text("Hello, World!");
    }
    ImGui::End();
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool GUI_getIsInit()
{
    return is_init_gui;
}

bool GUI_getDoDraw()
{
    return do_draw;
}

void GUI_setDoDraw(bool new_value)
{
    do_draw = new_value;
}

typedef BOOL(__stdcall* TWglSwapBuffers) (HDC hDc);

static bool is_init_hook{};
static HWND wnd_handle{};
static WNDPROC origin_wndproc{};
void* p_swap_buffers{};
TWglSwapBuffers origin_wglSwapBuffers{};

LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool __stdcall wglSwapBuffers_hook(HDC hDc);

bool Hook_init()
{
    if (is_init_hook)
        return false;

    {
        wnd_handle = FindWindowA("LWJGL", nullptr);

        if (!wnd_handle)
        {
            wnd_handle = FindWindowA("GLFW30", nullptr);

            if (!wnd_handle)
            {
                return true;
            }
        }

        origin_wndproc = (WNDPROC)SetWindowLongPtrW(wnd_handle, GWLP_WNDPROC, (LONG_PTR)WndProc);
    }

    MH_Initialize();

    /* wglSwapBuffers */
    {
        p_swap_buffers = (void*)GetProcAddress(GetModuleHandleA("opengl32.dll"), "wglSwapBuffers");

        if (p_swap_buffers == nullptr)
        {
            return true;
        }

        MH_CreateHook(p_swap_buffers, &wglSwapBuffers_hook, (LPVOID*)&origin_wglSwapBuffers);
    }

    MH_EnableHook(MH_ALL_HOOKS);

    is_init_hook = true;

    return false;
}

void Hook_shutdown()
{
    if (!is_init_hook)
        return;

    SetWindowLongPtrW(wnd_handle, GWLP_WNDPROC, (LONG_PTR)origin_wndproc);
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);

    is_init_hook = false;
}

bool Hook_getIsInit()
{
    return is_init_hook;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (GUI_getIsInit())
    {
        /* If VK_INSERT, change menu draw state */
        if (msg == WM_KEYDOWN && wParam == VK_INSERT)
            GUI_setDoDraw(!GUI_getDoDraw());

        if (GUI_getDoDraw() && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;
    }

    return CallWindowProcA(origin_wndproc, hWnd, msg, wParam, lParam);
}

bool __stdcall wglSwapBuffers_hook(HDC hDc)
{
    HGLRC origin_context{ wglGetCurrentContext() };
    static HGLRC new_context{};

    if (static bool was_init{}; was_init == false)
    {
        new_context = wglCreateContext(hDc);
        wglMakeCurrent(hDc, new_context);

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glViewport(0, 0, viewport[2], viewport[3]);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_DEPTH_TEST);

        GUI_init(wnd_handle);

        was_init = true;
    }
    else
    {
        wglMakeCurrent(hDc, new_context);
        GUI_draw();
    }

    wglMakeCurrent(hDc, origin_context);

    return origin_wglSwapBuffers(hDc);
}