/* Structures */
typedef struct _sMotor
{
	word power;
	word curPower;
	float powerScale;
} sMotor; // Represent a single motor

/* Variables */
sMotor gMotor[kNumbOfTotalMotors]; // The global motor array

/* Functions */
void setupMotors(); // Initilize all the motor poitners
void updateMotors(); // Update the motor outputs
void updateMotor(tMotor mot); // Update a motor output
void stopAllMotors(); // Update all motor outputs to 0
