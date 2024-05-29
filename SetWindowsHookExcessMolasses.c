#include <windows.h>

#ifdef _WIN64
    #define __ARCH_BITS 64
#elif _WIN32
    #define __ARCH_BITS 32
#endif

#define TOKEN_TO_QUOTED(x) #x
#define MACRO_TO_QUOTED(x) TOKEN_TO_QUOTED(x)

#ifdef __ARCH_BITS
    #define __TITLE "SetWindowsHookEx" MACRO_TO_QUOTED(__ARCH_BITS)
#else
    #define __TITLE "SetWindowsHookExcessMolasses"
#endif

HINSTANCE __hinstDLL=NULL;
HHOOK __hhk=NULL;

static BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch(fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        OutputDebugString(TEXT(__TITLE " DLL_PROCESS_ATTACH"));
        __hinstDLL=hinstDLL;
        return TRUE;
    case DLL_THREAD_ATTACH:
        OutputDebugString(TEXT(__TITLE " DLL_THREAD_ATTACH"));
        break;
    case DLL_THREAD_DETACH:
        OutputDebugString(TEXT(__TITLE " DLL_THREAD_DETACH"));
        break;
    case DLL_PROCESS_DETACH:
        OutputDebugString(TEXT(__TITLE " DLL_PROCESS_DETACH"));
        
        if(lpvReserved)
            break;
        
        //
        break;
    }
    
    return TRUE;
}

STDAPI DllRegisterServer(void)
{
    OutputDebugString(TEXT(__TITLE " DllRegisterServer"));
    return S_OK;
}

STDAPI DllUnregisterServer(void)
{
    OutputDebugString(TEXT(__TITLE " DllUnregisterServer"));
    return S_OK;
}

STDAPI DllInstall(BOOL bInstall, PCWSTR pszCmdLine)
{
    OutputDebugString(TEXT(__TITLE " DllInstall"));
    return S_OK;
}

LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(__hhk)
        OutputDebugString(TEXT(__TITLE " GetMsgProc Test"));
    else
        OutputDebugString(TEXT(__TITLE " GetMsgProc"));
    
    return CallNextHookEx(__hhk, nCode, wParam, lParam);
}

EXTERN_C void __stdcall Test(void)
{
    OutputDebugString(TEXT(__TITLE " Test"));
    
    if(__hhk=SetWindowsHookEx(WH_GETMESSAGE, &GetMsgProc, __hinstDLL, 0))
    {
        MessageBox(NULL, "Test has established a hook.\n\nClose this dialog to unhook.", __TITLE, MB_OK|MB_ICONINFORMATION|MB_SYSTEMMODAL);
        UnhookWindowsHookEx(__hhk);
    }
    else
    {
        MessageBox(NULL, "SetWindowsHookEx has failed.\n\nThe program should now quit.", __TITLE, MB_OK|MB_ICONEXCLAMATION|MB_SYSTEMMODAL);
    }
    
    return;
}
