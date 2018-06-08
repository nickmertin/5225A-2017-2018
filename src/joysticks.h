/* Structures */
typedef struct _sJoy
{
	short cur;
	short lst;
	short deadzone;
	bool enabled;
	TVexJoysticks partner;
	bool ignore;
} sJoy; // Represent a single item on the joystick

/* Functions */
void setupJoysticks(); // Initilize all the joysticks
void updateJoysticks(); // Update the enabled joysticks
void updateJoystick(TVexJoysticks joy); // Update a joystick
void enableJoystick(TVexJoysticks joy, TVexJoysticks partner = -1); // Set a joystick to be enabled

/* Variables */
sJoy gJoy[kNumbOfVexRFIndices]; // The global joystick array
bool gKiddieControl = false; // True if partner joystick has control

/* Defines */
#define RISING(joy) (gJoy[joy].cur && !gJoy[joy].lst)
#define FALLING(joy) (!gJoy[joy].cur && gJoy[joy].lst)
#define ENABLE(joy) enableJoystick(joy, joy##Xmtr2)
