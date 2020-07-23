#include "Misc.h"
#ifdef _WIN32
#include <WinHttp.h>
#pragma comment(lib, "winhttp.lib")
#endif
#include <string.h>
#include <vector>
#include "Network.h"
#define _CRT_SECURE_NO_WARNINGS

void run(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	if (strcmp(keycode, "D") != 0)
	{
#ifdef _WIN32
		Sleep(atoi(keycode));
#elif __linux__
		sleep(atoi(keycode));
#endif
	}
#ifdef _WIN32
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	WCHAR pWideString[2048];
	ZeroMemory(pWideString, sizeof(pWideString));
	MultiByteToWideChar(CP_ACP, 0, param, strlen(param), pWideString, sizeof(pWideString));
	if (!CreateProcess(NULL, pWideString, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
#elif __linux__
	char command[512];
	sprintf(command, "%s &", param);
	system(command);
#endif
}

void toggleScreens(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
#ifdef _WIN32
	if (strcmp(param, "0") == 0)
		system("displayswitch /internal");
	if (strcmp(param, "1") == 0)
		system("displayswitch /external");
	if (strcmp(param, "2") == 0)
	{
		Sleep(1000);
		system("displayswitch /extend");
	}
#endif
}

void screensOff(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
#ifdef _WIN32
	SendNotifyMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
#endif
}

void webRequest(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{

	char request[256] = "";
	char receive[512] = "";
	char host[256] = "";
	char port[8] = "80";
	char portFlag = false;
	char portHead = 0;
	for (int i = 0; i < strlen(param); i++)
	{
		if (param[i] == ':')
			portFlag = true;
		else if (!portFlag)
			host[i] = param[i];
		else
			port[portHead++] = i;
	}	
	sprintf(request,"GET %s HTTP/1.0\r\nHost: %s\r\n\r\n", keycode, host);
	sendPacket(host, port, request, strlen(request)+1, receive, sizeof(receive));
}

void sendPacket(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	char message[32];
	char recvbuff[32];
	sprintf(message, "%s\r\n", keycode);
	sendPacket(param, "23", message, strlen(message),recvbuff,sizeof(recvbuff));
}

void projectorPowerToggle(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	char recvbuff[32];
	const char* message = "ICHK????\r\n";
	sendPacket(param, "23",  message,strlen(message),recvbuff,sizeof(recvbuff));
	if (strcmp(recvbuff, "F\x0D") == 0)
	{
		//Turn projector on
		const char* message = "POWR___1\r\n";
		printf("Turning Projector On\n");
		sendPacket(param, "23", message, strlen(message), recvbuff, sizeof(recvbuff));
	}
	else
	{
		//Turn projector on
		const char* message = "POWR___0\r\n";
		printf("Turning Projector Off\n");
		sendPacket(param, "23", message, strlen(message), recvbuff, sizeof(recvbuff));
	}

}

wchar_t* widen(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
#ifdef _WIN32
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
#elif __linux__
	mbtowc(wString,charArray,strlen(charArray));
#endif
	return wString;
}

void registerMiscActions()
{
	registerActionCallback(&projectorPowerToggle, "ProjectorPowerToggle");
	registerActionCallback(&sendPacket, "SendPacket");
	registerActionCallback(&run, "Run");
#ifdef _WIN32
	registerActionCallback(&toggleScreens, "ToggleScreens");
	registerActionCallback(&screensOff, "ScreensOff");
#endif
	registerActionCallback(&webRequest, "WebRequest");
}

