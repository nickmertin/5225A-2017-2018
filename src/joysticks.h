/* Structures */
typedef struct _sJoy
{
	short cur;
	short lst;
	short deadzone;
	bool enabled;
	TVexJoysticks mirror;
} sJoy; // Represent a single item on the joystick

/* Functions */
void setupJoysticks(); // Initilize all the joysticks
void updateJoysticks(); // Update the enabled joysticks
void updateJoystick(TVexJoysticks joy); // Update a joystick
void enableJoystick(TVexJoysticks joy); // Set a joystick to be enabled
void mirrorJoystick(TVexJoysticks joy, TVexJoysticks mirror); // set a joystick to be mirrored

/* Variables */
sJoy gJoy[kNumbOfVexRFIndices]; // The global joystick array

/* Defines */
#define RISING(joy) (gJoy[joy].cur && !gJoy[joy].lst)
#define FALLING(joy) (!gJoy[joy].cur && gJoy[joy].lst)
#define MIRROR(joy) mirrorJoystick(joy, joy##Xmtr2)
