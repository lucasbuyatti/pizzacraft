#include "winsock_hook.h"

// Puntero a la funcion
PVOID WSASend_Pointer = nullptr;

// Definición de puntero a la función original
WSASend_Call original_WSASend = nullptr;

// Estado del hook
extern MH_STATUS status = MH_UNKNOWN;

MH_STATUS winsock_hook::Hook_init() {

    if (status == MH_OK) {
        return MH_OK;
    }

    // Intentar inicializar MinHook
    status = MH_Initialize();
    if (status != MH_OK) {
        delog("MH_Initialize");
        return status;
    }

    // Obtener el handle del módulo ws2_32
    HMODULE ws2_32 = GetModuleHandleA("ws2_32.dll");
    if (ws2_32 == NULL) {
        delog("GetModuleHandleA");
        return MH_ERROR_MODULE_NOT_FOUND;
    }

    // Obtener la dirección de la función WSASend
    WSASend_Pointer = GetProcAddress(ws2_32, "WSASend");
    if (WSASend_Pointer == NULL) {
        delog("GetProcAddress");
        return MH_ERROR_FUNCTION_NOT_FOUND;
    }

    // Crear el hook
    status = MH_CreateHook(WSASend_Pointer, &WSASendHook, (LPVOID*)&original_WSASend);
    if (status != MH_OK) {
        delog("MH_CreateHook");
        return status;
    }

    // Habilitar el hook
    status = MH_EnableHook(WSASend_Pointer);
    if (status != MH_OK) {
        delog("MH_EnableHook");
        return status;
    }

    return status;
}

void winsock_hook::Hook_shutdown() {
    if (status != MH_OK)
        return;

    MH_DisableHook(WSASend_Pointer);
    MH_RemoveHook(WSASend_Pointer);

    dlog("Hook removido");

    status = MH_UNKNOWN;
}

int WSAAPI WSASendHook(
    SOCKET s,
    LPWSABUF lpBuffers,
    DWORD dwBufferCount,
    LPDWORD lpNumberOfBytesSent,
    DWORD dwFlags,
    LPWSAOVERLAPPED lpOverlapped,
    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
)
{
    // Si hay datos en lpBuffers, bloqueamos el envío
    if (lpBuffers != nullptr && lpBuffers->len > 0) {
        // Aquí bloqueamos el paquete, no llamamos a la función original
        dlog("Bloqueando paquete de %d bytes", lpBuffers->len);

        // Si quieres simular que el paquete se ha enviado correctamente pero no hacer nada, retorna 0
        return 0;  // Retorno exitoso pero no envía nada
    }

    // Si no hay datos o alguna otra condición, dejamos que pase la función original
    return original_WSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);
}
