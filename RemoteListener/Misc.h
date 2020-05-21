#ifndef __Misc_h
#define __Misc_h
#include "RemoteListener.h"
#include "Actions.h"

void run(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat);
void toggleScreens(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat);
void screensOff(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat);
void webRequest(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat);
wchar_t* widen(const char* charArray);
void registerMiscActions();
#endif