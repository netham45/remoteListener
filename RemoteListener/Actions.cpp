#include "Actions.h"
#include <string.h>
#ifdef _WIN32
#include "Keyboard.h"
#include "T9.h"
#include "Mouse.h"
#endif

#include "Mode.h"
#include "Misc.h"

Action* headAction = 0;

using namespace std;
void parseAction(const char* device, const char* button, const char* function, const char* repeat, const char* keycode, const char* param, const char* mode, const char* modeType, const char* OS, int numRepeat, bool* outputDevice)
{
	bool run = true;

	if (strcmp(OS, HOSTOS) != 0 && strcmp(OS, "D") != 0) //Check that this action runs on this OS
	{
		run = false;
	}

	//Check for filter conditions
	if (getMode(modeType).compare(mode) != 0 && strcmp(mode, "D") != 0) //If the action has a mode set and we don't equal it
	{
		run = false;
	}

	if (strcmp(repeat, "D") == 0) //If repeat is default
	{
		if (numRepeat > 0 && numRepeat < 3) //Don't repeat on 1 or 2
		{
			run = false;
		}
	}
	else if (strcmp(repeat, "A") == 0) {} //A for Always Repeat
	else if (atoi(repeat) != numRepeat) //Else only repeat when repeat equals the desired value
	{
		run = false;
	}
	if (*outputDevice && numRepeat == 0)
	{
		printf("\n\nInput  -  Device % s Button % s\n", device, button);
		printf("State  -  curMode %s, OS %s\n\n", getMode(modeType).c_str(), HOSTOS);
		*outputDevice = false;
	}
	if (run || numRepeat == 0 || ((strcmp(mode,"D") == 0 || getMode(modeType).compare(mode) == 0) && (strcmp(OS, HOSTOS) == 0 || strcmp(OS, "D") == 0)))
		printf("Action  -  Function: %s  KeyCode: %s  Param: %s  Repeat: %i Execute:%s%s\n           Filters  -  Repeat: %s/%s  Mode: %s/%s  OS: %s/%s\n",
			function,
			keycode,
			param,
			numRepeat,
			run ? "Yes" : "No",
			((strcmp(mode, "D") == 0 || getMode(modeType).compare(mode) == 0) && (strcmp(OS, HOSTOS) == 0 || strcmp(OS, "D") == 0)) ? "" : " (Muted)",
			strcmp(repeat, "D") == 0 ? "Default [0,3+]" : strcmp(repeat, "A") == 0 ? "Any" : repeat,
			((strcmp(repeat, "D") == 0 && (numRepeat == 0 || numRepeat >= 3)) || strcmp(repeat, "A") == 0 || atoi(repeat) == numRepeat) ? "Pass" : "Fail",
			strcmp(mode, "D") == 0 ? "Default (Any)" : mode,
			(getMode(modeType).compare(mode) == 0 || strcmp(mode, "D") == 0) ? "Pass" : "Fail",
			strcmp(OS, "D") == 0 ? "Default (Any)" : OS,
			(strcmp(OS,HOSTOS) == 0 || strcmp(OS, "D") == 0) ? "Pass" : "Fail"
			);

	if (!run)
		return;
	

	if (!headAction)
		printf("No action callbacks registered!\n");
	else
		for (Action* curAction = headAction; curAction; curAction = curAction->nextAction)
			if (curAction->Name.compare(function) == 0)
				curAction->callback(repeat, keycode, param, mode, modeType, numRepeat);
}


Action* getLastAction()
{
	Action* curAction = headAction;
	if (curAction == NULL)
		return NULL;
	for (curAction; curAction->nextAction; curAction = curAction->nextAction);
	return curAction;
}

void registerActionCallback(actionCallback callback, std::string Name)
{
	Action* newAction = new Action;
	newAction->Name = Name;
	newAction->callback = callback;
	newAction->nextAction = NULL;
	Action* lastAction = getLastAction();
	if (lastAction)
		lastAction->nextAction = newAction;
	else
		headAction = newAction;
}

void registerActionCallbacks()
{
#ifdef _WIN32
	registerMouseActions();
	registerT9Actions();
	registerKeyboardActions();
#endif
	registerModeActions();
	registerMiscActions();
}
