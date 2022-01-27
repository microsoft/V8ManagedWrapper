
#include "stdafx.h"

#ifdef _WIN32

#include <windows.h>
extern "C" {
    BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD dwReason, LPVOID lpvReserved) {
        // Do nothing.
        return TRUE;
    }
}

#endif
