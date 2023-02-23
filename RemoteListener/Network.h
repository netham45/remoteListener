#ifndef __Network_h
#define __Network_h
#include "RemoteListener.h"
int start(const char* server, const char* port, const char* type);
bool sendPacket(const char* server, const char* port, const char* message, unsigned int messagelen, char* recvbuf, unsigned int recvbuflen);

#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")


#endif