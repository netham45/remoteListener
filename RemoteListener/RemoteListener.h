#ifndef __RemoteListener_h
#define __RemoteListener_h
#define _CRT_SECURE_NO_DEPRECATE
#ifdef _WIN32
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#pragma comment (lib, "Wtsapi32.lib")
#define HOSTOS "Windows"
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <WtsApi32.h>
#elif __linux__
#define HOSTOS "Linux"
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#define SA struct sockaddr 
#endif
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

constexpr auto DEFAULT_BUFLEN = 2048;
constexpr auto HELO = "VERSION\n";
constexpr auto MAGIC4PC_SUBSCRIBE = R"data({"t": "sub_sensor", "updateFreq" : 33, "filter" : ["coordinate"]})data";
#endif

