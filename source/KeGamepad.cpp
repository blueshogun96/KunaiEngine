#include "KeGamepad.h"
#include <vector>


/* 
 * Gamepad handle 
 */
class CKeGamepadHandle
{
public:
	CKeGamepadHandle() : game_controller(NULL), haptic(NULL), joystick_id(-1), id(0), connected(No) {}
	virtual ~CKeGamepadHandle()
	{
        if( haptic )
        {
            SDL_HapticClose( haptic );
            haptic = NULL;
        }

        if( game_controller )
        {
            SDL_GameControllerClose( game_controller );
            game_controller = NULL;
        }
	}

	SDL_GameController* game_controller;
	SDL_Haptic*			haptic;
	SDL_JoystickID		joystick_id;
	KeGamepadState		state;
	int					id;
	bool				connected;
};


/* 
 * Joystick handle 
 */
class CKeJoystickHandle
{
public:
	CKeJoystickHandle() : joystick( NULL ), joystick_id(-1), id(0), haptic(NULL), connected(No) {}
	virtual ~CKeJoystickHandle()
	{
		if( haptic )
        {
			SDL_HapticClose( haptic );
            haptic = NULL;
        }

		if( joystick )
        {
			SDL_JoystickClose( joystick );
            joystick = NULL;
        }
	}

	SDL_Joystick*	joystick;
	SDL_JoystickID	joystick_id;
	SDL_Haptic*		haptic;
	KeJoystickState state;
	int				id;
	bool			connected;
};

/* Gamepad handles */
std::vector<std::shared_ptr<CKeGamepadHandle>> GamepadHandles;
std::vector<std::shared_ptr<CKeJoystickHandle>> JoystickHandles;


/*
 * Name: KeInitializeGamepads
 * Desc: Initializes the gamepad API for the target platform.
 */
bool KeInitializeGamepads()
{
	int ret = 0;

	/* Initialize SDL2 gamepad API */
	ret = SDL_InitSubSystem( SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC );
	if( ret != 0 )
		DISPDBG( KE_ERROR, "An error has occured initializing SDL controller API!" );
	
	/* Enable joystick events */
	SDL_JoystickEventState( SDL_ENABLE );
    
    //test_controller = SDL_GameControllerOpen(0);
    
	return ret;
}


/*
 * Name: KeUnintializeGamepads
 * Desc: Uninitializes the above.
 */
void KeUninitializeGamepads()
{
    //SDL_GameControllerClose( test_controller );
    
	/* Kill all open gamepad devices */
	GamepadHandles.clear();
	JoystickHandles.clear();

	/* Uninitialize the gamepad API */
	SDL_QuitSubSystem( SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC );
}


/*
 * Name: KeOnGamepadAdded
 * Desc: Called when a gamepad has been added/attached.
 */
void KeOnGamepadAdded( int device_id )
{
    std::shared_ptr<CKeGamepadHandle> handle( new CKeGamepadHandle );

	/* Attempt to open this device */
	handle->game_controller = SDL_GameControllerOpen( device_id );

	/* Get the joystick instance ID */
	SDL_Joystick* j = SDL_GameControllerGetJoystick( handle->game_controller );
	handle->joystick_id = SDL_JoystickInstanceID(j);
	handle->connected = Yes;
	handle->id = device_id; /* Game controller ID */

	ZeroMemory( &handle->state, sizeof( KeGamepadState ) );

	/* Is this a haptic device? */
	if( SDL_JoystickIsHaptic(j) )
	{
		handle->haptic = SDL_HapticOpenFromJoystick(j);

		/* Is this a rumble supported haptic feature? */
		if( SDL_HapticRumbleSupported( handle->haptic ) )
		{
			/* Can we initialize the rumble feature? */
			if( SDL_HapticRumbleInit( handle->haptic ) )
			{
				/* An error occurred */
				SDL_HapticClose( handle->haptic );
				handle->haptic = 0;

				DISPDBG( KE_WARNING, "Error initializing rumble feature for gamepad!\n"
                    "\tDevice ID: " << device_id << std::endl <<
					"\tJoystick ID: " << handle->joystick_id <<  std::endl <<
                    "\tError: " << SDL_GetError() << std::endl );
			}
		}
		else
		{
			/* This is not a rumble haptic */
			SDL_HapticClose( handle->haptic );
			handle->haptic = 0;
		}
	}

	/* Add this to the list of handles */
	GamepadHandles.push_back(handle);
    
    DISPDBG( KE_DBGLVL(0), "Gamepad #" << handle->id << " (" << handle->joystick_id << ") added\n"
            "\tDevice name: " << SDL_GameControllerName( handle->game_controller ) << "\n" );
}


/* 
 * Name: KeOnGamepadButtonPress
 * Desc: Called when a gamepad button has been pressed
 */
void KeOnGamepadButtonPress( int device_id, void* context )
{
	SDL_ControllerButtonEvent* event = static_cast<SDL_ControllerButtonEvent*>( context );

	/* Search for the specified device and update the button info */

    std::vector<std::shared_ptr<CKeGamepadHandle>>::iterator i = GamepadHandles.begin();

	while( i != GamepadHandles.end() )
	{
		if( (*i)->joystick_id == device_id )
		{
			(*i)->state.buttons[event->button].pressed = event->state;
			(*i)->state.buttons[event->button].timestamp = event->timestamp;

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

    std::vector<std::shared_ptr<CKeGamepadHandle>>::iterator i = GamepadHandles.begin();

	while( i != GamepadHandles.end() )
	{
		if( (*i)->joystick_id == device_id )
		{
            DISPDBG( KE_DBGLVL(0), "Gamepad #" << (*i)->id << " (" << (*i)->joystick_id << ") removed\n" );
            
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
    std::vector<std::shared_ptr<CKeGamepadHandle>>::iterator i = GamepadHandles.begin();

	while( i != GamepadHandles.end() )
	{
		if( (*i)->id == device_id )
		{
			memmove( gamepad, &(*i)->state, sizeof( KeGamepadState ) );

			return true;
		}

		++i;
	}

	return false;
}


/*
 * Name: KeGetGamepadCount
 * Desc: Returns the number of gamepads connected
 */
int KeGetGamepadCount()
{
    return int(GamepadHandles.size());
}


/*
 * Joystick funcitons
 */

/*
 * Name: KeOnJoystickAdded
 * Desc: Called when a Joystick has been added/attached.
 */
void KeOnJoystickAdded( int device_id )
{
    std::shared_ptr<CKeJoystickHandle> handle( new CKeJoystickHandle );

	/* Attempt to open this device */
	handle->joystick = SDL_JoystickOpen( device_id );
	handle->joystick_id = SDL_JoystickInstanceID( handle->joystick );
	handle->id = device_id;
	handle->connected = Yes;

	ZeroMemory( &handle->state, sizeof( KeJoystickState ) );

	/* Is this a haptic device? */
	if (SDL_JoystickIsHaptic( handle->joystick ))
	{
		handle->haptic = SDL_HapticOpenFromJoystick( handle->joystick );

		/* Is this a rumble supported haptic feature? */
		if (SDL_HapticRumbleSupported( handle->haptic ))
		{
			/* Can we initialize the rumble feature? */
			if (SDL_HapticRumbleInit( handle->haptic ))
			{
				/* An error occurred */
				SDL_HapticClose( handle->haptic );
				handle->haptic = 0;

				DISPDBG( KE_WARNING, "Error initializing rumble feature for Joystick!\n"
					"Device ID: " << device_id << std::endl <<
					"Joystick ID: " << handle->joystick_id << std::endl <<
                    "Error: " << SDL_GetError() << std::endl );
			}
		}
		else
		{
			/* This is not a rumble haptic */
			SDL_HapticClose( handle->haptic );
			handle->haptic = 0;
		}
	}

	/* Add this to the list of handles */
	JoystickHandles.push_back( handle );
    
    DISPDBG( KE_DBGLVL(0), "Joystick #" << handle->id << " (" << handle->joystick_id << ") added\n"
            "\tDevice name: " << SDL_JoystickName( handle->joystick ) << "\n" );
}


/*
* Name: KeOnJoystickButtonPress
* Desc: Called when a Joystick button has been pressed
*/
void KeOnJoystickButtonPress( int device_id, void* context )
{
	SDL_JoyButtonEvent* event = static_cast<SDL_JoyButtonEvent*>(context);

	/* Search for the specified device and update the button info */

    std::vector<std::shared_ptr<CKeJoystickHandle>>::iterator i = JoystickHandles.begin();

	while( i != JoystickHandles.end())
	{
		if( (*i)->joystick_id == device_id )
		{
			(*i)->state.buttons[event->button].pressed = event->state;
			(*i)->state.buttons[event->button].timestamp = event->timestamp;

			return;
		}

		++i;
	}
}

/*
 * Name: KeOnJoystickButtonPress
 * Desc: Called when a Joystick POV hat has been pressed
 */
void KeOnJoystickPOVPress( int device_id, void* context )
{
	SDL_JoyHatEvent* event = static_cast<SDL_JoyHatEvent*>(context);

	/* Search for the specified device and update the button info */

    std::vector<std::shared_ptr<CKeJoystickHandle>>::iterator i = JoystickHandles.begin();

	while( i != JoystickHandles.end() )
	{
		if( (*i)->joystick_id == device_id )
		{
			(*i)->state.dpad = event->value;

			return;
		}

		++i;
	}
}

/*
 * Name: KeOnJoystickRemoved
 * Desc: Called when a Joystick has been removed/dettached.
 */
void KeOnJoystickRemoved( int device_id )
{
	/* Search for this ID, if we find it, go ahead and remove it after unintializing it. */

    std::vector<std::shared_ptr<CKeJoystickHandle>>::iterator i = JoystickHandles.begin();

	while( i != JoystickHandles.end() )
	{
		if( (*i)->joystick_id == device_id )
		{
            DISPDBG( KE_DBGLVL(0), "Joystick #" << (*i)->id << " (" << (*i)->joystick_id << ") removed\n" );
            
			JoystickHandles.erase( i );

			return;
		}

		++i;
	}

}


/*
 * Name: KeGetJoystickState
 * Desc: Returns the input state of the specified Joystick
 */
bool KeGetJoystickState( int device_id, KeJoystickState* Joystick )
{
    std::vector<std::shared_ptr<CKeJoystickHandle>>::iterator i = JoystickHandles.begin();

	while( i != JoystickHandles.end() )
	{
		if( (*i)->id == device_id )
		{
			memmove( Joystick, &(*i)->state, sizeof( KeJoystickState ) );

			return true;
		}

		++i;
	}

	return false;
}


/*
 * Name: KeGetJoystickCount
 * Desc: Returns the number of joysticks connected
 */
int KeGetJoystickCount()
{
    return int(JoystickHandles.size());
}
