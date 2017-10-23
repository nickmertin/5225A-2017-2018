/* Structures */
typedef struct _sJoy
{
	short cur;
	short lst;
	short deadzone;
} sJoy; // Represent a single item on the joystick

/* Functions */
void setupJoysticks(); // Initilize all the joysticks
void updateJoysticks(); // Update the joystick outputs
void updateJoystick(TVexJoysticks joy); // Update a joystick output

/* Variables */
sJoy gJoy[kNumbOfVexRFIndices]; // The global joystick array

/* Defines */
#define RISING(joy) (gJoy[joy].cur && !gJoy[joy].lst)
#define FALLING(joy) (!gJoy[joy].cur && gJoy[joy].lst)
