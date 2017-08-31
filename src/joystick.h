/* Defines */
// Utilities for new joystick system
#define _VALUE(channel) \
int value; \
if (gJoystickFlags & (1 << channel)) \
	value = gJoystick[channel]; \
else \
{ \
	value = vexRT[channel]; \
	if (abs(value) < gDeadzone[channel]) \
		value = 0; \
	if (value) \
		gLastBtnPressedTime = nPgmTime; \
	gJoystickLast[channel] = gJoystick[channel]; \
	gJoystick[channel] = value; \
	gJoystickFlags |= 1 << channel; \
}

// Defines for new joystick system
#define DRIVER(num) (1 << num)
#define CONFIGS void handleJoysticks()
#define CONFIG_INIT { gJoystickFlags = 0; for (TVexJoysticks i = 0; i < kNumbOfVexRFIndices; ++i) gDeadzone[i] = 0; HANDLE_STATE_REQUEST(joystick, , ) }
#define CONFIG(driver) if (DRIVER(joystickData.state) & (driver))
#define DEADZONE(channel, dz) gDeadzone[channel] = dz;
#define ANY(channel, code) { _VALUE(channel) code }
#define RISING(channel, code) { _VALUE(channel) if (value && !gJoystickLast[channel]) { code } }
#define FALLING(channel, code) { _VALUE(channel) if (!value && gJoystickLast[channel]) { code } }
#define HIGH(channel, code) { _VALUE(channel) if (value) { code } }
#define LOW(channel, code) { _VALUE(channel) if (!value) { code } }
#define TWO(channel1, channel2, code) { int value1, value2; { _VALUE(channel1) value1 = value; } { _VALUE(channel2) value2 = value; } code }

/* Types */
typedef enum _tDriver
{
	SINGLE_STICK,
	DUAL_STICK,
	DRIVER_END
} tDriver;

/* Variables */
unsigned long gLastBtnPressedTime = 0;

// Globals for new joystick system
int gDeadzone[kNumbOfVexRFIndices];
int gJoystick[kNumbOfVexRFIndices];
int gJoystickLast[kNumbOfVexRFIndices];
unsigned long gJoystickFlags;
const string gDriverNames[2] = { "Single Stick", "Dual Stick" };

bool gThrowLast = false;
bool gLiftOnPreset = false;

/* Functions */
// New joystick system
CONFIGS;

/* State Machine */
MAKE_MACHINE(joystick, tDriver, DRIVER_END,
{
	case SINGLE_STICK: break;
	case DUAL_STICK: break;
})
