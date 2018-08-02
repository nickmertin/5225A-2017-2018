void driveFuncTest(int targ, unsigned long time, int a, int b)
{
	writeDebugStreamLine("driveFuncTest %d, %d, %d, %d TO: %d", targ, time, a, b, driveTimeout);
	targ += gSensor[trackR].value;
	setDriveLeft(67);
	setDriveRight(67);
	while ( gSensor[trackR].value < targ && SAFETY_CHECK(drive)
		sleep(10);
	}
	setDriveLeft(20);
	setDriveRight(20);
	unsigned long timeout = -1;
	if (timeout == -1)
		writeDebugStreamLine ("test worked");

}

ADD_FUNCS_TO_MACHINE_1(drive, driveFuncTest);
PREP_FUNC_STATE_4(driveFuncTest, int, unsigned long, int, int);
