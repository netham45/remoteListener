#pragma once

#include "RemoteListener.h"

void parseAction(const char* function, const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat);

typedef void (*actionCallback)(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat);

struct Action {
	Action* nextAction = 0;
	std::string Name = "";
	actionCallback callback = 0;
};

Action* getLastAction();
void registerActionCallback(actionCallback callback, std::string Name);
void registerActionCallbacks();