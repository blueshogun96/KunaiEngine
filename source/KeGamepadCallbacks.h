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