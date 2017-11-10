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

void autoStationaryCore(bool setup, int liftUp, int liftDown)
{
	unsigned long driveTimeout, coneTimeout;
	if (setup)
	{
		hogCPU();
		gPosition.x = 30;
		gPosition.y = 30;
		gPosition.a = PI / 4;
		releaseCPU();
		grabPreload();
	}
	else
	{
		turnToTargetAsync(48, 48, gPosition.y, gPosition.x, cw, 127, 127, true, true, 0);
		driveTimeout = nPgmTime + 1500;
		coneTimeout = nPgmTime + 1000;
		setArm(127);
		while (gSensor[armPoti].value < gArmPositions[2] && !TimedOut(coneTimeout, "sg core 1")) sleep(10);
		setArm(10);
		turnToTargetAwait(driveTimeout);
	}
	moveToTargetAsync(48, 48, gPosition.y, gPosition.x, 60, 4, 1, 12, false, true);
	driveTimeout = nPgmTime + 3000;
	gLiftTarget = liftUp;
	tStart(LiftTaskUp);
	coneTimeout = nPgmTime + 1000;
	while (!gLiftTargetReached && !TimedOut(coneTimeout, "sg core 2")) sleep(10);
	setLift(-15);
	moveToTargetAwait(driveTimeout);
	setArm(-90);
	coneTimeout = nPgmTime + 500;
	while (gSensor[armPoti].value > 1650 && !TimedOut(coneTimeout, "sg core 3")) sleep(10);
	setArm(10);
	setLift(-50);
	coneTimeout = nPgmTime + 800;
	while (gSensor[liftPoti].value > liftDown && !TimedOut(coneTimeout, "sg core 4")) sleep(10);
	setLift(0);
	setClaw(CLAW_OPEN_POWER);
	coneTimeout = nPgmTime + 500;
	while (gSensor[clawPoti].value < CLAW_OPEN && !TimedOut(coneTimeout, "sg core 5")) sleep(10);
	setClaw(-15);
	setArm(127);
	coneTimeout = nPgmTime + 500;
	while (gSensor[armPoti].value < ARM_TOP - 100 && !TimedOut(coneTimeout, "sg core 6")) sleep(10);
	setArm(10);
	sleep(200);
}

void autoStationaryBlueLeft()
{
	autoStationaryCore(true, 1800, 1700);
	setClaw(-15);
	moveToTargetAsync(33, 35, 38.5, 38.5, -80, 6, 4, 4, true, true);
	unsigned long driveTimeout = nPgmTime + 2000;
	moveToTargetAwait(driveTimeout);
	sleep(100);
	moveToTargetAsync(63, 23, 33, 35, 127, 4, 1, 6, false, true);
	driveTimeout = nPgmTime + 3000;
	tStart(dropArm);
	setLift(-80);
	unsigned long coneTimeout = nPgmTime + 1000;
	while (!gSensor[limBottom].value && !TimedOut(coneTimeout, "sg lb 1")) sleep(10);
	setLift(-10);
	moveToTargetAwait(driveTimeout);
	moveToTargetAsync(63, 23, 33, 35, 30, 4, 1, 2, false, true);
	driveTimeout = nPgmTime + 800;
	setClaw(CLAW_CLOSE_POWER);
	coneTimeout = nPgmTime + 1000;
	while (gSensor[clawPoti].value > CLAW_CLOSE && !TimedOut(coneTimeout, "sg lb 2")) sleep(10);
	setClaw(CLAW_CLOSE_HOLD_POWER);
	sleep(100);
	moveToTargetAwait(driveTimeout);
	autoStationaryCore(false, 2250, 2100);
	moveToTargetAsync(70, 30, 48, 48, -127, 12, 3, 3, true, true);
	driveTimeout = nPgmTime + 2000;
	sleep(100);
	tStart(dropArm);
	moveToTargetAwait(driveTimeout);
	turnToTargetAsync(72, 9, 61, 35, cw, 127, 127, true, true, 0);
	driveTimeout = nPgmTime + 1200;
	setLift(-80);
	coneTimeout = nPgmTime + 1000;
	while (!gSensor[limBottom].value && !TimedOut(coneTimeout, "sg lb 3")) sleep(10);
	setLift(-10);
	turnToTargetAwait(driveTimeout);
	moveToTargetAsync(72, 9, gPosition.y, gPosition.x, 60, 4, 1, 2, true, true);
	driveTimeout = nPgmTime + 2000;
	moveToTargetAwait(driveTimeout);
	setClaw(CLAW_CLOSE_POWER);
	coneTimeout = nPgmTime + 1000;
	while (gSensor[clawPoti].value > CLAW_CLOSE && !TimedOut(coneTimeout, "sg lb 4")) sleep(10);
	setClaw(CLAW_CLOSE_HOLD_POWER);
	sleep(100);
	moveToTargetAsync(72, 20, 72, 9, -80, 6, 2, 2, false, true);
	driveTimeout = nPgmTime + 2000;
	moveToTargetAwait(driveTimeout);
	autoStationaryCore(false, 2800, 2500);
	moveToTarget(61, 35, 48, 48, -50, 6, 3, 3, true, true);
}

void autoTest()
{
	gPosition.x = gPosition.y = gPosition.a = 0;
	moveToTarget(15, 0, 0, 0, 127, 6, 1, 2, false, true);
	moveToTarget(25, 10, 15, 0, 127, 6, 1, 2, false, true);
	moveToTarget(60, -20, 25, 10, 127, 8, 2, 2, true, true);
}
