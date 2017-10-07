/* Enumerations */
typedef enum _tSensorClass
{
	snclsNone,
	snclsInput,
	snclsOutput,
	snclsOther
} tSensorClass; // Represents a sensor base type

typedef enum _tSensorMode
{
	snmdNormal,
	snmdDgtIn
} tSensorMode;

/* Structures */
typedef struct _sSensor
{
	int value;
	int lstValue;
	tSensorClass cls;
	tSensors port;
	int safetyVal;
	unsigned long failedStartTime;
	unsigned long failedCheckTime;
	unsigned long safetyStartTime;
	bool failed;
	float velocity;
	float lstVelocity;
	int valueVelLst;
	unsigned long timeVelCheck;
	tSensorMode mode;
	int dgtMin;
	int dgtMax;
	bool velGood;
} sSensor; // Represent a single sensor

/* Functions */
void setupSensors(); // Initilize all the sensors
void updateSensorOutput(tSensors sen); // Update the output for a single sensor
void updateSensorOutputs(); // Update all the output sensors
void updateSensorInput(tSensors sen); // Update the input for a signle sensor
void updateSensorInputs(); // Update all the input sensors
tSensorClass checkSenClass(tSensors sen); // Check the sensor class of a motor
bool correctBtnIn(tSensors sen); // Call this function to interpret a sensor set up using setupDgtIn
void setupDgtIn(tSensors sen, int min, int max); // Setup a sensor to be interpreted as a boolean using a custom range
void resetQuadratureEncoder(tSensors sen); // Reset a quadrature encoder
bool safetyCheck(tSensors sen, unsigned long failedTime, float failedVal, unsigned long safetyMovingTime); // Check if a rotary sensor has failed
void safetyClear(tSensors sen); // Clear the error flag on a rotary sensor
void safetySet(tSensors sen); // Set the error flag on a rotary sensor
void velocityCheck(tSensors sen); // Check the velocity of a sensor
void velocityClear(tSensors sen); // Clear the velocity of a sensor
void startSensor(tSensors sen); // Set a sensors starting value
void startSensors(); // Set all the sensors starting value

/* Variables */
sSensor gSensor[kNumbOfTotalSensors]; // The global sensor array
