/*
 * Name: KeOnGamepadAdded
 * Desc: Called when a gamepad has been added/attached.
 */
void KeOnGamepadAdded( int device_id );


/*
 * Name: KeOnGamepadRemoved
 * Desc: Called when a gamepad has been removed/dettached.
 */
void KeOnGamepadRemoved( int device_id );


/* 
 * Name: KeOnGamepadButtonPress
 * Desc: Called when a gamepad button has been pressed
 */
void KeOnGamepadButtonPress( int device_id, void* context );


/*
* Name: KeOnJoystickAdded
* Desc: Called when a Joystick has been added/attached.
*/
void KeOnJoystickAdded( int device_id );


/*
* Name: KeOnJoystickButtonPress
* Desc: Called when a Joystick button has been pressed
*/
void KeOnJoystickButtonPress( int device_id, void* context );


/*
 * Name: KeOnJoystickButtonPress
 * Desc: Called when a Joystick POV hat has been pressed
 */
void KeOnJoystickPOVPress( int device_id, void* context );


/*
 * Name: KeOnJoystickRemoved
 * Desc: Called when a Joystick has been removed/dettached.
 */
void KeOnJoystickRemoved( int device_id );