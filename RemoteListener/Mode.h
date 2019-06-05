#pragma once
#include "RemoteListener.h"
#include "Actions.h"

void cycleMode(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat);
void registerModeActions();
std::string getMode();