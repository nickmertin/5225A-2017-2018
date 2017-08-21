/* Functions */
void setupMotors()
{
	// Clear both motor arrays
	for (ubyte i = 0; i < kNumbOfTotalMotors; ++i) {
		gMotor[i].power = gMotor[i].curPower = motor[i] = 0;
		gMotor[i].powerScale = -1;
	}
}

void updateMotors()
{
	for (ubyte i = 0; i < kNumbOfTotalMotors; ++i)
		updateMotor(i);
}

void updateMotor(tMotor mot)
{
	motor[mot] = gMotor[mot].curPower = (gMotor[mot].powerScale > 0 ? round(gMotor[mot].power * gMotor[mot].powerScale) : gMotor[mot].power);
}

void stopAllMotors()
{
	for (ubyte i = 0; i < kNumbOfTotalMotors; ++i)
	{
		gMotor[i].power = 0;
		updateMotor(i);
	}
}
