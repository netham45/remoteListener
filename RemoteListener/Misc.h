#pragma once
#include "RemoteListener.h"
#include "Actions.h"

void run(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat);
void toggleScreens(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat);
void screensOff(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat);
void registerMiscActions();