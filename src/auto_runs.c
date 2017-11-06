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
	autoStationaryBlueLeft();
	//autoTest();
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
	gPosition.x = 33.6;
	gPosition.y = 33.6;
	gPosition.a = PI / 4;
	releaseCPU();
	grabPreload();
	moveToTargetAsync(43, 43, 33.6, 33.6, 127, 6, 1, 1.5, false, true);
	unsigned long driveTimeout = nPgmTime + 1000;
	gLiftTarget = 1800;
	tStart(LiftTaskUp);
	unsigned long coneTimeout = nPgmTime + 1000;
	while (!gLiftTargetReached && !TimedOut(coneTimeout)) sleep(10);
	setLift(-15);
	moveToTargetAwait(driveTimeout);
	setArm(-70);
	coneTimeout = nPgmTime + 500;
	while (gSensor[armPoti].value > 1750 && !TimedOut(coneTimeout)) sleep(10);
	setArm(10);
	setLift(-50);
	coneTimeout = nPgmTime + 800;
	while (gSensor[liftPoti].value > 1800 && !TimedOut(coneTimeout)) sleep(10);
	setLift(10);
	setClaw(CLAW_OPEN_POWER);
	coneTimeout = nPgmTime + 500;
	while (gSensor[clawPoti].value < CLAW_OPEN && !TimedOut(coneTimeout)) sleep(10);
	setClaw(CLAW_OPEN_HOLD_POWER);
	setArm(127);
	coneTimeout = nPgmTime + 500;
	while (gSensor[armPoti].value < ARM_TOP - 100 && !TimedOut(coneTimeout)) sleep(10);
	setArm(10);
	sleep(500);
}

void autoStationaryBlueLeft()
{
	autoStationaryCore();
	setClaw(-15);
	moveToTargetAsync(36, 38, 43, 43, -60, 6, 4, 4, false, true);
	unsigned long driveTimeout = nPgmTime + 2000;
	moveToTargetAwait(driveTimeout);
	sleep(300);
	moveToTargetAsync(70, 18, 36, 36, 127, 6, 2, 8, false, true);
	driveTimeout = nPgmTime + 3000;
	tStart(dropArm);
	setLift(-80);
	unsigned long coneTimeout = nPgmTime + 2000;
	while (!gSensor[limBottom].value && !TimedOut(coneTimeout)) sleep(10);
	setLift(-10);
	moveToTargetAwait(driveTimeout);
	moveToTargetAsync(70, 18, 36, 36, 50, 4, 1, 1, true, true);
	setClaw(CLAW_CLOSE_POWER);
	coneTimeout = nPgmTime + 500;
	while (gSensor[clawPoti].value < CLAW_CLOSE && !TimedOut(coneTimeout)) sleep(10);
	setClaw(CLAW_CLOSE_HOLD_POWER);
}

void autoTest()
{
	gPosition.x = gPosition.y = gPosition.a = 0;
	moveToTarget(15, 0, 0, 0, 127, 6, 1, 2, false, true);
	moveToTarget(25, 10, 15, 0, 127, 6, 1, 2, false, true);
	moveToTarget(60, -20, 25, 10, 127, 8, 2, 2, true, true);
}
