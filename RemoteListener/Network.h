#ifndef __Network_h
#define __Network_h
#include "RemoteListener.h"
int start(const char* server, const char* port);
bool sendPacket(const char* server, const char* port, const char* message, unsigned int messagelen, char* recvbuf, unsigned int recvbuflen);
#endif