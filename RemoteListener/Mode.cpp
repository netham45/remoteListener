#include "Mode.h"
#include "rapidxml.hpp"
#include "Configs.h"
#include <string>
#include <vector>

using namespace std;

string currentMode = "";

void cycleMode(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
{
	vector<string> Modes = getModes();
	if (currentMode.compare("") == 0)
		currentMode = Modes[0];
	else
		for (unsigned int i = 0; i < Modes.size(); i++)
			if (currentMode.compare(Modes[i]) == 0)
				currentMode = Modes[++i % Modes.size()];
}

string getMode()
{
	if (currentMode.compare("") == 0)
		cycleMode(0,0,0,0,0);
	return currentMode;
}

void registerModeActions()
{
	registerActionCallback(&cycleMode, "ModeCycle");
}