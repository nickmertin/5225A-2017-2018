/* Functions */
void setupMotors()
{
	// Clear both motor arrays
	for (ubyte i = 0; i < kNumbOfTotalMotors; ++i) {
		gMotor[i].power = gMotor[i].curPower = motor[i] = 0;
		gMotor[i].powerScale = -1;
#ifdef MOTOR_SENSOR_LOGS
		gMotor[i].datalog = -1;
#endif
	}

#ifdef MOTOR_SENSOR_LOGS
	_motorDoDatalog = false;
#endif
}

void updateMotors()
{
#ifdef MOTOR_SENSOR_LOGS
	tHog();
	datalogDataGroupStart();
	_motorDoDatalog = true;
#endif
	for (ubyte i = 0; i < kNumbOfTotalMotors; ++i)
		updateMotor(i);
#ifdef MOTOR_SENSOR_LOGS
	_motorDoDatalog = false;
	datalogDataGroupEnd();
	tRelease();
#endif
}

void updateMotor(tMotor mot)
{
	motor[mot] = gMotor[mot].curPower = gMotor[mot].powerScale >= 0 ? round(gMotor[mot].power * gMotor[mot].powerScale) : gMotor[mot].power;
#ifdef MOTOR_SENSOR_LOGS
	if (_motorDoDatalog && gMotor[mot].datalog != -1)
		datalogAddValue(gMotor[mot].datalog, gMotor[mot].curPower);
#endif
}
