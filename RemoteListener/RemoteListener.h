#ifndef __RemoteListener_h
#define __RemoteListener_h
#define _CRT_SECURE_NO_DEPRECATE
#ifdef _WIN32
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#define HOSTOS "Windows"
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#elif __linux__
#define HOSTOS "Linux"
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#define SA struct sockaddr 
#endif
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

constexpr auto DEFAULT_BUFLEN = 2048;
constexpr auto HELO = "VERSION\n";
#endif
