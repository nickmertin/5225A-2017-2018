/* Functions */
void setupMotors()
{
	// Clear both motor arrays
	for (ubyte i = 0; i < kNumbOfTotalMotors; ++i) {
		gMotor[i].power = gMotor[i].curPower = motor[i] = 0;
		gMotor[i].powerScale = -1;
		gMotor[i].logPower = false;
	}
}

void updateMotors()
{
	for (ubyte i = 0; i < kNumbOfTotalMotors; ++i)
		updateMotor(i);
}

void updateMotor(tMotor mot)
{
#ifdef LOG_MOTOR_POWER
	word oldPower = gMotor[mot].curPower;
#endif
	motor[mot] = gMotor[mot].curPower = gTimedOut ? 0 : (gMotor[mot].powerScale >= 0 ? round(gMotor[mot].power * gMotor[mot].powerScale) : gMotor[mot].power);
#ifdef LOG_MOTOR_POWER
	if (gMotor[mot].logPower && gMotor[mot].curPower != oldPower)
		writeDebugStreamLine("%06d MP %2d %4d", nPgmTime, mot + 1, gMotor[mot].curPower);
#endif
}
