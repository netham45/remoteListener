#include "RemoteListener.h"
#include "Keyboard.h"


bool keyStates[255] = { false };

void SendKeys(const char* keys)
{
	for (unsigned int i = 0; i < strlen(keys); i++)
	{
		SendKey(keys[i]);
	}
}

void SendKey(char key)
{
	short vKey = (SHORT)VkKeyScanA(key);
	PressKey(vKey);
	ReleaseKey(vKey);
}

void PressKey(short vKey)
{
	INPUT input[10];
	int inputHead = 0;
	char modifiers = HIBYTE(vKey);
	if (modifiers & 1)
		makeInput(&input[inputHead++], VK_LSHIFT, 0);
	if (modifiers & 2)
		makeInput(&input[inputHead++], VK_LCONTROL, 0);
	if (modifiers & 4)
		makeInput(&input[inputHead++], VK_LMENU, 0);
	makeInput(&input[inputHead++], vKey, 0);
	SendInput(inputHead, input, sizeof(INPUT));
}

void ReleaseKey(short vKey)
{
	INPUT input[10];
	int inputHead = 0;
	char modifiers = HIBYTE(vKey);
	makeInput(&input[inputHead++], vKey, KEYEVENTF_KEYUP);
	if (modifiers & 1)
		makeInput(&input[inputHead++], VK_LSHIFT, KEYEVENTF_KEYUP);
	if (modifiers & 2)
		makeInput(&input[inputHead++], VK_LCONTROL, KEYEVENTF_KEYUP);
	if (modifiers & 4)
		makeInput(&input[inputHead++], VK_LMENU, KEYEVENTF_KEYUP);
	SendInput(inputHead, input, sizeof(INPUT));
}

void makeInput(INPUT *input,unsigned char wVk, DWORD dwFlags)
{
	input->type = INPUT_KEYBOARD;
	input->ki.wScan = 0;
	input->ki.time = 0;
	input->ki.dwExtraInfo = 0;

	input->ki.wVk = wVk;
	input->ki.dwFlags = dwFlags;
}


void SendVK_CB(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
{
	SendVK(vkFromFriendlyName(keycode));
}

void SendVK(const unsigned char vKey)
{
	PressKey(vKey);
	ReleaseKey(vKey);
	keyStates[vKey] = false;
}

void ToggleVK_CB(const char* repeat, const char* keycode, const char* param, const char* mode, int numRepeat)
{
	ToggleVK(vkFromFriendlyName(keycode));
}

void ToggleVK(char vKey)
{
	if (keyStates[vKey]) //If it's currently pressed
	{
		ReleaseKey(vKey);
	}
	else //If it's not pressed
	{
		PressKey(vKey);
	}
	keyStates[vKey] = !keyStates[vKey]; //Invert the state
}

unsigned char vkFromFriendlyName(const char* friendlyName)
{
	//TODO: Something better for this
	if (strcmp(friendlyName, "VK_LBUTTON") == 0)
	{
		return VK_LBUTTON;
	}
	if (strcmp(friendlyName, "VK_RBUTTON") == 0)
	{
		return VK_RBUTTON;
	}
	if (strcmp(friendlyName, "VK_CANCEL") == 0)
	{
		return VK_CANCEL;
	}
	if (strcmp(friendlyName, "VK_MBUTTON") == 0)
	{
		return VK_MBUTTON;
	}
	if (strcmp(friendlyName, "VK_XBUTTON1") == 0)
	{
		return VK_XBUTTON1;
	}
	if (strcmp(friendlyName, "VK_XBUTTON2") == 0)
	{
		return VK_XBUTTON2;
	}
	if (strcmp(friendlyName, "VK_BACK") == 0)
	{
		return VK_BACK;
	}
	if (strcmp(friendlyName, "VK_TAB") == 0)
	{
		return VK_TAB;
	}
	if (strcmp(friendlyName, "VK_CLEAR") == 0)
	{
		return VK_CLEAR;
	}
	if (strcmp(friendlyName, "VK_RETURN") == 0)
	{
		return VK_RETURN;
	}
	if (strcmp(friendlyName, "VK_SHIFT") == 0)
	{
		return VK_SHIFT;
	}
	if (strcmp(friendlyName, "VK_CONTROL") == 0)
	{
		return VK_CONTROL;
	}
	if (strcmp(friendlyName, "VK_MENU") == 0)
	{
		return VK_MENU;
	}
	if (strcmp(friendlyName, "VK_PAUSE") == 0)
	{
		return VK_PAUSE;
	}
	if (strcmp(friendlyName, "VK_CAPITAL") == 0)
	{
		return VK_CAPITAL;
	}
	if (strcmp(friendlyName, "VK_KANA") == 0)
	{
		return VK_KANA;
	}
	if (strcmp(friendlyName, "VK_HANGEUL") == 0)
	{
		return VK_HANGEUL;
	}
	if (strcmp(friendlyName, "VK_HANGUL") == 0)
	{
		return VK_HANGUL;
	}
	if (strcmp(friendlyName, "VK_JUNJA") == 0)
	{
		return VK_JUNJA;
	}
	if (strcmp(friendlyName, "VK_FINAL") == 0)
	{
		return VK_FINAL;
	}
	if (strcmp(friendlyName, "VK_HANJA") == 0)
	{
		return VK_HANJA;
	}
	if (strcmp(friendlyName, "VK_KANJI") == 0)
	{
		return VK_KANJI;
	}
	if (strcmp(friendlyName, "VK_ESCAPE") == 0)
	{
		return VK_ESCAPE;
	}
	if (strcmp(friendlyName, "VK_CONVERT") == 0)
	{
		return VK_CONVERT;
	}
	if (strcmp(friendlyName, "VK_NONCONVERT") == 0)
	{
		return VK_NONCONVERT;
	}
	if (strcmp(friendlyName, "VK_ACCEPT") == 0)
	{
		return VK_ACCEPT;
	}
	if (strcmp(friendlyName, "VK_MODECHANGE") == 0)
	{
		return VK_MODECHANGE;
	}
	if (strcmp(friendlyName, "VK_SPACE") == 0)
	{
		return VK_SPACE;
	}
	if (strcmp(friendlyName, "VK_PRIOR") == 0)
	{
		return VK_PRIOR;
	}
	if (strcmp(friendlyName, "VK_NEXT") == 0)
	{
		return VK_NEXT;
	}
	if (strcmp(friendlyName, "VK_END") == 0)
	{
		return VK_END;
	}
	if (strcmp(friendlyName, "VK_HOME") == 0)
	{
		return VK_HOME;
	}
	if (strcmp(friendlyName, "VK_LEFT") == 0)
	{
		return VK_LEFT;
	}
	if (strcmp(friendlyName, "VK_UP") == 0)
	{
		return VK_UP;
	}
	if (strcmp(friendlyName, "VK_RIGHT") == 0)
	{
		return VK_RIGHT;
	}
	if (strcmp(friendlyName, "VK_DOWN") == 0)
	{
		return VK_DOWN;
	}
	if (strcmp(friendlyName, "VK_SELECT") == 0)
	{
		return VK_SELECT;
	}
	if (strcmp(friendlyName, "VK_PRINT") == 0)
	{
		return VK_PRINT;
	}
	if (strcmp(friendlyName, "VK_EXECUTE") == 0)
	{
		return VK_EXECUTE;
	}
	if (strcmp(friendlyName, "VK_SNAPSHOT") == 0)
	{
		return VK_SNAPSHOT;
	}
	if (strcmp(friendlyName, "VK_INSERT") == 0)
	{
		return VK_INSERT;
	}
	if (strcmp(friendlyName, "VK_DELETE") == 0)
	{
		return VK_DELETE;
	}
	if (strcmp(friendlyName, "VK_HELP") == 0)
	{
		return VK_HELP;
	}
	if (strcmp(friendlyName, "VK_LWIN") == 0)
	{
		return VK_LWIN;
	}
	if (strcmp(friendlyName, "VK_RWIN") == 0)
	{
		return VK_RWIN;
	}
	if (strcmp(friendlyName, "VK_APPS") == 0)
	{
		return VK_APPS;
	}
	if (strcmp(friendlyName, "VK_SLEEP") == 0)
	{
		return VK_SLEEP;
	}
	if (strcmp(friendlyName, "VK_NUMPAD0") == 0)
	{
		return VK_NUMPAD0;
	}
	if (strcmp(friendlyName, "VK_NUMPAD1") == 0)
	{
		return VK_NUMPAD1;
	}
	if (strcmp(friendlyName, "VK_NUMPAD2") == 0)
	{
		return VK_NUMPAD2;
	}
	if (strcmp(friendlyName, "VK_NUMPAD3") == 0)
	{
		return VK_NUMPAD3;
	}
	if (strcmp(friendlyName, "VK_NUMPAD4") == 0)
	{
		return VK_NUMPAD4;
	}
	if (strcmp(friendlyName, "VK_NUMPAD5") == 0)
	{
		return VK_NUMPAD5;
	}
	if (strcmp(friendlyName, "VK_NUMPAD6") == 0)
	{
		return VK_NUMPAD6;
	}
	if (strcmp(friendlyName, "VK_NUMPAD7") == 0)
	{
		return VK_NUMPAD7;
	}
	if (strcmp(friendlyName, "VK_NUMPAD8") == 0)
	{
		return VK_NUMPAD8;
	}
	if (strcmp(friendlyName, "VK_NUMPAD9") == 0)
	{
		return VK_NUMPAD9;
	}
	if (strcmp(friendlyName, "VK_MULTIPLY") == 0)
	{
		return VK_MULTIPLY;
	}
	if (strcmp(friendlyName, "VK_ADD") == 0)
	{
		return VK_ADD;
	}
	if (strcmp(friendlyName, "VK_SEPARATOR") == 0)
	{
		return VK_SEPARATOR;
	}
	if (strcmp(friendlyName, "VK_SUBTRACT") == 0)
	{
		return VK_SUBTRACT;
	}
	if (strcmp(friendlyName, "VK_DECIMAL") == 0)
	{
		return VK_DECIMAL;
	}
	if (strcmp(friendlyName, "VK_DIVIDE") == 0)
	{
		return VK_DIVIDE;
	}
	if (strcmp(friendlyName, "VK_F1") == 0)
	{
		return VK_F1;
	}
	if (strcmp(friendlyName, "VK_F2") == 0)
	{
		return VK_F2;
	}
	if (strcmp(friendlyName, "VK_F3") == 0)
	{
		return VK_F3;
	}
	if (strcmp(friendlyName, "VK_F4") == 0)
	{
		return VK_F4;
	}
	if (strcmp(friendlyName, "VK_F5") == 0)
	{
		return VK_F5;
	}
	if (strcmp(friendlyName, "VK_F6") == 0)
	{
		return VK_F6;
	}
	if (strcmp(friendlyName, "VK_F7") == 0)
	{
		return VK_F7;
	}
	if (strcmp(friendlyName, "VK_F8") == 0)
	{
		return VK_F8;
	}
	if (strcmp(friendlyName, "VK_F9") == 0)
	{
		return VK_F9;
	}
	if (strcmp(friendlyName, "VK_F10") == 0)
	{
		return VK_F10;
	}
	if (strcmp(friendlyName, "VK_F11") == 0)
	{
		return VK_F11;
	}
	if (strcmp(friendlyName, "VK_F12") == 0)
	{
		return VK_F12;
	}
	if (strcmp(friendlyName, "VK_F13") == 0)
	{
		return VK_F13;
	}
	if (strcmp(friendlyName, "VK_F14") == 0)
	{
		return VK_F14;
	}
	if (strcmp(friendlyName, "VK_F15") == 0)
	{
		return VK_F15;
	}
	if (strcmp(friendlyName, "VK_F16") == 0)
	{
		return VK_F16;
	}
	if (strcmp(friendlyName, "VK_F17") == 0)
	{
		return VK_F17;
	}
	if (strcmp(friendlyName, "VK_F18") == 0)
	{
		return VK_F18;
	}
	if (strcmp(friendlyName, "VK_F19") == 0)
	{
		return VK_F19;
	}
	if (strcmp(friendlyName, "VK_F20") == 0)
	{
		return VK_F20;
	}
	if (strcmp(friendlyName, "VK_F21") == 0)
	{
		return VK_F21;
	}
	if (strcmp(friendlyName, "VK_F22") == 0)
	{
		return VK_F22;
	}
	if (strcmp(friendlyName, "VK_F23") == 0)
	{
		return VK_F23;
	}
	if (strcmp(friendlyName, "VK_F24") == 0)
	{
		return VK_F24;
	}
	if (strcmp(friendlyName, "VK_NAVIGATION_VIEW") == 0)
	{
		return VK_NAVIGATION_VIEW;
	}
	if (strcmp(friendlyName, "VK_NAVIGATION_MENU") == 0)
	{
		return VK_NAVIGATION_MENU;
	}
	if (strcmp(friendlyName, "VK_NAVIGATION_UP") == 0)
	{
		return VK_NAVIGATION_UP;
	}
	if (strcmp(friendlyName, "VK_NAVIGATION_DOWN") == 0)
	{
		return VK_NAVIGATION_DOWN;
	}
	if (strcmp(friendlyName, "VK_NAVIGATION_LEFT") == 0)
	{
		return VK_NAVIGATION_LEFT;
	}
	if (strcmp(friendlyName, "VK_NAVIGATION_RIGHT") == 0)
	{
		return VK_NAVIGATION_RIGHT;
	}
	if (strcmp(friendlyName, "VK_NAVIGATION_ACCEPT") == 0)
	{
		return VK_NAVIGATION_ACCEPT;
	}
	if (strcmp(friendlyName, "VK_NAVIGATION_CANCEL") == 0)
	{
		return VK_NAVIGATION_CANCEL;
	}
	if (strcmp(friendlyName, "VK_NUMLOCK") == 0)
	{
		return VK_NUMLOCK;
	}
	if (strcmp(friendlyName, "VK_SCROLL") == 0)
	{
		return VK_SCROLL;
	}
	if (strcmp(friendlyName, "VK_OEM_NEC_EQUAL") == 0)
	{
		return VK_OEM_NEC_EQUAL;
	}
	if (strcmp(friendlyName, "VK_OEM_FJ_JISHO") == 0)
	{
		return VK_OEM_FJ_JISHO;
	}
	if (strcmp(friendlyName, "VK_OEM_FJ_MASSHOU") == 0)
	{
		return VK_OEM_FJ_MASSHOU;
	}
	if (strcmp(friendlyName, "VK_OEM_FJ_TOUROKU") == 0)
	{
		return VK_OEM_FJ_TOUROKU;
	}
	if (strcmp(friendlyName, "VK_OEM_FJ_LOYA") == 0)
	{
		return VK_OEM_FJ_LOYA;
	}
	if (strcmp(friendlyName, "VK_OEM_FJ_ROYA") == 0)
	{
		return VK_OEM_FJ_ROYA;
	}
	if (strcmp(friendlyName, "VK_LSHIFT") == 0)
	{
		return VK_LSHIFT;
	}
	if (strcmp(friendlyName, "VK_RSHIFT") == 0)
	{
		return VK_RSHIFT;
	}
	if (strcmp(friendlyName, "VK_LCONTROL") == 0)
	{
		return VK_LCONTROL;
	}
	if (strcmp(friendlyName, "VK_RCONTROL") == 0)
	{
		return VK_RCONTROL;
	}
	if (strcmp(friendlyName, "VK_LMENU") == 0)
	{
		return VK_LMENU;
	}
	if (strcmp(friendlyName, "VK_RMENU") == 0)
	{
		return VK_RMENU;
	}
	if (strcmp(friendlyName, "VK_BROWSER_BACK") == 0)
	{
		return VK_BROWSER_BACK;
	}
	if (strcmp(friendlyName, "VK_BROWSER_FORWARD") == 0)
	{
		return VK_BROWSER_FORWARD;
	}
	if (strcmp(friendlyName, "VK_BROWSER_REFRESH") == 0)
	{
		return VK_BROWSER_REFRESH;
	}
	if (strcmp(friendlyName, "VK_BROWSER_STOP") == 0)
	{
		return VK_BROWSER_STOP;
	}
	if (strcmp(friendlyName, "VK_BROWSER_SEARCH") == 0)
	{
		return VK_BROWSER_SEARCH;
	}
	if (strcmp(friendlyName, "VK_BROWSER_FAVORITES") == 0)
	{
		return VK_BROWSER_FAVORITES;
	}
	if (strcmp(friendlyName, "VK_BROWSER_HOME") == 0)
	{
		return VK_BROWSER_HOME;
	}
	if (strcmp(friendlyName, "VK_VOLUME_MUTE") == 0)
	{
		return VK_VOLUME_MUTE;
	}
	if (strcmp(friendlyName, "VK_VOLUME_DOWN") == 0)
	{
		return VK_VOLUME_DOWN;
	}
	if (strcmp(friendlyName, "VK_VOLUME_UP") == 0)
	{
		return VK_VOLUME_UP;
	}
	if (strcmp(friendlyName, "VK_MEDIA_NEXT_TRACK") == 0)
	{
		return VK_MEDIA_NEXT_TRACK;
	}
	if (strcmp(friendlyName, "VK_MEDIA_PREV_TRACK") == 0)
	{
		return VK_MEDIA_PREV_TRACK;
	}
	if (strcmp(friendlyName, "VK_MEDIA_STOP") == 0)
	{
		return VK_MEDIA_STOP;
	}
	if (strcmp(friendlyName, "VK_MEDIA_PLAY_PAUSE") == 0)
	{
		return VK_MEDIA_PLAY_PAUSE;
	}
	if (strcmp(friendlyName, "VK_LAUNCH_MAIL") == 0)
	{
		return VK_LAUNCH_MAIL;
	}
	if (strcmp(friendlyName, "VK_LAUNCH_MEDIA_SELECT") == 0)
	{
		return VK_LAUNCH_MEDIA_SELECT;
	}
	if (strcmp(friendlyName, "VK_LAUNCH_APP1") == 0)
	{
		return VK_LAUNCH_APP1;
	}
	if (strcmp(friendlyName, "VK_LAUNCH_APP2") == 0)
	{
		return VK_LAUNCH_APP2;
	}
	if (strcmp(friendlyName, "VK_OEM_1") == 0)
	{
		return VK_OEM_1;
	}
	if (strcmp(friendlyName, "VK_OEM_PLUS") == 0)
	{
		return VK_OEM_PLUS;
	}
	if (strcmp(friendlyName, "VK_OEM_COMMA") == 0)
	{
		return VK_OEM_COMMA;
	}
	if (strcmp(friendlyName, "VK_OEM_MINUS") == 0)
	{
		return VK_OEM_MINUS;
	}
	if (strcmp(friendlyName, "VK_OEM_PERIOD") == 0)
	{
		return VK_OEM_PERIOD;
	}
	if (strcmp(friendlyName, "VK_OEM_2") == 0)
	{
		return VK_OEM_2;
	}
	if (strcmp(friendlyName, "VK_OEM_3") == 0)
	{
		return VK_OEM_3;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_A") == 0)
	{
		return VK_GAMEPAD_A;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_B") == 0)
	{
		return VK_GAMEPAD_B;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_X") == 0)
	{
		return VK_GAMEPAD_X;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_Y") == 0)
	{
		return VK_GAMEPAD_Y;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_RIGHT_SHOULDER") == 0)
	{
		return VK_GAMEPAD_RIGHT_SHOULDER;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_LEFT_SHOULDER") == 0)
	{
		return VK_GAMEPAD_LEFT_SHOULDER;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_LEFT_TRIGGER") == 0)
	{
		return VK_GAMEPAD_LEFT_TRIGGER;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_RIGHT_TRIGGER") == 0)
	{
		return VK_GAMEPAD_RIGHT_TRIGGER;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_DPAD_UP") == 0)
	{
		return VK_GAMEPAD_DPAD_UP;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_DPAD_DOWN") == 0)
	{
		return VK_GAMEPAD_DPAD_DOWN;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_DPAD_LEFT") == 0)
	{
		return VK_GAMEPAD_DPAD_LEFT;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_DPAD_RIGHT") == 0)
	{
		return VK_GAMEPAD_DPAD_RIGHT;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_MENU") == 0)
	{
		return VK_GAMEPAD_MENU;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_VIEW") == 0)
	{
		return VK_GAMEPAD_VIEW;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON") == 0)
	{
		return VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON") == 0)
	{
		return VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_LEFT_THUMBSTICK_UP") == 0)
	{
		return VK_GAMEPAD_LEFT_THUMBSTICK_UP;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_LEFT_THUMBSTICK_DOWN") == 0)
	{
		return VK_GAMEPAD_LEFT_THUMBSTICK_DOWN;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT") == 0)
	{
		return VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_LEFT_THUMBSTICK_LEFT") == 0)
	{
		return VK_GAMEPAD_LEFT_THUMBSTICK_LEFT;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_RIGHT_THUMBSTICK_UP") == 0)
	{
		return VK_GAMEPAD_RIGHT_THUMBSTICK_UP;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN") == 0)
	{
		return VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT") == 0)
	{
		return VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT;
	}
	if (strcmp(friendlyName, "VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT") == 0)
	{
		return VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT;
	}
	if (strcmp(friendlyName, "VK_OEM_4") == 0)
	{
		return VK_OEM_4;
	}
	if (strcmp(friendlyName, "VK_OEM_5") == 0)
	{
		return VK_OEM_5;
	}
	if (strcmp(friendlyName, "VK_OEM_6") == 0)
	{
		return VK_OEM_6;
	}
	if (strcmp(friendlyName, "VK_OEM_7") == 0)
	{
		return VK_OEM_7;
	}
	if (strcmp(friendlyName, "VK_OEM_8") == 0)
	{
		return VK_OEM_8;
	}
	if (strcmp(friendlyName, "VK_OEM_AX") == 0)
	{
		return VK_OEM_AX;
	}
	if (strcmp(friendlyName, "VK_OEM_102") == 0)
	{
		return VK_OEM_102;
	}
	if (strcmp(friendlyName, "VK_ICO_HELP") == 0)
	{
		return VK_ICO_HELP;
	}
	if (strcmp(friendlyName, "VK_ICO_00") == 0)
	{
		return VK_ICO_00;
	}
	if (strcmp(friendlyName, "VK_PROCESSKEY") == 0)
	{
		return VK_PROCESSKEY;
	}
	if (strcmp(friendlyName, "VK_ICO_CLEAR") == 0)
	{
		return VK_ICO_CLEAR;
	}
	if (strcmp(friendlyName, "VK_PACKET") == 0)
	{
		return VK_PACKET;
	}
	if (strcmp(friendlyName, "VK_OEM_RESET") == 0)
	{
		return VK_OEM_RESET;
	}
	if (strcmp(friendlyName, "VK_OEM_JUMP") == 0)
	{
		return VK_OEM_JUMP;
	}
	if (strcmp(friendlyName, "VK_OEM_PA1") == 0)
	{
		return VK_OEM_PA1;
	}
	if (strcmp(friendlyName, "VK_OEM_PA2") == 0)
	{
		return VK_OEM_PA2;
	}
	if (strcmp(friendlyName, "VK_OEM_PA3") == 0)
	{
		return VK_OEM_PA3;
	}
	if (strcmp(friendlyName, "VK_OEM_WSCTRL") == 0)
	{
		return VK_OEM_WSCTRL;
	}
	if (strcmp(friendlyName, "VK_OEM_CUSEL") == 0)
	{
		return VK_OEM_CUSEL;
	}
	if (strcmp(friendlyName, "VK_OEM_ATTN") == 0)
	{
		return VK_OEM_ATTN;
	}
	if (strcmp(friendlyName, "VK_OEM_FINISH") == 0)
	{
		return VK_OEM_FINISH;
	}
	if (strcmp(friendlyName, "VK_OEM_COPY") == 0)
	{
		return VK_OEM_COPY;
	}
	if (strcmp(friendlyName, "VK_OEM_AUTO") == 0)
	{
		return VK_OEM_AUTO;
	}
	if (strcmp(friendlyName, "VK_OEM_ENLW") == 0)
	{
		return VK_OEM_ENLW;
	}
	if (strcmp(friendlyName, "VK_OEM_BACKTAB") == 0)
	{
		return VK_OEM_BACKTAB;
	}
	if (strcmp(friendlyName, "VK_ATTN") == 0)
	{
		return VK_ATTN;
	}
	if (strcmp(friendlyName, "VK_CRSEL") == 0)
	{
		return VK_CRSEL;
	}
	if (strcmp(friendlyName, "VK_EXSEL") == 0)
	{
		return VK_EXSEL;
	}
	if (strcmp(friendlyName, "VK_EREOF") == 0)
	{
		return VK_EREOF;
	}
	if (strcmp(friendlyName, "VK_PLAY") == 0)
	{
		return VK_PLAY;
	}
	if (strcmp(friendlyName, "VK_ZOOM") == 0)
	{
		return VK_ZOOM;
	}
	if (strcmp(friendlyName, "VK_NONAME") == 0)
	{
		return VK_NONAME;
	}
	if (strcmp(friendlyName, "VK_PA1") == 0)
	{
		return VK_PA1;
	}
	if (strcmp(friendlyName, "VK_OEM_CLEAR") == 0)
	{
		return VK_OEM_CLEAR;
	}
	return 0;
}

void registerKeyboardActions()
{
	registerActionCallback(&SendVK_CB, "SendVK");
	registerActionCallback(&ToggleVK_CB, "ToggleVK");
}