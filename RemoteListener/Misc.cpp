#include "Misc.h"
#ifdef _WIN32
#include <WinHttp.h>
#pragma comment(lib, "winhttp.lib")
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")
#endif
#include <string.h>
#include <iostream>
#include <vector>
#include "Network.h"
#include <string>
#include <regex>
#include "secrets.h"

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

int post(const char* URL, const char* data)
{
#ifdef _WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cerr << "WSAStartup failed with error: " << iResult << '\n';
		return 1;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL, * ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Create a regular expression to match the URL components
	std::regex urlRegex("(\\w+)://([^:/]+)(:([0-9]+))?(/.*)?");


	// Match the regular expression to the URL
	std::smatch urlMatch;
	std::string sURL(URL);
	if (!std::regex_match(sURL, urlMatch, urlRegex)) {
		return 1;
	}
	std::string scheme = urlMatch[1];
	std::string hostname = urlMatch[2];
	std::string portStr = urlMatch[4];
	std::string path = urlMatch[5];

	
	iResult = getaddrinfo(hostname.c_str(), portStr.empty() ? "80" : portStr.c_str(), & hints, & result);
	if (iResult != 0) {
		std::cerr << "getaddrinfo failed with error: " << iResult << "for host" << hostname << '\n';
		WSACleanup();
		return 1;
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			std::cerr << "socket failed with error: " << WSAGetLastError() << '\n';
			WSACleanup();
			return 1;
		}

		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		std::cerr << "Unable to connect to server!\n";
		WSACleanup();
		return 1;
	}

	std::string request =
		"POST " + std::string(path) + " HTTP/1.1\r\n"
		"Host: " + std::string(hostname) + "\r\n"
		HA_AUTHORIZATION +
		"Content-Type: application/json\r\n"
		"Content-Length: " + std::to_string(strlen(data)) + "\r\n"
		"\r\n" + std::string(data);
	printf("%s\n",request.c_str());

	iResult = send(ConnectSocket, request.c_str(), request.length(), 0);
	if (iResult == SOCKET_ERROR) {
		std::cerr << "send failed with error: " << WSAGetLastError() << '\n';
		closesocket(ConnectSocket);
		WSACleanup();

		return 1;
	}

	std::string response;


	char recvbuf[1024];
	fd_set readSet;
	timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;

	// Set the receive timeout option on the socket
	setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	FD_ZERO(&readSet);
	FD_SET(ConnectSocket, &readSet);

	iResult = select(0, &readSet, NULL, NULL, &timeout);

	if (iResult == 0) {
		std::cerr << "recv timed out\n";
		return 1;
	}
	else if (iResult == SOCKET_ERROR) {
		std::cerr << "select failed with error: " << WSAGetLastError() << '\n';
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	iResult = recv(ConnectSocket, recvbuf, 1024, 0);
	if (iResult > 0) {
		response += std::string(recvbuf, iResult);
	}
	else if (iResult == 0) {
		std::cout << "Connection closed\n";
	}
	else {
		std::cerr << "recv failed with error: " << WSAGetLastError() << '\n';
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	int body_pos = response.find("\r\n\r\n");
	if (body_pos == std::string::npos) {
		std::cerr << "Error parsing response\n";
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}


	std::cout << response;
	return 0;
#else
	return 1;
#endif
}

void harequest(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	char data[128];
	sprintf(data, R"json({"entity_id": "%s"})json", keycode);
	post(param, data);
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
	registerActionCallback(&harequest, "HARequest");
#endif
	registerActionCallback(&webRequest, "WebRequest");
}

