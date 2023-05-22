#include "Bullyware/Bullyware.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Bullyware::Initialize, hModule, 0, nullptr));
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    {
        Bullyware::Shutdown();
        break;
    }
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

