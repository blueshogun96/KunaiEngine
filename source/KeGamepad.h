#pragma once

#include "Ke.h"
#include "KeSystem.h"



/*
 * Gamepad button macros
 */
#define KE_GAMEPAD_BUTTON_A					0
#define KE_GAMEPAD_BUTTON_B					1
#define KE_GAMEPAD_BUTTON_X					2
#define KE_GAMEPAD_BUTTON_Y					3
#define KE_GAMEPAD_BUTTON_BACK				4
#define KE_GAMEPAD_BUTTON_GUIDE				5
#define KE_GAMEPAD_BUTTON_START				6
#define KE_GAMEPAD_BUTTON_LEFTSTICK			7
#define KE_GAMEPAD_BUTTON_RIGHTSTICK		8
#define KE_GAMEPAD_BUTTON_LEFTSHOULDER		9
#define KE_GAMEPAD_BUTTON_RIGHTSHOULDER		10
#define KE_GAMEPAD_BUTTON_DPAD_UP			11
#define KE_GAMEPAD_BUTTON_DPAD_DOWN			12
#define KE_GAMEPAD_BUTTON_DPAD_LEFT			13
#define KE_GAMEPAD_BUTTON_DPAD_RIGHT		14


/*
 * Gamepad buttons
 */
struct KeGamepadButtons
{
	uint32_t	timestamp;
	bool		pressed;
};

/*
 * Gamepad state structure
 */
struct KeGamepadState
{
	KeGamepadButtons buttons[24];
	bool			 connected;
};


/*
 * Joystick state structure
 */
struct KeJoystickState
{
	KeGamepadButtons buttons[24];
	uint32_t		 dpad;
};


/*
 * Gamepad management APIs
 */

/*
 * Name: KeInitializeGamepads
 * Desc: Initializes the gamepad API for the target platform.
 */
bool KeInitializeGamepads();


/*
 * Name: KeUnintializeGamepads
 * Desc: Uninitializes the above.
 */
void KeUninitializeGamepads();


/*
 * Name: KeGetGamepadState
 * Desc: Returns the input state of the specified gamepad
 */
bool KeGetGamepadState( int device_id, KeGamepadState* gamepad );


/*
 * Name: KeGetGamepadCount
 * Desc: Returns the number of gamepads connected
 */
int KeGetGamepadCount();


/*
 * Name: KeGetJoystickState
 * Desc: Returns the input state of the specified Joystick
 */
bool KeGetJoystickState( int device_id, KeJoystickState* Joystick );


/*
 * Name: KeGetJoystickCount
 * Desc: Returns the number of joysticks connected
 */
int KeGetJoystickCount();


/*
 * Namespaced API
 */
namespace Ke
{
    namespace Gamepad
    {
        bool (*Initialize)() = KeInitializeGamepads;
        void (*Uninitialize)() = KeUninitializeGamepads;
        bool (*GetState)( int, KeGamepadState* ) = KeGetGamepadState;
        int  (*GetCount)() = KeGetGamepadCount;
    }
    
    namespace Joystick
    {
        bool (*GetState)( int, KeJoystickState* ) = KeGetJoystickState;
        int  (*GetCount)() = KeGetJoystickCount;
    }
}
