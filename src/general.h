/* Defines */
// Deadzones
#define DZ_CLAW 20
#define DZ_LIFT 20

#define EXPANDER_THRESHOLD 500

/* Variables */
short gJoyThrottle = 0; // The value of the joystick that controls the linear movement of our robot
short gJoyTurn = 0; // The value of the joystick that controls the angular movement of our robot
string gError = "  "; // The current error
string gErrorLst = "  "; // The last error

/* Functions */
task statusMonitor(); // A function to flash an LED if the second battery is not plugged in
