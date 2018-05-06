/*
Omnispeak: A Commander Keen Reimplementation
Copyright (C) 2012 David Gow <david@ingeniumdigital.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "id_in.h"
#include "id_sd.h"
#include "id_us.h"
#include "id_vl.h"

#include <string.h>

char nonchar_keys[] ={
	0x01, 0x0E, 0x0F, 0x1D,
	0x2A, 0x39, 0x3A, 0x3B,
	0x3C, 0x3D, 0x3E, 0x3F,
	0x40, 0x41, 0x42, 0x43,
	0x44, 0x57, 0x59, 0x46,
	0x1C, 0x36, 0x37, 0x38,
	0x47, 0x49, 0x4F, 0x51,
	0x52, 0x53, 0x45, 0x48,
	0x50, 0x4B, 0x4D, 0x00
};
const char *nonchar_key_strings[] ={
	"Esc",		"Bksp",		"Tab",		"Ctrl",
	"LShft",	"Space",	"CapsLk",	"F1",
	"F2",		"F3",		"F4",		"F5",
	"F6",		"F7", 		"F8", 		"F9",
	"F10",		"F11",		"F12",		"ScrlLk",
	"Enter",	"RShft",	"PrtSc",	"Alt",
	"Home",		"PgUp",		"End",		"PgDn",
	"Ins",		"Del",		"NumLk",	"Up",
	"Down",		"Left",		"Right",	""
};
const char *char_key_strings[] ={
	/*	 0	 	 1	 	 2	 	 3		 4	     5	 	 6	 	7	     */
	/*	 8	 	 9	 	 A	 	 B		 C		 D 		 E		 F	     */
	"?",	"?",	"1",	"2",	"3",	"4",	"5",	"6",	/* 0 */
	"7",	"8",	"9",	"0",	"-",	"+",	"?",	"?",
	"Q",	"W",	"E",	"R",	"T",	"Y",	"U",	"I",	/* 1 */
	"O",	"P",	"[",	"]",	"|",	"?",	"A",	"S",
	"D",	"F",	"G",	"H",	"J",	"K",	"L",	";",	/* 2 */
	"\"",	"?",	"?",	"?",	"Z",	"X",	"C",	"V",
	"B",	"N",	"M",	",",	".",	"/",	"?",	"?",	/* 3 */
	"?",	"?",	"?",	"?",	"?",	"?",	"?",	"?",
	"?",	"?",	"?",	"?",	"?",	"?",	"?",	"?",	/* 4 */
	"\x0F",	"?",	"-",	"\x15",	"5",	"\x11",	"+",	"?",
	"\x13",	"?",	"?",	"?",	"?",	"?",	"?",	"?",	/* 5 */
	"?",	"?",	"?",	"?",	"?",	"?",	"?",	"?",
	"?",	"?",	"?",	"?",	"?",	"?",	"?",	"?",	/* 6 */
	"?",	"?",	"?",	"?",	"?",	"?",	"?",	"?",
	"?",	"?",	"?",	"?",	"?",	"?",	"?",	"?",	/* 7 */
	"?",	"?",	"?",	"?",	"?",	"?",	"?",	"?"
};

// =========================================================================
static	char in_ASCIINames[] =		// Unshifted ASCII for scan codes
{
	//	 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
	0  , 27 , '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 8  , 9  ,	// 0
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 13 , 0  , 'a', 's',	// 1
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', 39 , '`', 0  , 92 , 'z', 'x', 'c', 'v',	// 2
	'b', 'n', 'm', ',', '.', '/', 0  , '*', 0  , ' ', 0  , 0  , 0  , 0  , 0  , 0  ,	// 3
	0  , 0  , 0  , 0  , 0  , 0  , 0  , '7', '8', '9', '-', '4', '5', '6', '+', '1',	// 4
	'2', '3', '0', 127, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,	// 5
	0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,	// 6
	0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0		// 7
},
in_shiftNames[] =		// Shifted ASCII for scan codes
{
	//	 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
	0  , 27 , '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 8  , 9  ,	// 0
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 13 , 0  , 'A', 'S',	// 1
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', 34 , '~', 0  , '|', 'Z', 'X', 'C', 'V',	// 2
	'B', 'N', 'M', '<', '>', '?', 0  , '*', 0  , ' ', 0  , 0  , 0  , 0  , 0  , 0  ,	// 3
	0  , 0  , 0  , 0  , 0  , 0  , 0  , '7', '8', '9', '-', '4', '5', '6', '+', '1',	// 4
	'2', '3', '0', 127, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,	// 5
	0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,	// 6
	0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0   	// 7
},
in_specialNames[] =	// ASCII for 0xe0 prefixed codes
{
	//	 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
	0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,	// 0
	0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 13 , 0  , 0  , 0  ,	// 1
	0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,	// 2
	0  , 0  , 0  , 0  , 0  , '/', 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,	// 3
	0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,	// 4
	0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,	// 5
	0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,	// 6
	0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0   	// 7
};



bool in_Paused;
IN_ControlType in_controlType = IN_ctrl_Keyboard1;
bool in_keyStates[256];
IN_ScanCode in_lastKeyScanned = IN_SC_None;
char in_lastASCII;
bool in_disableJoysticks = false;
bool INL_StartJoy(int joystick);
void INL_StopJoy(int joystick);

IN_Backend *in_backend = 0;

#define IN_MAX_JOYSTICKS 2



IN_KeyMapping in_kbdControls;

int16_t in_gamepadButtons[4];

IN_ScanCode *key_controls[] ={
	&in_kbdControls.jump, &in_kbdControls.pogo, &in_kbdControls.fire,
	&in_kbdControls.upLeft, &in_kbdControls.up, &in_kbdControls.upRight,
	&in_kbdControls.right, &in_kbdControls.downRight, &in_kbdControls.down,
	&in_kbdControls.downLeft, &in_kbdControls.left
};

static	bool		CapsLock;

static IN_Direction	in_dirTable[] =		// Quick lookup for total direction
{
	IN_dir_NorthWest,	IN_dir_North,	IN_dir_NorthEast,
	IN_dir_West,		IN_dir_None,	IN_dir_East,
	IN_dir_SouthWest,	IN_dir_South,	IN_dir_SouthEast
};

void IN_HandleKeyUp(IN_ScanCode sc, bool special)
{
	//Use F11 to toggle fullscreen.
	if (sc == IN_SC_F11)
		VL_ToggleFullscreen();

	in_keyStates[sc] = 0;
}

void IN_HandleKeyDown(IN_ScanCode sc, bool special)
{
	in_keyStates[sc] = true;
	in_lastKeyScanned = sc;
	char c;

	if (sc == 0xe0)		// Special key prefix
		special = true;
	else if (sc == IN_SC_Pause)	// Handle Pause key
		in_Paused = true;
	else
	{
		if (sc & 0x80)	// Break code
		{
			sc &= 0x7f;

			// DEBUG - handle special keys: ctl-alt-delete, print scrn
			in_keyStates[sc] = false;
		}
		else			// Make code
		{
			/*
			LastCode = CurCode;
			CurCode = LastScan = sc;
			Keyboard[sc] = true;
				*/

			if (special)
				c = in_specialNames[sc];
			else
			{
				if (sc == IN_SC_CapsLock)
				{
					CapsLock ^= true;
					// DEBUG - make caps lock light work
				}

				if (in_keyStates[IN_SC_LeftShift] || in_keyStates[IN_SC_RightShift])	// If shifted
				{
					c = in_shiftNames[sc];
					if ((c >= 'A') && (c <= 'Z') && CapsLock)
						c += 'a' - 'A';
				}
				else
				{
					c = in_ASCIINames[sc];
					if ((c >= 'a') && (c <= 'z') && CapsLock)
						c -= 'a' - 'A';
				}
			}
			if (c)
				in_lastASCII = c;
		}
	}
}

static void INL_SetupKbdControls()
{
	in_kbdControls.jump = IN_SC_Control;
	in_kbdControls.pogo = IN_SC_Alt;
	in_kbdControls.fire = IN_SC_Space;
	in_kbdControls.up = IN_SC_UpArrow;
	in_kbdControls.down = IN_SC_DownArrow;
	in_kbdControls.left = IN_SC_LeftArrow;
	in_kbdControls.right = IN_SC_RightArrow;
	in_kbdControls.upLeft = IN_SC_Home;
	in_kbdControls.upRight = IN_SC_PgUp;
	in_kbdControls.downRight = IN_SC_PgDown;
	in_kbdControls.downLeft = IN_SC_End;
}

void IN_PumpEvents()
{
	in_backend->pumpEvents();
}

const char *IN_GetScanName ( IN_ScanCode scan)
{
	char *p;
	const char **ps;

	p = nonchar_keys;
	ps = nonchar_key_strings;

	while ( *p)
	{
		if ( *p == scan )
			return *ps;

		ps++;
		p++;
	}
	return char_key_strings [scan];
}

void IN_WaitKey()
{
	in_backend->waitKey();
}

//void IN_WaitASCII() {}

void IN_WaitForButtonPress()
{

	while (1)
	{
		IN_PumpEvents();

		if (in_lastKeyScanned)
		{
			in_keyStates[in_lastKeyScanned] = 0;

			if (in_lastKeyScanned == in_lastKeyScanned)
				in_lastKeyScanned = 0;

			in_lastKeyScanned = 0;
			return;
		}

	#if 0
		if (MousePresent)
		{
			if (INL_GetMouseButtons())
				while (INL_GetMouseButtons())
					;

			return;
		}

	#endif
		for (int i = 0; i < IN_MAX_JOYSTICKS; i++)
		{
			if (IN_JoyPresent(i))
			{
				if (IN_GetJoyButtonsDB(i))
				{
					while (IN_GetJoyButtonsDB(i))
					{
						IN_PumpEvents();
						VL_Present();
					}
					return;
				}
			}
		}
		VL_Present();
	}
}

bool IN_GetKeyState(IN_ScanCode scanCode)
{
	return in_keyStates[scanCode];
}

IN_ScanCode IN_GetLastScan(void)
{
	return in_lastKeyScanned;
}

void IN_SetLastScan(IN_ScanCode scanCode)
{
	in_lastKeyScanned = scanCode;
}

char IN_GetLastASCII(void)
{
	return in_lastASCII;
}

void IN_SetLastASCII(char c)
{
	in_lastASCII = c;
}

void IN_Startup(void)
{
	in_backend = IN_Impl_GetBackend();
	for (int i = 0; i < 256; ++i)
		in_keyStates[i] = 0;

	// Set the default kbd controls.
	INL_SetupKbdControls();
	
	in_backend->startup(in_disableJoysticks);
}

// TODO: IMPLEMENT!
void IN_Default(bool gotit,int16_t inputChoice)
{
	in_controlType = (IN_ControlType)inputChoice;
}

uint8_t *in_demoBuf;
int in_demoPtr;
int in_demoBytes;
IN_DemoMode in_demoState;

void IN_DemoStartPlaying(uint8_t *data, int len)
{
	in_demoBuf = data;
	in_demoBytes = len;
	in_demoPtr = 0;
	in_demoState = IN_Demo_Playback;
}

void IN_DemoStopPlaying()
{
	in_demoState = IN_Demo_Off;
}

#if 0
bool IN_DemoIsPlaying()
{
	return in_demoState == IN_Demo_Playback;
}
#endif

IN_DemoMode IN_DemoGetMode()
{
	return in_demoState;
}

void IN_ClearKeysDown()
{
	in_lastKeyScanned = IN_SC_None;
	in_lastASCII = IN_KP_None;
	memset (in_keyStates, 0, sizeof (in_keyStates));
}

void CK_US_UpdateOptionsMenus( void );

bool INL_StartJoy(int joystick)
{
	if (in_disableJoysticks)
		return false;
	
	in_backend->joyStart(joystick);
	
	CK_US_UpdateOptionsMenus();
	
	return true;
}

void INL_StopJoy(int joystick)
{
	in_backend->joyStop(joystick);

	// If the joystick is unplugged, switch to keyboard immediately.
	if (in_controlType == IN_ctrl_Joystick1 + joystick)
		in_controlType = IN_ctrl_Keyboard1;
	
	CK_US_UpdateOptionsMenus();
	
}

bool IN_JoyPresent(int joystick)
{
	return in_backend->joyPresent(joystick);
}

void IN_GetJoyAbs(int joystick, int *x, int *y)
{
	in_backend->joyGetAbs(joystick, x, y);
}

uint16_t IN_GetJoyButtonsDB(int joystick)
{
	in_backend->joyGetButtons(joystick);
}

void IN_SetControlType(int player, IN_ControlType type)
{
	in_controlType = type;
}

void IN_ReadCursor(IN_Cursor *cursor)
{
	cursor->button0 = false;
	cursor->button1 = false;
	cursor->xMotion = IN_motion_None;
	cursor->yMotion = IN_motion_None;
	if (in_controlType == IN_ctrl_Joystick1 || in_controlType == IN_ctrl_Joystick2)
	{
		int joy = in_controlType - IN_ctrl_Joystick1;
		int rawX, rawY;
		IN_GetJoyAbs(joy, &rawX, &rawY);
		cursor->xMotion = IN_motion_None;
		if (rawX < 0)
			cursor->xMotion = IN_motion_Left;
		else if (rawX > 0)
			cursor->xMotion = IN_motion_Right;
		cursor->yMotion = IN_motion_None;
		if (rawY < 0)
			cursor->yMotion = IN_motion_Up;
		else if (rawY > 0)
			cursor->yMotion = IN_motion_Down;
		
		uint16_t buttons = IN_GetJoyButtonsDB(joy);
		cursor->button0 = buttons & 1;
		cursor->button1 = buttons & 2;
	}
}

void IN_ReadControls(int player, IN_ControlFrame *controls)
{
	controls->xDirection = IN_motion_None;
	controls->yDirection = IN_motion_None;
	controls->jump = false;
	controls->pogo = false;
	controls->button2 = false;

	if (in_demoState == IN_Demo_Playback)
	{
		uint8_t ctrlByte = in_demoBuf[in_demoPtr + 1];
		controls->yDirection = (IN_Motion) ((ctrlByte & 3) - 1);
		controls->xDirection = (IN_Motion) (((ctrlByte >> 2) & 3) - 1);
		controls->jump = (ctrlByte >> 4) & 1;
		controls->pogo = (ctrlByte >> 5) & 1;

		// Delay for n frames.
		if ((--in_demoBuf[in_demoPtr]) == 0 )
		{
			in_demoPtr += 2;
			if (in_demoPtr >= in_demoBytes)
				in_demoState = IN_Demo_PlayDone;
		}
	}
	else if (in_demoState == IN_Demo_PlayDone)
	{
		Quit("Demo playback exceeded");
	}
	else
	{
		if (in_controlType == IN_ctrl_Keyboard1 || in_controlType == IN_ctrl_Keyboard2)
		{
		if (IN_GetKeyState(in_kbdControls.upLeft))
		{
			controls->xDirection = IN_motion_Left;
			controls->yDirection = IN_motion_Up;
		}
		else if (IN_GetKeyState(in_kbdControls.upRight))
		{
			controls->xDirection = IN_motion_Right;
			controls->yDirection = IN_motion_Up;
		}
		else if (IN_GetKeyState(in_kbdControls.downLeft))
		{
			controls->xDirection = IN_motion_Left;
			controls->yDirection = IN_motion_Down;
		}
		else if (IN_GetKeyState(in_kbdControls.downRight))
		{
			controls->xDirection = IN_motion_Right;
			controls->yDirection = IN_motion_Down;
		}

		if (IN_GetKeyState(in_kbdControls.up)) controls->yDirection = IN_motion_Up;
		else if (IN_GetKeyState(in_kbdControls.down)) controls->yDirection = IN_motion_Down;

		if (IN_GetKeyState(in_kbdControls.left)) controls->xDirection = IN_motion_Left;
		else if (IN_GetKeyState(in_kbdControls.right)) controls->xDirection = IN_motion_Right;

		if (IN_GetKeyState(in_kbdControls.jump)) controls->jump = true;
		if (IN_GetKeyState(in_kbdControls.pogo)) controls->pogo = true;
		// TODO: Handle fire input separately? (e.g. two-button firing)
		if (IN_GetKeyState(in_kbdControls.fire)) controls->button2 = true;
		}
		else if (in_controlType == IN_ctrl_Joystick1 || in_controlType == IN_ctrl_Joystick2)
		{
			int joy = in_controlType - IN_ctrl_Joystick1;
			int rawX, rawY;
			IN_GetJoyAbs(joy, &rawX, &rawY);
			controls->xDirection = IN_motion_None;
			if (rawX < 0)
				controls->xDirection = IN_motion_Left;
			else if (rawX > 0)
				controls->xDirection = IN_motion_Right;
			controls->yDirection = IN_motion_None;
			if (rawY < 0)
				controls->yDirection = IN_motion_Up;
			else if (rawY > 0)
				controls->yDirection = IN_motion_Down;
			
			uint16_t buttons = IN_GetJoyButtonsDB(joy);
			controls->jump = buttons & 1;
			controls->pogo = buttons & 2;
		}

		controls->dir = in_dirTable[3 * (controls->yDirection + 1) + controls->xDirection + 1];
		
	}

	//TODO: Record this inputFrame
}

void IN_WaitButton()
{
	// Zero all of the input
	IN_PumpEvents();

	in_keyStates[in_lastKeyScanned] = 0;

	in_lastKeyScanned = 0;

#if 0
	if (MousePresent)
	{
		while (INL_GetMouseButtons())
			;
	}

	for (int i = 0; i < 2; i++)
	{
		if (JoyPresent[i] || Gamepad)
			while (IN_GetJoyButtonsDB(i))
				;
	}
#endif

	// Now wait for a button press
	IN_WaitForButtonPress();

}

// TODO: Handle Joy/Mouse
int IN_CheckAck()
{
	int di;

	di = IN_GetLastScan();

#if 0
	if (MousePresent && INL_GetMouseButtons())
		di = 1;
#endif

	for (int i = 0; i < IN_MAX_JOYSTICKS; i++)
	{
		if (IN_JoyPresent(i))
			if (IN_GetJoyButtonsDB(i))
				di = 1;
	}


	return di;

}

bool IN_UserInput(int tics, bool waitPress)
{
	int lasttime = SD_GetTimeCount();

	do
	{
		IN_PumpEvents();

		if (IN_CheckAck())
		{
			if (waitPress)
				IN_WaitForButtonPress();

			return true;

		}
		VL_Present();
	} while (SD_GetTimeCount() - lasttime < tics);

	return false;
}
