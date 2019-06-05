#pragma once

#include "RemoteListener.h"

#include "Actions.h"

void t9(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat);
void t9send(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat);
void t9back(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat);
void t9clear(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat);
void clear();
void registerT9Actions();