void selectAuto()
{
	int autoVal = gSensor[autoPoti].value - 2048;
	if (autoVal < 0) gAlliance = allianceBlue;
	else gAlliance = allianceRed;

	autoVal = abs(autoVal);
	if (autoVal < 300) gCurAuto = 0;
	else if (autoVal < 600) gCurAuto = 1;
	else if (autoVal < 900) gCurAuto = 2;
	else if (autoVal < 1200) gCurAuto = 3;
	else if (autoVal < 1500) gCurAuto = 4;
	else if (autoVal < 1800) gCurAuto = 5;
	else gCurAuto = 6;
}

void runAuto()
{
	//autoStationaryBlueLeft();
	autoTest();
	return;
	if (gAlliance == allianceBlue)
	{
		switch (gCurAuto)
		{
			case 0: autoSkills(); break;
			case 1: autoStationaryBlueLeft(); break;
		}
	}
	else
	{
		switch (gCurAuto)
		{
			case 0: break;
			default: break;
		}
	}
}

void autoSkills()
{
}

void autoStationaryCore()
{
	hogCPU();
	gPosition.x = 23;
	gPosition.y = 23;
	gPosition.a = PI / 4;
	releaseCPU();
	grabPreload();
	moveToTargetAsync(32.5, 32.5, 23, 23, 127, 6, 1, false, true);
	unsigned long driveTimeout = nPgmTime + 1000;
	gLiftTarget = 1500;
	tStart(LiftTaskUp);
	unsigned long coneTimeout = nPgmTime + 800;
	while (!gLiftTargetReached && !TimedOut(coneTimeout)) sleep(10);
	setLift(-15);
	moveToTargetAwait(driveTimeout);
	setArm(-70);
	coneTimeout = nPgmTime + 500;
	while (gSensor[armPoti].value > 1750 && !TimedOut(coneTimeout)) sleep(10);
	setArm(20);
	sleep(300);
	setArm(10);
	setLift(-50);
	coneTimeout = nPgmTime + 800;
	while (gSensor[liftPoti].value > 950 && !TimedOut(coneTimeout)) sleep(10);
	setLift(10);
	setClaw(CLAW_OPEN_POWER);
	coneTimeout = nPgmTime + 500;
	while (gSensor[clawPoti].value > CLAW_OPEN && !TimedOut(coneTimeout)) sleep(10);
	setClaw(CLAW_OPEN_HOLD_POWER);
	sleep(200);
}

void autoStationaryBlueLeft()
{
	autoStationaryCore();
	moveToTargetAsync(24, 30, 32.5, 32.5, -127, 6, 3, true, true);
	unsigned long driveTimeout = nPgmTime + 1500;
	sleep(300);
	setLift(-80);
	unsigned long coneTimeout = nPgmTime + 800;
	while (!gSensor[limBottom].value && !TimedOut(coneTimeout)) sleep(10);
	setLift(-10);
	moveToTargetAwait(driveTimeout);
	//turnToTargetAsync(72, 24, 24, 30, ccw, 80, 80, true, true, 0);
	driveTimeout = nPgmTime + 2000;
	tStart(dropArm);
	//turnToTargetAwait(driveTimeout);
	moveToTarget(72, 24, 24, 30, 127, 6, 1, false, true);
	setClaw(CLAW_CLOSE_POWER);
	coneTimeout = nPgmTime + 500;
	while (gSensor[clawPoti].value < CLAW_CLOSE && !TimedOut(coneTimeout)) sleep(10);
	setClaw(CLAW_CLOSE_HOLD_POWER);
}

void autoTest()
{
	gPosition.x = gPosition.y = gPosition.a = 0;
	moveToTarget(15, 0, 0, 0, 127, 6, 2, false, true);
	moveToTarget(25, 10, 15, 0, 127, 6, 2, false, true);
	moveToTarget(60, -20, 25, 5, 127, 6, 1, true, true);
}
