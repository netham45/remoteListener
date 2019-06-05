#pragma once
#include "messageParser.h"
#include "Configs.h"

void parseMessage(char* message, unsigned int messageLen)
{
	char button[DEFAULT_BUFLEN];
	char device[DEFAULT_BUFLEN];

	char repeatStr[DEFAULT_BUFLEN];
	unsigned int repeat = 0;

	unsigned int head = 0;
	unsigned int bufHead = 0;

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
	printf("Input: %s:%s\n", device, button);
	runActionsForDeviceButton(device, button, repeat);
}