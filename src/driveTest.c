void driveFuncTest(int targ, unsigned long time, int a, int b)
{
	LOG(drive)("driveFuncTest %d, %d, %d, %d TO: %d", targ, time, a, b, driveTimeout);
	targ += gSensor[trackR].value;
	setDrive(67, 67);
	WHILE(drive, gSensor[trackR].value < targ)
	{
		VEL_CHECK_INC(drive, velSensor);
		sleep(10);
	}
	setDrive(20, 20);
	unsigned long timeout = -1;
	if (timeout == -1)
		writeDebugStreamLine ("test worked");

}

ADD_FUNCS_TO_MACHINE_1(drive, driveFuncTest);
PREP_FUNC_STATE_4(driveFuncTest, int, unsigned long, int, int);
