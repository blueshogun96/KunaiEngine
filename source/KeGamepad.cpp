#include "KeGamepad.h"
#include <vector>


/* Gamepad handle */
class CKeGamepadHandle
{
public:
	CKeGamepadHandle() : game_controller(NULL), haptic(NULL), joystick_id(0), id(0), connected(No) {}
	~CKeGamepadHandle()
	{
		if( haptic )
			SDL_HapticClose( haptic );

		SDL_GameControllerClose( game_controller );
	}

	SDL_GameController* game_controller;
	SDL_Haptic*			haptic;
	SDL_JoystickID		joystick_id;
	KeGamepadState		state;
	int					id;
	bool				connected;
};

/* Gamepad handles */
std::vector<CKeGamepadHandle> GamepadHandles;


/*
 * Name: KeInitializeGamepads
 * Desc: Initializes the gamepad API for the target platform.
 */
bool KeInitializeGamepads()
{
	int ret = 0;

	/* Initialize SDL2 gamepad API */
	ret = SDL_InitSubSystem( SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC );
	if( ret != 0 )
		DISPDBG( KE_ERROR, "An error has occured initializing game controller and haptics!" );

	return ret;
}


/*
 * Name: KeUnintializeGamepads
 * Desc: Uninitializes the above.
 */
void KeUninitializeGamepads()
{
	/* Kill all open gamepad devices */
	GamepadHandles.empty();

	/* Uninitialize the gamepad API */
	SDL_QuitSubSystem( SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC );
}


/*
 * Name: KeOnGamepadAdded
 * Desc: Called when a gamepad has been added/attached.
 */
void KeOnGamepadAdded( int device_id )
{
	CKeGamepadHandle handle;

	/* Attempt to open this device */
	handle.game_controller = SDL_GameControllerOpen( device_id );

	/* Get the joystick instance ID */
	SDL_Joystick* j = SDL_GameControllerGetJoystick( handle.game_controller );
	handle.joystick_id = SDL_JoystickInstanceID(j);
	handle.connected = Yes;
	handle.id = device_id;

	ZeroMemory( &handle.state, sizeof( KeGamepadState ) );

	/* Is this a haptic device? */
	if( SDL_JoystickIsHaptic(j) )
	{
		handle.haptic = SDL_HapticOpenFromJoystick(j);

		/* Is this a rumble supported haptic feature? */
		if( SDL_HapticRumbleSupported( handle.haptic ) )
		{
			/* Can we initialize the rumble feature? */
			if( SDL_HapticRumbleInit( handle.haptic ) )
			{
				/* An error occurred */
				SDL_HapticClose( handle.haptic );
				handle.haptic = 0;

				DISPDBG( KE_WARNING, "Error initializing rumble feature for gamepad! "
					"Device ID:" << device_id <<
					"Joystick ID: " << handle.joystick_id << 
					"Error code:" << SDL_GetError() );
			}
		}
		else
		{
			/* This is not a rumble haptic */
			SDL_HapticClose( handle.haptic );
			handle.haptic = 0;
		}
	}

	/* Add this to the list of handles */
	GamepadHandles.push_back(handle);
}


/* 
 * Name: KeOnGamepadButtonPress
 * Desc: Called when a gamepad button has been pressed
 */
void KeOnGamepadButtonPress( int device_id, void* context )
{
	SDL_ControllerButtonEvent* event = static_cast<SDL_ControllerButtonEvent*>( context );

	/* Search for the specified device and update the button info */

	std::vector<CKeGamepadHandle>::iterator i = GamepadHandles.begin();

	while( i != GamepadHandles.end() )
	{
		if( i->id == device_id )
		{
			i->state.buttons[event->button].pressed = event->state;
			i->state.buttons[event->button].timestamp = event->timestamp;

			return;
		}

		++i;
	}
}


/*
 * Name: KeOnGamepadRemoved
 * Desc: Called when a gamepad has been removed/dettached.
 */
void KeOnGamepadRemoved( int device_id )
{
	/* Search for this ID, if we find it, go ahead and remove it after unintializing it. */

	std::vector<CKeGamepadHandle>::iterator i = GamepadHandles.begin();

	while( i != GamepadHandles.end() )
	{
		if( i->id == device_id )
		{
			GamepadHandles.erase(i);

			return;
		}

		++i;
	}

}


/*
 * Name: KeGetGamepadState
 * Desc: Returns the input state of the specified gamepad
 */
bool KeGetGamepadState( int device_id, KeGamepadState* gamepad )
{
	std::vector<CKeGamepadHandle>::iterator i = GamepadHandles.begin();

	while( i != GamepadHandles.end() )
	{
		if( i->id == device_id )
		{
			memmove( gamepad, &i->state, sizeof( KeGamepadState ) );

			return true;
		}

		++i;
	}

	return false;
}