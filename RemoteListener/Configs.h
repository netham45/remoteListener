#ifndef __Configs_h
#define __Configs_h

#include "RemoteListener.h"
#include <string>
#include <vector>

void cacheDoc();
void runActionsForDeviceButton(const char* deviceName, const char* buttonName, int numRepeat);
std::vector<std::string> getModes(const char* modeClass);
const char* getServer();
const char* getPort();
const bool getConsoleEnabled();
#endif