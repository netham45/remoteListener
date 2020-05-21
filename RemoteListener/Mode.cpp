#include "Mode.h"
#include "rapidxml.hpp"
#include "Configs.h"
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

unordered_map<string, string> currentModes;

void cycleMode(const char* repeat, const char* keycode, const char* param, const char* mode, const char* modeType, int numRepeat)
{
	string currentMode;
	currentMode = getMode(param);
	vector<string> Modes = getModes(param);
	if (currentMode.compare("") == 0)
		currentMode = Modes[0];
	else
		for (unsigned int i = 0; i < Modes.size(); i++)
			if (currentMode.compare(Modes[i]) == 0)
				currentMode = Modes[++i % Modes.size()];
	currentModes[param] = currentMode;
}

void setMode(const char* repeat, const char* keycode, const char* param, const char* mode, const char* modeType, int numRepeat)
{
	string currentMode = "";
	vector<string> Modes = getModes(keycode);
	for (unsigned int i = 0; i < Modes.size(); i++)
		if (Modes[i].compare(param) == 0)
			currentMode = Modes[i];
	if (currentMode.empty())
	{
		printf("Can't set mode for %s to %s, %s is not a defined option. Defined options: ", keycode, param, param);
		for (unsigned int i = 0; i < Modes.size(); i++)
			if (Modes[i].compare(param) == 0)
				printf("%s ", Modes[i]);
		printf("\n");
	}
	currentModes[keycode] = currentMode;
}

string getMode(const char* modeType)
{
	if (strcmp(modeType, "D") == 0)
		return "";
	string paramMode = modeType;
	std::unordered_map<string, string>::const_iterator currentMode_map = currentModes.find(paramMode);
	if (currentMode_map != currentModes.end())
		return currentMode_map->second.data();
	else
	{
		vector<string> Modes = getModes(modeType);
		return Modes[0];
	}
}

void registerModeActions()
{
	registerActionCallback(&setMode, "ModeSet");
	registerActionCallback(&cycleMode, "ModeCycle");
}