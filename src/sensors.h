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
	snmdDgtIn,
	snmdFiltered,
} tSensorMode;

/* Structures */
typedef struct _sSensor
{
	int* value;
	int* lstValue;
	tSensorClass cls;
	tSensors port;
	int safetyVal;
	unsigned long failedStartTime;
	unsigned long failedCheckTime;
	unsigned long safetyStartTime;
	bool failed;
	float velocity;
	float velocityLst;
	int valueVelLst;
	unsigned long timeVelCheck;
	float filterLeniency;
	int rawData[10];
	tSensorMode mode;
} sSensor; // Represent a single sensor

/* Variables */
sSensor gSensor[kNumbOfTotalSensors]; // The global sensor array

// Internal variables
int _sensorValue[kNumbOfTotalSensors]; // Internally store the value of all the sensors
int _sensorLstValue[kNumbOfTotalSensors]; // Internally store the last values of all the sensors

/* Functions */
void setupSensors(); // Initilize all the sensor poitners
void updateSensorOutput(tSensors sen); // Update the output for a single sensor
void updateSensorOutputs(); // Update all the output sensors
void updateSensorInput(tSensors sen); // Update the input for a signle sensor
void updateSensorInputs(); // Update all the input sensors
void updateSensorLst(tSensors sen); // Update the last value of a sensor
void updateSensorsLst(); // Update the last value of all the sensors
tSensorClass checkSenClass(tSensors sen); // Check the sensor class
bool correctBtnIn(tSensors sen); // Call this function to fix the value of a button in an analog port
float filterSensor(tSensors sen); // Filter the data from this sensor
void setupDgtIn(tSensors sen); // Setup a sensor to be a digital sensor in an analog port
void setupFilteredSensor(tSensors sen, float leniency); // Setup standard deviation filtering on a sensor
void resetQuadratureEncoder(tSensors sen); // Reset a quadrature encoder
bool safetyCheck(tSensors sen, unsigned long failedTime, float failedVal, unsigned long safetyMovingTime); // Check if a rotary sensor has failed
void safetyClear(tSensors sen); // Clear the error flag on a rotary sensor
void safetySet(tSensors sen); // Set the error flag on a rotary sensor
void velocityCheck(tSensors sen); // Check the velocity of a sensor
void velocityClear(tSensors sen); // Clear the velocity of a sensor
