/* Structures */
typedef struct _sMotor
{
	word power;
	word curPower;
	float powerScale;
	bool logPower;
} sMotor; // Represent a single motor

/* Functions */
void setupMotors(); // Initilize all the motors
void updateMotors(); // Update the motor outputs
void updateMotor(tMotor mot); // Update a motor output

/* Variables */
sMotor gMotor[kNumbOfTotalMotors]; // The global motor array
bool gTimedOut = false;
