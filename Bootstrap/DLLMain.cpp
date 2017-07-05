#include "stdafx.h"
#include <io.h>
#include <stdlib.h>
#include <fcntl.h>
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>

#include "functions.h"

#pragma comment(lib, "Psapi.lib")

#include "MSCorEE.h"
#include <metahost.h>
#pragma comment(lib, "mscoree.lib")

// Import mscorlib.tlb (Microsoft Common Language Runtime Class Library).
#import "mscorlib.tlb" raw_interfaces_only				\
    high_property_prefixes("_get","_put","_putref")		\
    rename("ReportEvent", "InteropServices_ReportEvent")\
	rename("value", "mscor_value")
using namespace mscorlib;

ICLRMetaHost *pMetaHost = NULL;
ICLRRuntimeInfo *pRuntimeInfo = NULL;
ICLRRuntimeHost *pRuntimeHost = NULL;

void LoadCLR()
{
    CLRCreateInstance(CLSID_CLRMetaHost, IID_PPV_ARGS(&pMetaHost));
    pMetaHost->GetRuntime(L"v4.0.30319", IID_PPV_ARGS(&pRuntimeInfo));

    BOOL fLoadable;

    pRuntimeInfo->IsLoadable(&fLoadable);
    pRuntimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_PPV_ARGS(&pRuntimeHost));
	pRuntimeHost->Start();
}

DWORD ExecuteManaged(PCWSTR method, PCWSTR argument)
{
	DWORD result;

	pRuntimeHost->ExecuteInDefaultAppDomain(L"SeeSharp.dll", L"SeeSharp.Main", method, argument, &result);

	return result;
}

bool CanInvokeCS(PCWSTR method)
{
	DWORD result = ExecuteManaged(L"CanInvoke", method);

	if (result == -1)
		fChatWindowSay("ERROR", "ERROR: Script system not initialised!  Please use /init to initialise before attempting to execute commands.", 9);

	return (result == 1);
}

void InvokeCS(PCWSTR argument)
{
    ExecuteManaged(L"Invoke", argument);
}

void UnloadCLR()
{
	pRuntimeHost->Stop();

	pMetaHost = NULL;
	pRuntimeInfo = NULL;
	pRuntimeHost = NULL;
}

#pragma unmanaged
__declspec(noinline) int __fastcall ParseCmdHook()
{
	char* msg;
	__asm MOV msg, EDX

	if (msg[0] == '/')
	{
		int len = strlen(msg);
		printf("%i\n", len);
		WCHAR* tmp = new WCHAR[len];
		MultiByteToWideChar(0, 0, (msg + 1), len, tmp, len);
		LPCWSTR arg = tmp;

		if (CanInvokeCS(arg))
		{
			InvokeCS(arg);
			msg = "/_xt";
		}
	}

	__asm MOV ECX, 0x100
	__asm MOV EDX, msg
	int value = fParseCmd(0x100, msg);
	return value;
}

#pragma region Entry Point

extern "C" {

	BOOL WINAPI DllMain(HANDLE hDLL, DWORD dwReason, LPVOID lpReserved)
	{
		// First, let's get the process and window handles
		HANDLE hProcess = GetCurrentProcess();
		HWND hWnd = FindWindow(0, "CABAL");
		PDWORD lpflOldProtect = 0;		
		UINT address;

		switch(dwReason)
		{
			case DLL_PROCESS_ATTACH:
				// Load the CLR
				LoadCLR();
				// Set window title so we know we're in
				SetWindowText(hWnd, "CABAL :: Let's see how deep this rabbit hole goes.");
				// Hook message-parsing method call
				VirtualProtectEx(hProcess, (LPVOID)(0x00591AD7), 64, PAGE_READWRITE, lpflOldProtect);
				address = (UINT)&ParseCmdHook - (0x00591AD7 + 5);
				WriteProcessMemory(hProcess, (LPVOID)(0x00591AD7 + 1), &address, 4, NULL);
				break;
			case DLL_PROCESS_DETACH:
				// Unload the CLR
				UnloadCLR();
				// Reset window title
				SetWindowText(hWnd, "CABAL");
				break;
		}

		return true;
	}

}

#pragma endregion