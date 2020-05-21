#ifndef __Mouse_h
#define __Mouse_h
#include "RemoteListener.h"
#include "Actions.h"

void sendButtonPress(int button);
void sendButtonRelease(int button);
void sendCursorPosition(int dX, int dY, bool absolute);
void sendScroll(int Delta);
void pressButton(unsigned int button);
void releaseButton(unsigned int button);
void mouseCycleButtons(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat);
void mouseClick(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat);
void mouseHold(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat);
void mouseUp(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat);
void mouseDown(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat);
void mouseLeft(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat);
void mouseRight(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat);
void scrollUp(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat);
void scrollDown(const char* repeat, const char* keycode, const char* param, const char* mode, const char* mode_type, int numRepeat);
void registerMouseActions();
#endif