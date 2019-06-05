#pragma once

#include "RemoteListener.h"
#include <string>
#include <vector>

void cacheDoc();
void runActionsForDeviceButton(const char* deviceName, const char* buttonName, int numRepeat);
std::vector<std::string> getModes();
PCSTR getServer();
PCSTR getPort();