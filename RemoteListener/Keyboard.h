#pragma once

#include "Actions.h"

void SendKeys(const char* keys);
void SendKey(char key);
void PressKey(short vKey);
void ReleaseKey(short vKey);
void makeInput(INPUT* input, unsigned char wVk, DWORD dwFlags);
void SendVK_CB(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat);
void SendVK(const unsigned char virtualKey);
void ToggleVK_CB(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat);
void ToggleVK(const char vKey);
unsigned char vkFromFriendlyName(const char* friendlyName);
void registerKeyboardActions();