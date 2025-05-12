#pragma once
#include "../../includes.h"

// Definicion de alias para puntero a funcion de ws2_32.dll
typedef int(__stdcall* WSASend_Call)(
    SOCKET s,
    LPWSABUF lpBuffers,
    DWORD dwBufferCount,
    LPDWORD lpNumberOfBytesSent,
    DWORD dwFlags,
    LPWSAOVERLAPPED lpOverlapped,
    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
);

// Hook + Shutdown
namespace winsock_hook {

    static MH_STATUS status;

    MH_STATUS Hook_init();
    void Hook_shutdown();
}

// Funcion hookeada
int WSAAPI WSASendHook(
    SOCKET s,
    LPWSABUF lpBuffers,
    DWORD dwBufferCount,
    LPDWORD lpNumberOfBytesSent,
    DWORD dwFlags,
    LPWSAOVERLAPPED lpOverlapped,
    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
);
