#include "Mouse.h"

#define stepMultiplier 2

unsigned int buttonState = 0;
unsigned int currentButton = 0;

void sendButtonPress(int button)
{
	INPUT Input;
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = 2 << (2 * button);
	Input.mi.dx = 0;
	Input.mi.dy = 0;
	Input.mi.time = 0;
	Input.mi.dwExtraInfo = 0;
	Input.mi.mouseData = 0;
	SendInput(1, &Input, sizeof(Input));
}

void sendButtonRelease(int button)
{
	INPUT Input;
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = 4 << (2 * button);
	Input.mi.dx = 0;
	Input.mi.dy = 0;
	Input.mi.time = 0;
	Input.mi.dwExtraInfo = 0;
	Input.mi.mouseData = 0;
	SendInput(1, &Input, sizeof(Input));
}

void sendCursorPosition(int dX, int dY, bool absolute)
{
	INPUT Input;
	Input.type = INPUT_MOUSE;
	Input.mi.dx = dX;
	Input.mi.dy = dY;
	Input.mi.mouseData = 0;
	Input.mi.time = 0;
	Input.mi.dwFlags = MOUSEEVENTF_MOVE | (absolute?MOUSEEVENTF_ABSOLUTE:0);

	SendInput(1, &Input, sizeof(Input));
}

void sendScroll(int Delta)
{
	INPUT Input;
	Input.type = INPUT_MOUSE;
	Input.mi.dx = 0;
	Input.mi.dy = 0;
	Input.mi.dwFlags = MOUSEEVENTF_WHEEL;
	Input.mi.time = 0;
	Input.mi.dwExtraInfo = 0;
	Input.mi.mouseData = Delta * 120;
	SendInput(1, &Input, sizeof(Input));
}


void pressButton(unsigned int button)
{
	buttonState = buttonState | 1 << button;
	sendButtonPress(button);
}

void releaseButton(unsigned int button)
{
	buttonState = buttonState & ~(1 << button);
	sendButtonRelease(button);
}

void mouseCycleButtons(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	currentButton = (currentButton + 1) % 3;
	switch (currentButton)
	{
	case 0: //Left Click
		Beep(750 + (100 * currentButton), 200);
		Beep(550 + (100 * currentButton), 150);
		break;
	case 1: //Middle Click
		Beep(550 + (100 * currentButton), 150);
		Beep(750 + (100 * currentButton), 200);
		break;
	case 2: //Right Click
		Beep(650 + (100 * currentButton), 175);
		Beep(650 + (100 * currentButton), 175);
		break;
	}

}

void mouseClick(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	if (!(buttonState & (1 << currentButton))) //buttonState is a bitmask for each button, check if the button is not pressed before pressing it.
	{
		pressButton(currentButton);
		Sleep(50);
	}
	releaseButton(currentButton);
}

void mouseHold(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	if (!(buttonState & (1 << currentButton)))
	{
		pressButton(currentButton);
	}
}

void mouseUp(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	sendCursorPosition(0,0 - numRepeat * stepMultiplier, false);
}

void mouseDown(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	sendCursorPosition(0, numRepeat * stepMultiplier, false);
}

void mouseLeft(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	sendCursorPosition(0 - numRepeat * stepMultiplier, 0, false);
}

void mouseRight(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	sendCursorPosition(numRepeat * stepMultiplier, 0, false);
}

void scrollUp(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	sendScroll(numRepeat);
}

void scrollDown(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat)
{
	sendScroll(-numRepeat);
}

void registerMouseActions()
{
	registerActionCallback(&mouseUp, "MouseUp");
	registerActionCallback(&mouseDown, "MouseDown");
	registerActionCallback(&mouseLeft, "MouseLeft");
	registerActionCallback(&mouseRight, "MouseRight");
	registerActionCallback(&mouseUp, "ScreensOn");
	registerActionCallback(&mouseCycleButtons, "MouseCycleButtons");
	registerActionCallback(&scrollUp, "ScrollUp");
	registerActionCallback(&scrollDown, "ScrollDown");
	registerActionCallback(&mouseClick, "MouseClick");
	registerActionCallback(&mouseHold, "MouseHold");
}