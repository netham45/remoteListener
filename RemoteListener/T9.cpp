#include <time.h>
#include "T9.h"
#include "Keyboard.h"

const char* t9keys[] = { " ,.0?!'\\`\"1:;", "@*/-+=#$%^&~|(){}[]", "abc2ABC", "def3DEF", "ghi4GHI", "jkl5JKL", "mno6MNO", "pqrs7PRQS", "tuv8TUV", "wxyz9WXYZ" };

char buffer[1024] = { 0 };

time_t lastPressTime = 0;
int keyIndex = 0;
int lastKey = 0;

void t9(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
{
	int key = atoi(keycode);
	//TODO: Draw T9 text on screen
	if (numRepeat > 0 && numRepeat % 5 != 0) //Don't send keys as fast as the remote sends input
		return;
	time_t currentTime = time(NULL);
	double seconds = difftime(currentTime, lastPressTime); //Get the time since the last keypress
	lastPressTime = currentTime;
	if (seconds < 2 && strlen(buffer) > 0 && lastKey == key)
	{
		++keyIndex %= strlen(t9keys[key]); //Cycle through characters in the string
		buffer[strlen(buffer) - 1] = t9keys[key][keyIndex]; //And set the last character of the buffer to the string
	}
	else
	{
		keyIndex = 0;
		buffer[strlen(buffer)] = t9keys[key][0]; //Add to the end of the buffer
	}
	lastKey = key;
	printf("T9: %s\n", buffer);
}

void t9send(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
{
	SendKeys(buffer);
	clear();
}

void t9back(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
{
	if (strlen(buffer) > 0)
	{
		buffer[strlen(buffer) - 1] = 0; //Zero out the last character to backspace
	}
	else
	{
		SendVK(VK_BACK);
	}
}

void t9clear(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
{
	clear();
}

void clear()
{
	memset(buffer, 0, sizeof(buffer));
}

void registerT9Actions()
{
	registerActionCallback(&t9, "t9");
	registerActionCallback(&t9send, "t9Send");
	registerActionCallback(&t9back, "t9Back");
}