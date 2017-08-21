/* Functions */
void createGyro(sGyro& gyro, sSensor& sen)
{
	static sGyro* last = NULL; // Remember the last gyro so we can add new ones
	gyro.sensor = &sen; // Setup the gyro sensor
	sen.cls = snclsOther; // Set it to a manual input/output sensor

	calabrateGyro(gyro); // Calabrate the gyro

	if (last == NULL) // If this is the first gyro
		startGyroMonitor(gyro); // Start the gyro monitor
	else
		last->next = &gyro; // Add this gyro after the last one
	last = &gyro; // Update the last gyro
}

void calabrateGyro(sGyro& gyro)
{
	float stdDev = 0.0; // The standard deviation of the gyro (average distance points are from the average of the points)
	float avg = 0.0; // The average of the points

	for (unsigned long i = 0; i < GYRO_CALIBRATION_POINTS; ++i)
	{
		float rate = SensorValue[gyro.sensor->port]; // Get the raw rate of the gyro
		avg += rate; // Add to the average (sum of the points right now)
		_gyroPoints[i] = rate; // Save it to an array to use to calculate he standard deviation
		sleep(1); // Wait for 1ms
	}
	avg /= GYRO_CALIBRATION_POINTS; // Devide the sum by the total amount of data points to get the average
	gyro.avg = avg; // Save the average to the gyro structure

	for (unsigned long i = 0; i < GYRO_CALIBRATION_POINTS; ++i)
		stdDev += fabs(avg - _gyroPoints[i]); // Add the distance the points is away from the average to a variable
	stdDev /= GYRO_CALIBRATION_POINTS; // Devide the variable by the total amount of points to make it the standard deviation
	gyro.stdDev = stdDev; // Save it to the gyro structure

	/*
	 * Datasheet from VEX indicates that the sensitivity of the gyro is 1.1mV/dps
	 * and the cortex ADC for raw analog reads ranges from 0-4095 for 0v-5v
	 * readings. The gyro is scaled from the nominal 2.7v-3.6v operating range
	 * that the actual chip has to work on the cortex's 5v scale. The scale multiplier
	 * value is in the ballpark of 1.72, plus or minus a few hundredths.
	 */
	gyro.voltsPerDPS = (0.0011 / 1.71625741) * (avg * 5 / 4095);
}

float gyroGetRate(sGyro& gyro)
{
	float gyroRead = SensorValue[gyro.sensor->port];

	// Difference from zero-rate value or the average calibration read
	float diff = gyroRead - gyro.avg;

	// Difference fro zero-rate value, in volts
	float voltage = diff * 5 / 4095;

	if (fabs(diff) > GYRO_STD_DEVS * gyro.stdDev)
		return voltage / gyro.voltsPerDPS;

	return 0;
}

void startGyroMonitor(sGyro& gyro)
{
	// Set the first gyro and start the task
	_firstGyro = &gyro;
	startTask(monitorGyro);
}

/* Tasks */
task monitorGyro()
{
	sGyro* gyro;
	unsigned long lstTime = nPgmTime;
	while (true)
	{
		float dt = (float)(nPgmTime - lstTime) / 1000.0;
		lstTime = nPgmTime;
		gyro = _firstGyro; // Set the gyro to the first value
		while (gyro != NULL) // While I still have gyro's to update
		{
			float rate = gyroGetRate(*gyro) * dt; // Get the filtered rate
			gyro->value += rate; // Sum the rate to get teh value
			gyro = gyro->next; // Go the the next gyro
		}
		EndTimeSlice();
	}
}
