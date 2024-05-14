#include "GF2Hook.h"

#include "Addons/Hook.h"

#include <d3d9.h>

void InitialiseHook()
{
    GF2Hook Hook;
    bool bHookInit = false;

    while (true)
    {
        hook::Type<IDirect3DDevice9*> class_index = hook::Type<IDirect3DDevice9*>(0x1205750);
        if (class_index != nullptr && bHookInit == false)
        {
            Hook.Init();
            bHookInit = true;
        }

        if (bHookInit)
        {
            Hook.Tick();
        }
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitialiseHook, NULL, NULL, NULL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

