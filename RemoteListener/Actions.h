#ifndef __Actions_h
#define __Actions_h

#include "RemoteListener.h"

void parseAction(const char* device, const char* button, const char* function, const char* repeat, const char* keycode, const char* param, const char* mode, const char* modeType, const char* OS, int numRepeat, bool* outputDevice);

typedef void (*actionCallback)(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat);

struct Action {
	Action* nextAction = 0;
	std::string Name = "";
	actionCallback callback = 0;
};

Action* getLastAction();
void registerActionCallback(actionCallback callback, std::string Name);
void registerActionCallbacks();

#endif