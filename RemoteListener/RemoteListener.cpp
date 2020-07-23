
#include "RemoteListener.h"
#include "Network.h"
#include "Configs.h"
#include "Actions.h"
#include <process.h>

int main()
{
	printf("RemoteListener running on %s\n", HOSTOS);
#ifdef __linux__
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
#endif
#ifdef WIN32
	if (!getConsoleEnabled())
	{
		HWND consoleWnd = GetConsoleWindow();
		DWORD dwProcessId;
		GetWindowThreadProcessId(consoleWnd, &dwProcessId);
		if (GetCurrentProcessId() == dwProcessId)
		{
			FreeConsole();
		}
		else
		{
			printf("Console disabled, but this isn't our console to close.\n");
		}
	}

	// Ensure only one running instance
	DWORD errorCode = -1;
	HANDLE hMutexHandle;
	int tryCount = 0;
	do
	{
		bool badTerm = false;
		hMutexHandle = CreateMutex(NULL, TRUE, L"remote.listener");
		errorCode = GetLastError();
		if (errorCode == ERROR_ALREADY_EXISTS)
		{
			WTS_PROCESS_INFO* pWPIs = NULL;
			DWORD dwProcCount = 0;
			if (WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE, NULL, 1, &pWPIs, &dwProcCount))
			{
				for (DWORD i = 0; i < dwProcCount; i++)
				{
					if (lstrcmpW(pWPIs[i].pProcessName, L"RemoteListener.exe") == 0)
					{
						if (pWPIs[i].ProcessId != GetCurrentProcessId())
						{
							printf("Found another instance of RemoteListener under pid %i. Terminating.\n", pWPIs[i].ProcessId);
							HANDLE oldRemoteListener;
							oldRemoteListener = OpenProcess(PROCESS_ALL_ACCESS, false, pWPIs[i].ProcessId);
							if (!TerminateProcess(oldRemoteListener, 0))
								badTerm = 1;
							CloseHandle(oldRemoteListener);
						}
					}
				}
			}
			Sleep(1000);
			ReleaseMutex(hMutexHandle);
			CloseHandle(hMutexHandle);
		}
	} while (errorCode == ERROR_ALREADY_EXISTS && tryCount++ < 5);
	if (errorCode == ERROR_ALREADY_EXISTS)
	{
		printf("Couldn't close running instance of RemoteListener. Giving up.\n");
		return -1;
	}

#endif
	registerActionCallbacks();
	printf("Connecting To %s:%s\n", getServer(), getPort());
	start(getServer(), getPort());
#ifdef WIN32
	ReleaseMutex(hMutexHandle);
	CloseHandle(hMutexHandle);
#endif
	return 0;
}

