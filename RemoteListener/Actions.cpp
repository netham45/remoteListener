#include "Actions.h"
#include "Keyboard.h"
#include "T9.h"
#include "Mouse.h"
#include "Mode.h"
#include "Misc.h"

Action* headAction = 0;

using namespace std;
void parseAction(const char* function, const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
{
	bool run = true;

	//Check for filter conditions
	if (getMode().compare(mode) != 0 && strcmp(mode, "D") != 0) //If the action has a mode set and we don't equal it
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

	printf("\tAction - Function %s, KeyCode %s, param %s\n\t\tFilters - repeat %s, mode %s -- State - curMode %s, numRepeat %i -- Executing: %s\n",
		function, keycode, param, repeat, mode, getMode().c_str(), numRepeat, run?"Yes":"No");

	if (!run)
		return;

	if (!headAction)
		printf("No actions!\n");
	else
		for (Action* curAction = headAction; curAction->nextAction; curAction = curAction->nextAction)
			if (curAction->Name.compare(function) == 0)
				curAction->callback(repeat, keycode, param, mode, numRepeat);
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
	registerMouseActions();
	registerT9Actions();
	registerKeyboardActions();
	registerModeActions();
	registerMiscActions();
}