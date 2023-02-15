This is an application that connects to either a remote LIRC server or a remote Magic4PC server and translates the remote buttons it receives into actions on a client. There are project files are for Visual Studio 2019 Community Edition as well as a Linux Makefile.

The Config_lirc.xml file holds a sample LIRC remote layout.
The Config_magic4pc.xml file holds a sample magic4pc remote layout.


Valid actions:

| Function | Keycode | Params | Description |
| --- | --- | --- | --- |
| MouseUp | N/A | N/A | Moves the Mouse Up †|
| MouseDown | N/A | N/A | Moves the Mouse Down †|
| MouseLeft | N/A | N/A | Moves the Mouse Left †|
| MouseRight | N/A | N/A | Moves the Mouse Right †|
| ScreensOn | N/A | N/A | Moves the Mouse Up (to wake the screens) †|
| MouseCycleButtons | N/A | N/A | Cycles the button the mouse will press †|
| ScrollUp | N/A | N/A | Scrolls Mouse Wheel up †|
| ScrollDown | N/A | N/A | Scrolls Mouse Wheel down †|
| MouseClick | N/A | N/A | Clicks Mouse †|
| MouseClickDown | N/A | N/A | Presses Mouse †|
| MouseClickUp | N/A | N/A | Releases Mouse †|
| MouseHold | N/A | N/A | Holds Mouse Click †|
| t9 | 0-9 | N/A | Presses a T9 key †|
| t9Send| N/A | N/A | Sends the current T9 string †|
| t9Back | N/A | N/A |Backspaces one character on the current T9 string †|
| SendVK | Friendly VK_KEY name (Eg. VK_ENTER) | N/A | Sends a keypress then a release †|
| ToggleVK| Friendly VK_KEY | N/A | Toggles the state of a key †|
| ModeCycle | N/A | Mode Type | Cycles between modes |
| Run | N/A | Command to be executed | Runs a commandline |
| ToggleScreens | N/A | N/A | Toggles the screens between Extended  and One Screen †|
| ScreensOff | N/A | N/A | Turns the monitors off and locks the workstation †|
| WebRequest | Page | Host | Sends a web request and discards the response |
| SendPacket | Packet | Host | Sends a TCP packet to the destination and waits for one response packet |
| ModeSet | Mode Type | Mode | Sets the current Mode for Mode Type to Mode. |
| ProjectorPowerToggle | N/A | Host IP | Queries an EIKI EIP-U4700 projector and toggles it's power state. May work on other projectors.


†Emulating the keyboard/mouse is Windows only.

There are additional parameters, Mode, ModeType, Repeat, and OS. These can be used on any action.
If Mode is specified the action will only run if the ModeType is in the right mode, such as only running if Input is in Keyboard mode. Additional modes can be added under the Modes section, additional ModeTypes can be added by adding additional Modes sections.

If Repeat is specified it will only run on that repeat. For example, in the LIRC configuration MouseHold is ran after 5 repeats indicating the remote has been held for a couple of seconds.
Repeat by default skips the 1st and 2nd repeats to avoid bouncing. Repeat can be set to 'A' to always run.

The Button Name is the name of the button passed from LIRC or the button ID from Magic4PC, the Device Name is the name of the device.

OS can be either not provided to always run, or set to 'Windows' or 'Linux'.

Magic4PC will also fire additional events, such as 13_up, for when the button is released. It also fires mousedown and mouseup actions for when the mouse is clicked, and wheeldown and wheelup actions for when the wheel is scrolled.

The server config is in the Config.xml.

There is no limit on the number of devices, number of buttons per device, or number of actions per button. There is also no limit on the number of modes or mode types.

This project uses RapidXML for it's XML parsing. See http://rapidxml.sourceforge.net/ for more information. I haven't modified RapidXML at all.

This project uses nlohmann json https://github.com/nlohmann/json

This project uses this random base64 library for base64: https://gist.github.com/tomykaira/f0fd86b6c73063283afe550bc5d77594

T9 works like T9 on a phone. Here are the T9 keys:
```
" ,.0?!'\\`\":;"
1: "@*1/-+=#$%^&~|(){}[]"
2: "abc2ABC"
3: "def3DEF"
4: "ghi4GHI"
5: "jkl5JKL"
6: "mno6MNO"
7: "pqrs7PRQS"
8: "tuv8TUV"
9: "wxyz9WXYZ"
```

WebRequest takes the Page for the Keycode, such as "/index.php" and a server for the Params, such as "192.168.0.82" or "google.com".

SendPacket takes the packet to send for the Keycode, such as "POWR___1" and a server for the Params.
