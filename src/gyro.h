/* Defines */
#define GYRO_STD_DEVS 5 // Ignore data within n standard deviations of no motion average
#define GYRO_CALIBRATION_POINTS 2000 // Time in ms that the gyro calibrates for (also the number of data points)

/* Structures */
typedef struct _sGyro
{
	sSensor* sensor;
	float stdDev;
	float avg;
	float value;
	float voltsPerDPS;
	_sGyro* next;
} sGyro; // Represent a single gyro

/* Internal Variables */
sGyro* _firstGyro;

/* Internal Variables */
float _gyroPoints[GYRO_CALIBRATION_POINTS]; // Points used for gyro filter calculation (must be global because size)

/* Functions */
void createGyro(sGyro& gyro, sSensor& sen); // Setup a gyro
void calabrateGyro(sGyro& gyro); // Calabrate a gyro
float gyroGetRate(sGyro& gyro); // Get the filtered rate of a gyro
void startGyroMonitor(sGyro& gyro); // Start the gyro monitor

/* Tasks */
task monitorGyro(); // Monitor the gyro
