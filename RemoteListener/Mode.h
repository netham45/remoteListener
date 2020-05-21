#ifndef __Mode_h
#define __Mode_h
#include "RemoteListener.h"
#include "Actions.h"

void cycleMode(const char* repeat, const char* keycode, const char* param, const char* mode, const char* modeType, int numRepeat);
void registerModeActions();
std::string getMode(const char* param);
#endif