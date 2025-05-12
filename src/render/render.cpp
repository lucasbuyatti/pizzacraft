#include "render.h"

// Data global
static HGLRC g_hRC = nullptr;
static WGL_WindowData g_MainWindow;
static int g_Width = 600;
static int g_Height = 420;

int RunImGuiApp()
{
    WNDCLASSEXW wc = { sizeof(wc), CS_OWNDC, WndProc_, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGuiAppClass", nullptr };
    if (!::RegisterClassExW(&wc)) {
        printf("Failed to register window class\n");
        return 1;
    }

    HWND hwnd = CreateWindowExW(
        NULL,
        L"ImGuiAppClass",       // nombre de la clase registrada
        L"",                    // sin título
        WS_POPUP,               // sin bordes ni barra de título
        100, 100,               // posición inicial
        g_Width, g_Height,      // tamaño
        nullptr, nullptr,       // sin ventana padre, sin menú
        GetModuleHandle(nullptr),
        nullptr
    );

    if (!hwnd) {
        printf("Failed to create window\n");
        return 1;
    }

    if (!CreateDeviceWGL(hwnd, &g_MainWindow)) {
        printf("CreateDeviceWGL failed\n");
        CleanupDeviceWGL(hwnd, &g_MainWindow);
        ::DestroyWindow(hwnd);
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    if (!wglMakeCurrent(g_MainWindow.hDC, g_hRC)) {
        printf("wglMakeCurrent failed\n");
        CleanupDeviceWGL(hwnd, &g_MainWindow);
        ::DestroyWindow(hwnd);
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_InitForOpenGL(hwnd);
    ImGui_ImplOpenGL3_Init();

    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }

        if (modules::config::cfg.des_cfg.destruct.load()) break;

        if (done) break;

        if (::IsIconic(hwnd)) {
            ::Sleep(10);
            continue;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("(^_^)");
        {
            modules::reach::reachUI();
            ImGui::Separator();
            modules::aimbot::aimUI();
            ImGui::Separator();
            modules::autoclick::clickUI();
            ImGui::Separator();
            modules::velocity::velUI();
            ImGui::Separator();
            modules::fastplace::fplaceUI();
            ImGui::Separator();
            modules::noclickdelay::ncdUI();
            ImGui::Separator();
            if (ImGui::CollapsingHeader("Other")) {
                modules::config::configUI();
                ImGui::SameLine();
                modules::destruct::destructUI();
            }
        }
        ImGui::End();

        ImGui::Render();
        glViewport(0, 0, g_Width, g_Height);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ::SwapBuffers(g_MainWindow.hDC);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceWGL(hwnd, &g_MainWindow);
    if (g_hRC) {
        wglDeleteContext(g_hRC);
        g_hRC = nullptr;
    }
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    return 0;
}

bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    HDC hDc = ::GetDC(hWnd);
    if (!hDc) return false;

    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    int pf = ::ChoosePixelFormat(hDc, &pfd);
    if (pf == 0) return false;
    if (!::SetPixelFormat(hDc, pf, &pfd)) return false;
    ::ReleaseDC(hWnd, hDc);

    data->hDC = ::GetDC(hWnd);
    if (!data->hDC) return false;

    g_hRC = wglCreateContext(data->hDC);
    if (!g_hRC) return false;

    return true;
}

void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    if (data->hDC) {
        wglMakeCurrent(nullptr, nullptr);
        ::ReleaseDC(hWnd, data->hDC);
        data->hDC = nullptr;
    }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            g_Width = LOWORD(lParam);
            g_Height = HIWORD(lParam);
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}