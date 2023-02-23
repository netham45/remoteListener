#include "Configs.h"
#include <string>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iostream>
#include <malloc.h>
#include "Actions.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

using namespace rapidxml;
using namespace std;

xml_document<> cachedDoc;
bool parsed = false;
vector<char> buffer;

void cacheDoc()
{

	if (!parsed)
	{
		std::ifstream t("./Config.xml");
		buffer = vector<char>((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
		buffer.push_back('\0');
		t.close();
		cachedDoc.parse<0>(&buffer[0]);
		parsed = true;
	}
}

void runActionsForDeviceButton(const char* deviceName, const char* buttonName, int numRepeat)
{
	cacheDoc();
	xml_node<>* pRoot = cachedDoc.first_node();

	for (xml_node<>* pDevice = pRoot->first_node(); pDevice; pDevice = pDevice->next_sibling()) //For Each Device
	{
		if (strcmp(pDevice->name(), "Remote") != 0)
			continue;
		for (xml_attribute<>* attr = pDevice->first_attribute(); //Check each Attribute
			attr; attr = attr->next_attribute())
		{
			if (strcmp(attr->name(), "Name") != 0) //Only look for names
				continue;
			if (strcmp(attr->value(), deviceName) == 0) //If the Device Name equals the provided device
			{
				for (xml_node<>* pButton = pDevice->first_node(); pButton; pButton = pButton->next_sibling()) //For each child node (button mapping)
				{
					if (strcmp(pButton->name(), "Button") != 0)
						continue;
					for (xml_attribute<>* attr = pButton->first_attribute(); //Check each attribute
						attr; attr = attr->next_attribute())
					{
						if (strcmp(attr->name(), "Name") != 0) //Only look for names
							continue;
						if (strcmp(attr->value(), buttonName) == 0) //If the Button Name equals the provided button
						{
							bool outputDevice = true;
							for (xml_node<>* pAction = pButton->first_node(); pAction; pAction = pAction->next_sibling()) //For each child of the button (action to perform)
							{
								if (strcmp(pAction->name(), "Action") != 0)
									continue;
								string param = "D", keycode = "D", function = "D", repeat = "D", mode = "D", modeType = "D", state = "D", OS = "D"; //Used to indicate the action doesn't override default behavior
								for (xml_attribute<>* attr = pAction->first_attribute(); //For each attribute on the action
									attr; attr = attr->next_attribute())
								{
									if (strcmp(attr->name(), "Function") == 0) //If it's the function then set the function
									{
										function = attr->value();
									}
									else if (strcmp(attr->name(), "KeyCode") == 0) //Keycode
									{
										keycode = attr->value();
									}
									else if (strcmp(attr->name(), "Repeat") == 0) //Repeat
									{
										repeat = attr->value();
									}
									else if (strcmp(attr->name(), "Param") == 0) //Param
									{
										param = attr->value();
									}
									else if (strcmp(attr->name(), "Mode") == 0) //Mode
									{
										mode = attr->value();
									}
									else if (strcmp(attr->name(), "ModeType") == 0) //Mode
									{
										modeType = attr->value();
									}
									else if (strcmp(attr->name(), "OS") == 0) //Mode
									{
										OS = attr->value();
									}
								}
								parseAction(deviceName, buttonName, function.c_str(), repeat.c_str(), keycode.c_str(), param.c_str(), mode.c_str(), modeType.c_str(), OS.c_str(), numRepeat, &outputDevice);
							}
							if (outputDevice && numRepeat == 0)
								printf("Input - Device % s Button % s -- No Actions\n", deviceName, buttonName);
							return;
						}
					}
				}
			}
		}
	}
}

std::vector<std::string> getModes(const char* modeClass)
{
	vector <string> modes;

	cacheDoc();
	xml_node<>* pRoot = cachedDoc.first_node();
	for (xml_node<>* pModes = pRoot->first_node(); pModes; pModes = pModes->next_sibling()) //For Each State
	{
		if (strcmp(pModes->name(), "Modes") == 0)
		{
			for (xml_attribute<>* attr = pModes->first_attribute(); attr; attr = attr->next_attribute())
			{
				if (strcmp(attr->name(), "Name") == 0)
				{
					if (strcmp(attr->value(),modeClass) == 0)
					{
						for (xml_node<>* pMode = pModes->first_node(); pMode; pMode = pMode->next_sibling()) //For Each State
						{
							for (xml_attribute<>* attr = pMode->first_attribute(); attr; attr = attr->next_attribute())
							{
								if (strcmp(attr->name(), "Name") == 0)
								{
									modes.push_back(attr->value());
								}
							}
						}
					}
				}
			}
		}
	}
	return modes;
}

const char* getServer()
{
	cacheDoc();
	xml_node<>* pRoot = cachedDoc.first_node();

	for (xml_node<>* pServer = pRoot->first_node(); pServer; pServer = pServer->next_sibling()) //For Each State
		if (strcmp(pServer->name(), "Server") == 0)
			for (xml_attribute<>* attr = pServer->first_attribute(); //Check each attribute
				attr; attr = attr->next_attribute())
				if (strcmp(attr->name(), "Host") == 0)
					return attr->value();
	return "";
}

const char* getPort()
{
	cacheDoc();
	xml_node<>* pRoot = cachedDoc.first_node();

	for (xml_node<>* pServer = pRoot->first_node(); pServer; pServer = pServer->next_sibling()) //For Each State
		if (strcmp(pServer->name(), "Server") == 0)
			for (xml_attribute<>* attr = pServer->first_attribute(); //Check each attribute
				attr; attr = attr->next_attribute())
				if (strcmp(attr->name(), "Port") == 0)
					return attr->value();
	return "0";
}

const char* getType()
{
	cacheDoc();
	xml_node<>* pRoot = cachedDoc.first_node();

	for (xml_node<>* pServer = pRoot->first_node(); pServer; pServer = pServer->next_sibling()) //For Each State
		if (strcmp(pServer->name(), "Server") == 0)
			for (xml_attribute<>* attr = pServer->first_attribute(); //Check each attribute
				attr; attr = attr->next_attribute())
				if (strcmp(attr->name(), "Type") == 0)
					return attr->value();
	return "0";
}

const bool getConsoleEnabled()
{
	cacheDoc();
	xml_node<>* pRoot = cachedDoc.first_node();

	for (xml_node<>* pServer = pRoot->first_node(); pServer; pServer = pServer->next_sibling()) //For Each State
		if (strcmp(pServer->name(), "Console") == 0)
			for (xml_attribute<>* attr = pServer->first_attribute(); //Check each attribute
				attr; attr = attr->next_attribute())
				if (strcmp(attr->name(), "Enabled") == 0)
					return atoi(attr->value()) > 0;
	return true;
}
