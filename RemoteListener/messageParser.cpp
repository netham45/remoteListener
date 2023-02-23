#include "messageParser.h"
#include "Configs.h"
#include <string.h>
#include "json.hpp"
#include "Mouse.h"
#include "base64.h"

using namespace std;
using json = nlohmann::json;
int parsedVersion = 0;

void lirc_parseMessage(char* message, unsigned int messageLen)
{
	char button[DEFAULT_BUFLEN] = { 0 };
	char device[DEFAULT_BUFLEN] = { 0 };

	char repeatStr[DEFAULT_BUFLEN];
	unsigned int repeat = 0;

	unsigned int head = 0;
	unsigned int bufHead = 0;
	unsigned int startHead = head;
	switch (parsedVersion)
	{
	case 0:
		while (message[++head] != '\n' && head < messageLen); //BEGIN
	case 1:
		if (head < messageLen) parsedVersion++; else break;
		while (message[++head] != '\n' && head < messageLen); //SUCCESS
	case 2:
		if (head < messageLen) parsedVersion++; else break;
		while (message[++head] != '\n' && head < messageLen); //VERSION
	case 3:
		if (head < messageLen) parsedVersion++; else break;
		while (message[++head] != '\n' && head < messageLen); //[DATA
	case 4:
		if (head < messageLen) parsedVersion++; else break;
		while (message[++head] != '\n' && head < messageLen); //1 (line)
	case 5:
		startHead = head + 1;
		if (head < messageLen) parsedVersion++; else break;
		while (message[++head] != '\n' && head < messageLen); //<VERSION>
		printf("Connected\nlircd Server Version: %.*s\n", head - startHead, message + startHead);
	case 6:
		if (head < messageLen) parsedVersion++; else break;
		while (message[++head] != '\n' && head < messageLen); //END
		parsedVersion++;
	}

	if (head >= messageLen)
		return;
	while (message[++head] != ' ' && head < messageLen); //Skip the first field
	while (message[++head] != ' ' && head < messageLen) //Read the Repeat Code, copy it to the repeatStr buffer
		repeatStr[bufHead++] = message[head];
	repeatStr[bufHead] = 0; //Null-terminate the string
#pragma warning( push )
#pragma warning( disable : 6054) //Disable warnings for possibly not null-terminated string for the next line
	repeat = strtol(repeatStr, NULL, 16); //Convert the hex string to an integer
#pragma warning( pop )
	bufHead = 0;
	while (message[++head] != ' ' && head < messageLen) //Read the Button Name, copy it to the button buffer
		button[bufHead++] = message[head];
	button[bufHead] = 0; //Null-terminate the string
	bufHead = 0;
	while (message[++head] != '\x0A' && head < messageLen) //Read the Device Name, copy it to the device buffer
		device[bufHead++] = message[head];
	device[bufHead] = 0; //Null-terminate the string
	if (strlen(device) && strlen(button))
	{
		runActionsForDeviceButton(device, button, repeat);
	}
}

uint8_t magicButtonsRepeat[2048] = { 0 };
bool ignoreNextKey = false;

void magic4pc_parseMessage(char* message, unsigned int messageLen, SOCKET ConnectSocket)
{
#ifdef _WIN32
	/*printf("Got data for magic4pc\n");
	for (int i=0;i<messageLen;i++)
		printf("%c",message[i]);
	printf("\n");*/
	message[messageLen] = 0;
	json data = json::parse(message);
	string type = data["t"].get<string>();
	if (type == "keepalive")
	{
		send(ConnectSocket, "{}", strlen("{}"), 0);
	}
	else if (type == "remote_update")
	{
		string payload = data["payload"].get<string>();
		string output;
		int xy[2];
		macaron::Base64::Decode(payload, output);
		memcpy(xy, output.c_str(), sizeof(xy));
		int x = xy[0];
		int y = xy[1];
		int fixedx = (int)((double)x * 34.2403343783); // 0-1914 -> 0-65535
		int fixedy = (int)((double)y * 61.0204841713); // 0-1074 -> 0-65535
		sendCursorPosition(fixedx, fixedy, true);
	}
	else if (type == "input")
	{
		bool isDown = data["parameters"]["isDown"].get<bool>();
		int keyCode = data["parameters"]["keyCode"].get<int>();
		if (ignoreNextKey)
		{
			ignoreNextKey = false;
		}
		else
		{
			if (isDown)
			{
				char name[32];
				sprintf(name, "%i", keyCode);
				runActionsForDeviceButton("magic4pc", name, magicButtonsRepeat[keyCode]++);
			}
			else
			{
				char name[32];
				sprintf(name, "%i_up", keyCode);
				runActionsForDeviceButton("magic4pc", name, magicButtonsRepeat[keyCode]);
				magicButtonsRepeat[keyCode] = 0;
			}
		}
		if (keyCode == 1537 and isDown)
		{
			ignoreNextKey = true;
		}
	}
	else if (type == "mouse")
	{
		runActionsForDeviceButton("magic4pc", data["mouse"]["type"].get<string>().c_str(), 0);
	}
	else if (type == "wheel")
	{
		int delta = data["wheel"]["delta"].get<int>();
		printf("Delta: %i\n", delta);
		runActionsForDeviceButton("magic4pc", delta > 0 ? "wheelup" : "wheeldown", abs(delta) / 60);
	}
#endif
}


void parseMessage(char* message, unsigned int messageLen, SOCKET ConnectSocket)
{
	const char* type = getType();
	if (_stricmp(type, "lirc") == 0)
	{
		lirc_parseMessage(message, messageLen);
	}
	else if (_stricmp(type, "magic4pc") == 0)
	{
		magic4pc_parseMessage(message, messageLen, ConnectSocket);
	}
}