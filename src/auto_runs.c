void selectAuto()
{
	updateSensorInput(autoPoti);
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
	selectAuto();
	writeDebugStreamLine("Selected auto: %s %d", gAlliance == allianceBlue ? "blue" : "red", gCurAuto);
	if (gAlliance == allianceBlue)
	{
		switch (gCurAuto)
		{
			case 0: autoSkills(); break;
			case 1: autoStationaryBlueLeft(); break;
			case 2: autoSideMobileLeft(); break;
		}
	}
	else
	{
		switch (gCurAuto)
		{
			case 0: autoTest(); break;
			case 1: autoStationaryRedRight(); break;
			case 2: autoSideMobileRight(); break;
		}
	}
}

void skillsRaiseMobile()
{
	setMobile(MOBILE_UP_POWER);
	unsigned long timeout = nPgmTime + 2000;
	while (gSensor[mobilePoti].value < MOBILE_TOP && !TimedOut(timeout, "skills/mobile")) sleep(10);
	setMobile(MOBILE_UP_HOLD_POWER);
}

void raiseMobileMid()
{
	setMobile(MOBILE_UP_POWER);
	unsigned long timeout = nPgmTime + 1000;
	while (gSensor[mobilePoti].value < MOBILE_MIDDLE_UP && !TimedOut(timeout, "mid")) sleep(10);
	setMobile(15);
}

void lowerMobile()
{
	setMobile(MOBILE_DOWN_POWER);
	unsigned long timeout = nPgmTime + 2000;
	while (gSensor[mobilePoti].value > MOBILE_BOTTOM && !TimedOut(timeout, "lower")) sleep(10);
	setMobile(MOBILE_DOWN_HOLD_POWER);
}

void backupToLine()
{
	setDrive(-30, -30);
	while (!gSensor[leftLine].value && !gSensor[rightLine].value) sleep(10);
	if (gSensor[leftLine].value)
	{
		setDrive(7, -30);
		while (!gSensor[rightLine].value) sleep(10);
	}
	else
	{
		setDrive(-30, 7);
		while (!gSensor[leftLine].value) sleep(10);
	}
	setDrive(7, 7);
	resetPositionFull(gPosition, gPosition.y, gPosition.x, 235);
	sleep(200);
	setDrive(0, 0);
}

void driverSkillsStart()
{
	setClaw(CLAW_CLOSE_HOLD_POWER);
	resetPositionFull(gPosition, 19.5, 44.5, 45);
	setMobile(MOBILE_DOWN_POWER);
	unsigned long coneTimeout = nPgmTime + 2500;
	while (gSensor[mobilePoti].value > MOBILE_BOTTOM + 1000 && !TimedOut(coneTimeout, "skills 1")) sleep(10);
	moveToTargetAsync(36, 61, 19.5, 44.5, 127, 6, 2, 2, false, false);
	unsigned long driveTimeout = nPgmTime + 4000;
	while (gSensor[mobilePoti].value > MOBILE_BOTTOM && !TimedOut(coneTimeout, "skills 2")) sleep(10);
	setMobile(MOBILE_DOWN_HOLD_POWER);
	moveToTargetAwait(driveTimeout);
	sleep(500);
	scoreFirstExternal(gPosition.a);
}

void autoSkills()
{
	resetPositionFull(gPosition, 0, 0, 0);

	turnToAngleSimple(180, cw, 127, 127);
	sleep(500);
	writeDebugStreamLine("%f", radToDeg(gPosition.a));

	/*
	float angleSum = 0, velocitySum = 0;
	unsigned long count = 0;
	float angleMin = 1000, angleMax = -1, velocityMin = 1000, velocityMax = -1;

	while (true)
	{
		resetPositionFull(gPosition, 0, 0, 0);

		setDrive(29, -29);
		while (gPosition.a < degToRad(90)) EndTimeSlice();
		float velocity = gVelocity.a;
		velocitySum += velocity;
		count++;
		setDrive(-15, 15);
		sleep(150);
		setDrive(0, 0);
		sleep(500);
		float angle = gPosition.a;
		angleSum += angle;

		if (angle < angleMin) angleMin = angle;
		if (angle > angleMax) angleMax = angle;
		if (velocity < velocityMin) velocityMin = velocity;
		if (velocity > velocityMax) velocityMax = velocity;

		writeDebugStreamLine("%4d %f %f %f %f %f %f %f %f", count, radToDeg(angleSum / count), velocitySum / count, radToDeg(angleMin), radToDeg(angleMax), velocityMin, velocityMax, radToDeg(angleMax - angleMin), velocityMax - velocityMin);
	}
	*/

	/*
	resetPositionFull(gPosition, 19.5, 43.5, 45);

	// 1
	writeDebugStreamLine("1");
	lowerMobile();
	//moveToTargetSimple(48, 72, 127, false, false);
	moveToTargetDisSimple(45, 29, 127, 0, stopSoft, true);
	raiseMobileMid();

	// 2
	writeDebugStreamLine("2");
	turnToTarget(21, 45, cw, 55, 55, true, true);
	moveToTargetSimple(22, 46, 127, 0, stopHarsh | stopSoft, false);

	// 3
	writeDebugStreamLine("3");
	moveToTargetDisSimple(225, -1.2, -127, 0, stopHarsh | stopSoft, false);
	sleep(300);
	turnToTarget(8.5, 105.5, ccw, 50, 50);
	lowerMobile();
	moveToTargetSimple(8.5, 105.5, 127, 12, stopSoft, true);
	skillsRaiseMobile();

	// 7
	writeDebugStreamLine("7");
	turnToTarget(28, 44, ch, 30, 30, true, true, 180);
	moveToTargetSimple(28, 44, -127, 0, stopHarsh | stopSoft, true);
	turnToAngle(225, cw, 127, 127);
	setDrive(40, 40);
	sleep(400);
	setDrive(15, 15);
	lowerMobile();
	setDrive(0, 0);

	// 8
	writeDebugStreamLine("8");
	moveToTargetDisSimple(225, -1.5, -70, 0, stopHarsh | stopSoft, true);
	turnToTarget(50, 24, cw, 127, 127);
	moveToTargetSimple(50, 24, 127);
	turnToTarget(72, 49, cw, 127, 127);
	moveToTargetSimple(72, 49, 127, 9, stopSoft, true);
	raiseMobileMid();

	// 10
	writeDebugStreamLine("10");
	turnToTarget(50, 24, ccw, 50, 50);
	moveToTargetSimple(50, 24, gTargetLast.y, gTargetLast.x, 127, 0, stopHarsh | stopSoft, false);
	moveToTargetDisSimple(225, -2, -127, 0, stopHarsh | stopSoft, true);

	// 11
	writeDebugStreamLine("11");
	turnToAngle(325, cw, 60, 60, false, false);
	turnToTarget(98, 72, cw, 30, 30);
	lowerMobile();
	moveToTargetSimple(98, 72, 127, 10, stopSoft, true);
	raiseMobileMid();

	// 12
	writeDebugStreamLine("12");
	moveToTargetSimple(119, 97, 127, 0, stopHarsh | stopSoft, false);
	moveToTargetDisSimple(45, -1.2, -127, 0, stopHarsh | stopSoft, true);

	// 13
	writeDebugStreamLine("13");
	turnToTarget(97, 122, cw, 75, 75);
	moveToTargetSimple(97, 122, 127);
	turnToTarget(68, 97, cw, 65, 65);
	lowerMobile();
	moveToTargetSimple(68, 97, 127, 10, stopSoft, true);
	raiseMobileMid();

	// 15
	writeDebugStreamLine("15");
	turnToTarget(90, 122, ccw, 55, 55);
	moveToTargetSimple(90, 122, 127, 0, stopHarsh | stopSoft, false);
	moveToTargetDisSimple(45, -1.5, -127, 0, stopHarsh | stopSoft, true);

	// 16
	writeDebugStreamLine("16");
	turnToTarget(36, 124, cw, 60, 60);
	lowerMobile();
	moveToTargetSimple(36, 124, 127, 8, stopSoft, true);
	skillsRaiseMobile();

	// 17
	writeDebugStreamLine("17");
	turnToTarget(90, 117, ch, 45, 45, true, true, 180);
	moveToTargetSimple(90, 117, -127);
	*/
}

void autoStationaryCore(bool first, int liftUp, int liftDown, tTurnDir turnDir)
{
	unsigned long driveTimeout, coneTimeout;
	if (first)
	{
		//grabPreload();
		setClaw(CLAW_CLOSE_HOLD_POWER);
		sleep(200);
		setArm(-80);
		coneTimeout = nPgmTime + 2000;
		while (gSensor[armPoti].value > 1800 && !TimedOut(coneTimeout, "preload")) sleep(10);
		setArm(80);
		sleep(300);
		setArm(10);
	}
	else
	{
		turnToTargetAsync(48, 48, gPosition.y, gPosition.x, turnDir, 60, 60, true, true, 0);
		driveTimeout = nPgmTime + 1500;
		coneTimeout = nPgmTime + 1000;
		setArm(127);
		while (gSensor[armPoti].value < gArmPositions[2] && !TimedOut(coneTimeout, "sg core 1")) sleep(10);
		setArm(15);
		turnToTargetAwait(driveTimeout);
	}
	moveToTargetAsync(48, 48, gPosition.y, gPosition.x, 80, 4, 1, 12, true, true);
	driveTimeout = nPgmTime + 3000;
	gLiftTarget = liftUp;
	tStart(LiftTaskUp);
	coneTimeout = nPgmTime + 1000;
	while (!gLiftTargetReached && !TimedOut(coneTimeout, "sg core 2")) sleep(10);
	setLift(0);
	moveToTargetAwait(driveTimeout);
	sleep(500);
	setArm(-90);
	coneTimeout = nPgmTime + 500;
	while (gSensor[armPoti].value > (first ? 1850 : 1750) && !TimedOut(coneTimeout, "sg core 3")) sleep(10);
	setArm(10);
	//sleep(500);
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
	gPosition.y = 50;
	gPosition.x = 13.5;
	gPosition.a = 0;
	autoStationaryCore(true, 2000, 1600, cw);
	moveToTargetAsync(45, 25, gPosition.y, gPosition.x, -60, 6, 4, 4, false, true);
	unsigned long driveTimeout = nPgmTime + 2000;
	moveToTargetAwait(driveTimeout);
	tStart(dropArm);
	sleep(200);
	turnToTargetAsync(69, 22, gPosition.y, gPosition.x, ccw, 50, 50, true, true, 10);
	driveTimeout = nPgmTime + 1500;
	turnToTargetAwait(driveTimeout);
	setLift(-80);
	unsigned long coneTimeout = nPgmTime + 1000;
	while (!gSensor[limBottom].value && !TimedOut(coneTimeout, "sg lb 1")) sleep(10);
	setLift(-10);
	moveToTargetAsync(69, 22, gPosition.y, gPosition.x, 60, 8, 0.5, 8, false, true);
	driveTimeout = nPgmTime + 3000;
	moveToTargetAwait(driveTimeout);
	moveToTargetAsync(69, 22, gPosition.y, gPosition.x, 40, 6, 0.5, 1, true, true);
	driveTimeout = nPgmTime + 1500;
	sleep(200);
	setClaw(CLAW_CLOSE_POWER);
	coneTimeout = nPgmTime + 1000;
	while (gSensor[clawPoti].value > CLAW_CLOSE + 200 && !TimedOut(coneTimeout, "sg lb 2")) sleep(10);
	setClaw(CLAW_CLOSE_HOLD_POWER);
	sleep(300);
	moveToTargetAwait(driveTimeout);
	//gPosition.x += 1;
	autoStationaryCore(false, 2250, 2100, cw);
	moveToTarget(61, 35, 48, 48, -50, 6, 3, 3, false, true);
}

void autoStationaryRedRight()
{
	gPosition.y = 13.5;
	gPosition.x = 50;
	gPosition.a = PI / 2;
	autoStationaryCore(true, 2000, 1600, ccw);
	moveToTargetAsync(25, 45, gPosition.y, gPosition.x, -40, 6, 4, 4, false, true);
	unsigned long driveTimeout = nPgmTime + 2000;
	moveToTargetAwait(driveTimeout);
	tStart(dropArm);
	sleep(200);
	turnToTargetAsync(23, 69, gPosition.y, gPosition.x, cw, 50, 50, true, true, -10);
	driveTimeout = nPgmTime + 1500;
	turnToTargetAwait(driveTimeout);
	tStart(dropArm);
	setLift(-80);
	unsigned long coneTimeout = nPgmTime + 1000;
	while (!gSensor[limBottom].value && !TimedOut(coneTimeout, "sg lb 1")) sleep(10);
	moveToTargetAsync(23, 69, gPosition.y, gPosition.x, 60, 8, 0.5, 8, false, true);
	driveTimeout = nPgmTime + 3000;
	moveToTargetAwait(driveTimeout);
	moveToTargetAsync(23, 69, gPosition.y, gPosition.x, 40, 6, 0.5, 1, true, true);
	driveTimeout = nPgmTime + 1500;
	sleep(200);
	setClaw(CLAW_CLOSE_POWER);
	coneTimeout = nPgmTime + 1000;
	while (gSensor[clawPoti].value > CLAW_CLOSE + 200 && !TimedOut(coneTimeout, "sg lb 2")) sleep(10);
	setClaw(CLAW_CLOSE_HOLD_POWER);
	sleep(300);
	moveToTargetAwait(driveTimeout);
	sleep(100);
	//gPosition.x += 2.5;
	//gPosition.y += 4.5;
	autoStationaryCore(false, 2250, 2100, ccw);
	moveToTarget(35, 61, 48, 48, -50, 6, 3, 3, false, true);
}

void autoSideMobileLeft()
{
	gPosition.y = 50;
	gPosition.x = 13.5;
	gPosition.a = 0;
	setClaw(CLAW_CLOSE_HOLD_POWER);
	moveToTargetAsync(96, 13.5, 50, 13.5, 60, 4, 1.5, 1.5, false, true);
	unsigned long driveTimeout = nPgmTime + 5000;
	setMobile(MOBILE_DOWN_POWER);
	unsigned long mobileTimeout = nPgmTime + 2000;
	while (gSensor[mobilePoti].value > MOBILE_BOTTOM && !TimedOut(mobileTimeout, "sm L 1")) sleep(10);
	setMobile(MOBILE_DOWN_HOLD_POWER);
	moveToTargetAwait(driveTimeout);
	sleep(300);
	scoreFirstExternal(0);
	skillsRaiseMobile();
	//setMobile(MOBILE_UP_POWER);
	//mobileTimeout = nPgmTime + 3000;
	//while (gSensor[mobilePoti].value < 1600 && !TimedOut(mobileTimeout, "sm L 2")) sleep(10);
	//gArmDown = false;
	//startTask(dropArm);
	//unsigned long coneTimeout = nPgmTime + 1500;
	//while (gSensor[mobilePoti].value < MOBILE_TOP && !TimedOut(mobileTimeout, "sm L 3")) sleep(10);
	//setMobile(MOBILE_UP_HOLD_POWER);
	//while (!gArmDown && !TimedOut(coneTimeout, "sm L 7")) sleep(10);
	//moveToTarget(107, 15, 60, 2, 1.5, 1.5, false, false);
	//sleep(100);
	//stack(false);
	//sleep(200);
	moveToTarget(72, 15, -127, 6, 1.5, 3, false, false);
	turnToTarget(40, 36, ccw, 80, 10, false, true, 180);
	moveToTarget(40, 36, -80, 6, 4, 6, true, true);
	sleep(200);
	turnToTarget(0, 0, ccw, 60, 60, true, true, 0);
	//setMobile(MOBILE_DOWN_POWER);
	//mobileTimeout = nPgmTime + 1000;
	//while (gSensor[mobilePoti].value > MOBILE_MIDDLE_DOWN && !TimedOut(mobileTimeout, "sm L 4")) sleep(10);
	//setMobile(15);
	setDrive(100, 100);
	sleep(1000);
	setDrive(-127, -127);
	driveTimeout = nPgmTime + 1000;
	sleep(300);
	while (!gSensor[leftLine].value && !gSensor[rightLine].value && !TimedOut(driveTimeout, "sm L 5")) sleep(10);
	setDrive(0, 0);
}

void autoSideMobileRight()
{
	gPosition.y = 13.5;
	gPosition.x = 50;
	gPosition.a = PI / 2;
	setClaw(CLAW_CLOSE_HOLD_POWER);
	moveToTargetAsync(13.5, 95, 13.5, 50, 60, 4, 1.5, 1.5, false, true);
	unsigned long driveTimeout = nPgmTime + 5000;
	setMobile(MOBILE_DOWN_POWER);
	unsigned long mobileTimeout = nPgmTime + 2000;
	while (gSensor[mobilePoti].value > MOBILE_BOTTOM && !TimedOut(mobileTimeout, "sm R 1")) sleep(10);
	setMobile(MOBILE_DOWN_HOLD_POWER);
	moveToTargetAwait(driveTimeout);
	sleep(300);
	scoreFirstExternal(PI / 2);
	skillsRaiseMobile();
	//setMobile(MOBILE_UP_POWER);
	//mobileTimeout = nPgmTime + 3000;
	//while (gSensor[mobilePoti].value < 1600 && !TimedOut(mobileTimeout, "sm R 2")) sleep(10);
	//gArmDown = false;
	//startTask(dropArm);
	//unsigned long coneTimeout = nPgmTime + 1500;
	//while (gSensor[mobilePoti].value < MOBILE_TOP && !TimedOut(mobileTimeout, "sm R 3")) sleep(10);
	//setMobile(MOBILE_UP_HOLD_POWER);
	//while (!gArmDown && !TimedOut(coneTimeout, "sm L 7")) sleep(10);
	//moveToTarget(gPosition.y, 108, 60, 2, 1.5, 1.5, false, false);
	//sleep(100);
	//stack(false);
	//sleep(200);
	moveToTarget(15, 72, -127, 6, 1.5, 3, false, false);
	turnToTarget(30, 42, cw, 10, 80, false, true, 180);
	moveToTarget(30, 42, -80, 6, 4, 6, true, true);
	sleep(200);
	turnToTarget(0, 0, cw, 60, 60, true, true, 0);
	setMobile(MOBILE_DOWN_POWER);
	mobileTimeout = nPgmTime + 1000;
	while (gSensor[mobilePoti].value > MOBILE_MIDDLE_DOWN && !TimedOut(mobileTimeout, "sm R 4")) sleep(10);
	setMobile(15);
	setDrive(100, 100);
	sleep(1000);
	setDrive(-127, -127);
	driveTimeout = nPgmTime + 1000;
	sleep(300);
	while (!gSensor[leftLine].value && !gSensor[rightLine].value && !TimedOut(driveTimeout, "sm R 5")) sleep(10);
	setDrive(0, 0);
}

void autoTest()
{
	gPosition.x = gPosition.y = gPosition.a = 0;
	turnToAngle(90, cw, 60, 60, true, true);
	sleep(500);
	writeDebugStreamLine("%.2f %.2f %.2f", gPosition.y, gPosition.x, radToDeg(gPosition.a));
}
