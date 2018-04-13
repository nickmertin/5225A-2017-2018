/* Functions */
void setupMotors()
{
	// Clear both motor arrays
	for (ubyte i = 0; i < kNumbOfTotalMotors; ++i) {
		gMotor[i].power = gMotor[i].curPower = motor[i] = 0;
		gMotor[i].powerScale = -1;
		gMotor[i].datalog = -1;
	}
	_motorDoDatalog = false;
}

void updateMotors()
{
	tHog();
	datalogDataGroupStart();
	_motorDoDatalog = true;
	for (ubyte i = 0; i < kNumbOfTotalMotors; ++i)
		updateMotor(i);
	_motorDoDatalog = false;
	datalogDataGroupEnd();
	tRelease();
}

void updateMotor(tMotor mot)
{
	word oldPower = gMotor[mot].curPower;
	motor[mot] = gMotor[mot].curPower = gMotor[mot].powerScale >= 0 ? round(gMotor[mot].power * gMotor[mot].powerScale) : gMotor[mot].power;
	if (_motorDoDatalog && gMotor[mot].datalog != -1)
		datalogAddValue(gMotor[mot].datalog, gMotor[mot].curPower);
}
