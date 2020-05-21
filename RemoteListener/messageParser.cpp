#include "messageParser.h"
#include "Configs.h"
#include <string.h>

int parsedVersion = 0;

void parseMessage(char* message, unsigned int messageLen)
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
