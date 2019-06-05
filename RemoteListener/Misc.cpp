#include "Misc.h"

void run(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	WCHAR pWideString[2048];
	MultiByteToWideChar(CP_ACP, 0, param, strlen(param), pWideString, 2048);

	CreateProcess(NULL, pWideString,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

void toggleScreens(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
{
	if (GetSystemMetrics(SM_CMONITORS) == 1) //Only one monitor active, extend
		system("displayswitch /extend");
	else //More than one active, disable all but.
		system("displayswitch /internal");
}

void screensOff(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
{
	SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
}

void registerMiscActions()
{
	registerActionCallback(&run, "Run");
	registerActionCallback(&toggleScreens, "ToggleScreens");
	registerActionCallback(&screensOff, "ScreensOff");
}