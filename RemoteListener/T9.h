#ifndef __T9_h
#define __T9_h

#include "RemoteListener.h"

#include "Actions.h"

void t9(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat);
void t9send(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat);
void t9back(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat);
void t9clear(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat);
void clear();
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
void drawText(const char* _text);
void drawHintText(const char* _text, int highlightIndex, int highlightLength);
LPCWSTR StringToLPCWSTR(const char* s);
void registerT9Actions();
#endif