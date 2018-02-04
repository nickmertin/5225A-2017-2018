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
			case 1: auto20BlueRight(); break;
			case 2: auto20BlueLeft(); break;
			case 3: auto5BlueRight(); break;
			case 4: auto5BlueLeft(); break;
			case 5: autoStationaryRight(); break;
			case 6: autoStationaryLeft(); break;
		}
	}
	else
	{
		switch (gCurAuto)
		{
			case 0: autoTest(); break;
			case 1: auto20RedLeft(); break;
			case 2: auto20RedRight(); break;
			case 3: auto5RedLeft(); break;
			case 4: auto5RedRight(); break;
			case 5: autoStationaryLeft(); break;
			case 6: autoStationaryRight(); break;
		}
	}
}

void normalize(float& x, float& y, float m, float b)
{
	float _b = y + x / m;
	x = (_b - b) / (m + 1 / m);
	y = m * x + b;
}

void resetBlueRight()
{
	setDrive(-30, -30);
	unsigned long timeout = nPgmTime + 1500;
	sleep(500);
	timeoutWhileLessThanF(&gVelocity.y, -0.1, timeout);
	setDrive(-7, -7);
#ifdef ULTRASONIC_RESET
	int count = 0;
	unsigned long value = 0;
	for (int i = 0; i < 10 || !count; ++i)
	{
		word cur = gSensor[sonar].value;
		if (cur > 1500 && cur < 3200)
		{
			value += cur;
			++count;
		}
		else if (i == 20)
		{
			resetPositionFull(gPosition, 8.25, gPosition.x, 0);
			return;
		}
		sleep(20);
	}
	resetPositionFull(gPosition, 8.25, 53.5 + (float)value / count / 148, 0);
#else
	sleep(500);
	resetPositionFull(gPosition, 8.25, gPosition.x, 0);
#endif
}

NEW_ASYNC_VOID_0(resetBlueRight);

void autoSkills()
{
	byte driveAsync;
	byte coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	sSimpleConfig liftConfig;
	sSimpleConfig armConfig;
	float _x;
	float _y;

	gMobileCheckLift = true;

#ifdef SKILLS_RESET_AT_START
	trackPositionTaskKill();
	resetPositionFull(gPosition, 40, 16, 45);
	//resetPositionFull(gPosition, 56, 8.25, 90);
	resetVelocity(gVelocity, gPosition);
	trackPositionTaskAsync();
#endif

	//resetQuadratureEncoder(liftEnc);
	//goto skip;

	liftSet(liftResetEncoder);
	coneTimeout = nPgmTime + 1400;

	// 1
	driveAsync = moveToTargetSimpleAsync(71, 47, 40, 16, 127, 24, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftResetEncoder, coneTimeout);
	configure(liftConfig, LIFT_MOBILE_THRESHOLD, 127, -10);
	liftSet(liftRaiseSimple, &liftConfig);
	coneTimeout = nPgmTime + 1000;
	timeoutWhileLessThanL(&gSensor[liftEnc].value, LIFT_MOBILE_THRESHOLD, coneTimeout);
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 1000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout);
	await(driveAsync, driveTimeout, "skills 1-1");
	setDrive(0, 0);
	driveAsync = moveToTargetSimpleAsync(71, 47, gPosition.y, gPosition.x, 127, 4, stopSoft, true);
	driveTimeout = nPgmTime + 1000;
	await(driveAsync, driveTimeout, "skills 1-2");
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout);

	// 2
	driveAsync = turnToTargetNewAsync(39, 14, ch, 0);
	driveTimeout = nPgmTime + 5000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "skills 2-1");
	driveAsync = moveToTargetSimpleAsync(39, 14, gPosition.y, gPosition.x, 80, 2, stopSoft, true);
	driveTimeout = nPgmTime + 2000;
	liftTimeoutWhile(liftLowerSimple, coneTimeout);
	configure(armConfig, ARM_PRESTACK - 100, -127, 20);
	armSet(armLowerSimple, &armConfig);
	await(driveAsync, driveTimeout, "skills 2-2");
	//coneAsync = clearArmAsync();
	//coneTimeout = nPgmTime + 2000;
	//await(coneAsync, coneTimeout, "skills 2-3");
	configure(liftConfig, LIFT_MOBILE_THRESHOLD + 10, 127, 0);
	liftSet(liftRaiseSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	liftTimeoutWhile(liftRaiseSimple, coneTimeout);
	mobileSet(mobileBottom, 0);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout);

	// 3
	driveAsync = moveToTargetDisSimpleAsync(gPosition.a + PI, 8, gPosition.y, gPosition.x, -80, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 3-1");
	driveAsync = turnToTargetNewAsync(16, 60, cw, PI);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	configure(armConfig, ARM_TOP, 127, 0);
	armSet(armRaiseSimple, &armConfig);
	await(driveAsync, driveTimeout, "skills 3-2");
	driveAsync = moveToTargetSimpleAsync(16, 60, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 3-3");
	driveAsync = turnToAngleNewAsync(0, cw);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 3-4");
	driveAsync = resetBlueRightAsync();
	driveTimeout = nPgmTime + 2000;
	await(driveAsync, driveTimeout, "skills 3-5");

	// 4
	driveAsync = moveToTargetDisSimpleAsync(0, 19, 0, gPosition.x, 80, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	await(driveAsync, driveTimeout, "skills 4-1");
	driveAsync = turnToTargetNewAsync(13, 107, cw, 0);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 4-2");
	driveAsync = moveToTargetSimpleAsync(13, 107, gPosition.y, gPosition.x, 80, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	await(driveAsync, driveTimeout, "skills 4-3");
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout);

	// 5
	driveAsync = turnToTargetNewAsync(30, 40, ch, PI);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	await(driveAsync, driveTimeout, "skills 5-1");
	driveAsync = moveToTargetSimpleAsync(30, 40, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 5-2");
	driveAsync = turnToAngleNewAsync(-135, cw);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 5-3");
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	driveAsync = moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 10.5, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "skills 5-4");
	//driveAsync = moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 15, _y, _x, 30, 0, stopNone, false);
	//driveTimeout = nPgmTime + 1500;
	//await(driveAsync, driveTimeout, "skills 5-5");
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gVelocity.y, -0.05, driveTimeout);
	setDrive(7, 7);
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout);
	sleep(300);
	driveAsync = moveToTargetDisSimpleAsync(PI / 4, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	setMobile(-7);
	await(driveAsync, driveTimeout, "skills 5-6");
	mobileSet(mobileManaged, 0);

	// 6
	driveAsync = turnToTargetNewAsync(16, 60, cw, PI);
	driveTimeout = nPgmTime + 3000;
	mobileSet(mobileBottom, 0);
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	await(driveAsync, driveTimeout, "skills 6-1");
	driveAsync = moveToTargetSimpleAsync(16, 60, gPosition.y, gPosition.x, -127, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 6-2");
	driveAsync = turnToAngleNewAsync(0, cw);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 6-3");
	driveAsync = resetBlueRightAsync();
	driveTimeout = nPgmTime + 2000;
	await(driveAsync, driveTimeout, "skills 6-4");

	// 7
	driveAsync = moveToTargetDisSimpleAsync(0, 18, 0, gPosition.x, 80, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	await(driveAsync, driveTimeout, "skills 7-1");
	driveAsync = turnToTargetStupidAsync(47, 71, ch, 0);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 7-2");
	driveAsync = moveToTargetSimpleAsync(47, 71, gPosition.y, gPosition.x, 80, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	await(driveAsync, driveTimeout, "skills 7-3");
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout);

	// 8
	driveAsync = turnToTargetNewAsync(14, 39, ccw, 0);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	await(driveAsync, driveTimeout, "skills 8-1");
	driveAsync = moveToTargetSimpleAsync(14, 39, gPosition.y, gPosition.x, 80, 2, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	await(driveAsync, driveTimeout, "skills 8-2");
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout);

	// 9
	driveAsync = moveToTargetDisSimpleAsync(gPosition.a + PI, 7, gPosition.y, gPosition.x, -80, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	await(driveAsync, driveTimeout, "skills 9-1");
	driveAsync = turnToTargetNewAsync(50, 18, ccw, PI);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 9-2");
	driveAsync = moveToTargetSimpleAsync(50, 18, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 9-3");
	driveAsync = turnToAngleNewAsync(90, ccw);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 9-4");
	setDrive(-30, -30);
	driveTimeout = nPgmTime + 1500;
	sleep(500);
	timeoutWhileLessThanF(&gVelocity.x, -0.1, driveTimeout);
	setDrive(-7, -7);
	sleep(500);
	resetPositionFull(gPosition, gPosition.y, 8.25, 90);
	//return;
	skip:

	// 10
	driveAsync = moveToTargetDisSimpleAsync(PI / 2, 31, gPosition.y, 0, 50, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	//mobileSet(mobileBottom, -1);
	await(driveAsync, driveTimeout, "skills 10-1");
	//return;
	driveAsync = turnToTargetCustomAsync(94, 72, ccw, 0, 25, 0.05);
	driveTimeout = nPgmTime + 8000;
	//configure(liftConfig, LIFT_BOTTOM, -127, 0);
	//liftSet(liftLowerSimple, &liftConfig);
	await(driveAsync, driveTimeout, "skills 10-2");
	//return;
	driveAsync = moveToTargetSimpleAsync(94, 72, gPosition.y, gPosition.x, 80, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	await(driveAsync, driveTimeout, "skills 10-3");
	//return;
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout);

	// 11
	driveAsync = turnToTargetStupidAsync(112, 95, ch, 0);
	driveTimeout = nPgmTime + 2000;
	await(driveAsync, driveTimeout, "skills 11-1");
	driveAsync = moveToTargetSimpleAsync(112, 95, gPosition.y, gPosition.x, 80, 2, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	await(driveAsync, driveTimeout, "skills 11-2");
	driveAsync = turnToTargetStupidAsync(110, 110, cw, 0);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 11-3");
	driveAsync = moveToTargetSimpleAsync(110, 110, gPosition.y, gPosition.x, 80, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	await(driveAsync, driveTimeout, "skills 11-4");
	driveAsync = turnToAngleNewAsync(45, ccw);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 11-5");
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 228);
	driveAsync = moveToTargetDisSimpleAsync(PI / 4, 10.5, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "skills 11-6");
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileGreaterThanF(&gVelocity.y, 0.05, driveTimeout);
	setDrive(7, 7);
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout);
	sleep(300);
	driveAsync = moveToTargetDisSimpleAsync(-0.75 * PI, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 11-7");
	mobileSet(mobileTop, -1);
	return;

	// 12
	driveAsync = turnToTargetNewAsync(23, 106, ccw, PI);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	await(driveAsync, driveTimeout, "skills 12-1");
	driveAsync = moveToTargetSimpleAsync(23, 106, gPosition.y, gPosition.x, 127, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 12-2");
	return;

	// OLD
	driveAsync = turnToTargetNewAsync(40, 64, ccw, 0);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 9-2");
	driveAsync = moveToTargetSimpleAsync(40, 64, gPosition.y, gPosition.x, 60, 1, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	await(driveAsync, driveTimeout, "skills 9-3");
	driveAsync = turnToTargetStupidAsync(71, 95, ch, 0);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 9-4");
	driveAsync = moveToTargetSimpleAsync(71, 95, gPosition.y, gPosition.x, 127, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 9-5");
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout);

	// 10
	driveAsync = turnToTargetStupidAsync(59, 112, ch, 0);
	driveTimeout = nPgmTime + 2000;
	await(driveAsync, driveTimeout, "skills 10-1");
	driveAsync = moveToTargetSimpleAsync(59, 112, gPosition.y, gPosition.x, 80, 2, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	await(driveAsync, driveTimeout, "skills 10-2");
	driveAsync = turnToTargetNewAsync(110, 110, ccw, 0);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 10-3");
	driveAsync = moveToTargetSimpleAsync(110, 110, gPosition.y, gPosition.x, 80, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	await(driveAsync, driveTimeout, "skills 10-4");
	driveAsync = turnToAngleNewAsync(45, cw);
	driveTimeout = nPgmTime + 3000;
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 198);
	driveAsync = moveToTargetDisSimpleAsync(PI / 4, 10.5, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "skills 5-4");
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileGreaterThanF(&gVelocity.y, 0.05, driveTimeout);
	setDrive(7, 7);
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout);
	sleep(300);
	driveAsync = moveToTargetDisSimpleAsync(0.75 * PI, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 5-6");
	mobileSet(mobileTop, -1);

	// 11
	driveAsync = turnToTargetNewAsync(23, 106, ccw, PI);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	await(driveAsync, driveTimeout, "skills 11-1");
	driveAsync = moveToTargetSimpleAsync(23, 106, gPosition.y, gPosition.x, 127, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "skills 11-2");
}

void auto20RedLeft()
{
	byte driveAsync;
	byte coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	sSimpleConfig liftConfig;
	sSimpleConfig armConfig;
	float _x;
	float _y;

	gMobileCheckLift = true;

	trackPositionTaskKill();
	resetPositionFull(gPosition, 40, 16, 45);
	resetVelocity(gVelocity, gPosition);
	trackPositionTaskAsync();

	liftSet(liftResetEncoder);
	coneTimeout = nPgmTime + 1400;

	// 1
	driveAsync = moveToTargetDisSimpleAsync(PI / 4, 10, gPosition.y, gPosition.x, 60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "rl20 1-1");
	driveAsync = turnToTargetNewAsync(107, 13, ccw, 0);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "rl20 1-2");
	driveAsync = moveToTargetSimpleAsync(107, 13, gPosition.y, gPosition.x, 127, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftResetEncoder, coneTimeout);
	mobileSet(mobileBottom, -1);
	await(driveAsync, driveTimeout, "rl20 1-3");
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout);

	// 2
	driveAsync = turnToTargetNewAsync(45, 25, ch, PI);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "rl20 2-1");
	driveAsync = moveToTargetSimpleAsync(45, 25, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftLowerSimple, coneTimeout);
	configure(armConfig, ARM_PRESTACK - 100, -127, 20);
	armSet(armLowerSimple, &armConfig);
	await(driveAsync, driveTimeout, "rl20 2-2");
	driveAsync = turnToAngleNewAsync(-135, ccw);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "rl20 2-3");
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	driveAsync = moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 10.5, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "rl20 2-4");
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gVelocity.y, -0.05, driveTimeout);
	setDrive(7, 7);
	configure(liftConfig, LIFT_MOBILE_THRESHOLD + 10, 127, 0);
	liftSet(liftRaiseSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	liftTimeoutWhile(liftRaiseSimple, coneTimeout);
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout);
	sleep(300);
	driveAsync = moveToTargetDisSimpleAsync(PI / 4, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	setMobile(-7);
	await(driveAsync, driveTimeout, "rl20 2-5");
	mobileSet(mobileManaged, 0);
}

void auto20RedRight()
{
	byte driveAsync;
	byte coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	sSimpleConfig liftConfig;
	sSimpleConfig armConfig;
	float _x;
	float _y;

	gMobileCheckLift = true;

	trackPositionTaskKill();
	resetPositionFull(gPosition, 16, 40, 45);
	resetVelocity(gVelocity, gPosition);
	trackPositionTaskAsync();

	liftSet(liftResetEncoder);
	coneTimeout = nPgmTime + 1400;

	// 1
	driveAsync = moveToTargetDisSimpleAsync(PI / 4, 6, gPosition.y, gPosition.x, 60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "rr20 1-1");
	driveAsync = turnToTargetCustomAsync(14, 107, cw, 0, 40, 0.12);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "rr20 1-2");
	driveAsync = moveToTargetSimpleAsync(14, 107, gPosition.y, gPosition.x, 127, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftResetEncoder, coneTimeout);
	mobileSet(mobileBottom, -1);
	await(driveAsync, driveTimeout, "rr20 1-3");
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout);

	// 2
	driveAsync = turnToTargetStupidAsync(25, 45, ch, PI);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "rr20 2-1");
	driveAsync = moveToTargetSimpleAsync(25, 45, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftLowerSimple, coneTimeout);
	configure(armConfig, ARM_PRESTACK - 100, -127, 20);
	armSet(armLowerSimple, &armConfig);
	await(driveAsync, driveTimeout, "rr20 2-2");
	driveAsync = turnToAngleNewAsync(-135, cw);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "rr20 2-3");
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	driveAsync = moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 10.5, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "rr20 2-4");
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gVelocity.y, -0.05, driveTimeout);
	setDrive(7, 7);
	configure(liftConfig, LIFT_MOBILE_THRESHOLD + 10, 127, 0);
	liftSet(liftRaiseSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	liftTimeoutWhile(liftRaiseSimple, coneTimeout);
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout);
	sleep(300);
	driveAsync = moveToTargetDisSimpleAsync(PI / 4, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	setMobile(-7);
	await(driveAsync, driveTimeout, "rr20 2-5");
	mobileSet(mobileManaged, 0);
}

void auto20BlueRight()
{
	byte driveAsync;
	byte coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	sSimpleConfig liftConfig;
	sSimpleConfig armConfig;
	float _x;
	float _y;

	gMobileCheckLift = true;

	trackPositionTaskKill();
	resetPositionFull(gPosition, 16, 40, 45);
	resetVelocity(gVelocity, gPosition);
	trackPositionTaskAsync();

	liftSet(liftResetEncoder);
	coneTimeout = nPgmTime + 1400;

	// 1
	driveAsync = moveToTargetDisSimpleAsync(PI / 4, 10, gPosition.y, gPosition.x, 60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "br20 1-1");
	driveAsync = turnToTargetNewAsync(13, 107, cw, 0);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "br20 1-2");
	driveAsync = moveToTargetSimpleAsync(13, 107, gPosition.y, gPosition.x, 127, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftResetEncoder, coneTimeout);
	mobileSet(mobileBottom, -1);
	await(driveAsync, driveTimeout, "br20 1-3");
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout);

	// 2
	driveAsync = turnToTargetNewAsync(25, 45, ch, PI);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "br20 2-1");
	driveAsync = moveToTargetSimpleAsync(25, 45, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftLowerSimple, coneTimeout);
	configure(armConfig, ARM_PRESTACK - 100, -127, 20);
	armSet(armLowerSimple, &armConfig);
	await(driveAsync, driveTimeout, "br20 2-2");
	driveAsync = turnToAngleNewAsync(-135, cw);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "br20 2-3");
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	driveAsync = moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 10.5, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "br20 2-4");
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gVelocity.y, -0.05, driveTimeout);
	setDrive(7, 7);
	configure(liftConfig, LIFT_MOBILE_THRESHOLD + 10, 127, 0);
	liftSet(liftRaiseSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	liftTimeoutWhile(liftRaiseSimple, coneTimeout);
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout);
	sleep(300);
	driveAsync = moveToTargetDisSimpleAsync(PI / 4, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	setMobile(-7);
	await(driveAsync, driveTimeout, "br20 2-5");
	mobileSet(mobileManaged, 0);
}

void auto20BlueLeft()
{
	byte driveAsync;
	byte coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	sSimpleConfig liftConfig;
	sSimpleConfig armConfig;
	float _x;
	float _y;

	gMobileCheckLift = true;

	trackPositionTaskKill();
	resetPositionFull(gPosition, 40, 16, 45);
	resetVelocity(gVelocity, gPosition);
	trackPositionTaskAsync();

	liftSet(liftResetEncoder);
	coneTimeout = nPgmTime + 1400;

	// 1
	driveAsync = moveToTargetDisSimpleAsync(PI / 4, 6, gPosition.y, gPosition.x, 60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "bl20 1-1");
	driveAsync = turnToTargetCustomAsync(107, 14, ccw, 0, 40, 0.12);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "bl20 1-2");
	driveAsync = moveToTargetSimpleAsync(107, 14, gPosition.y, gPosition.x, 127, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftResetEncoder, coneTimeout);
	mobileSet(mobileBottom, -1);
	await(driveAsync, driveTimeout, "bl20 1-3");
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout);

	// 2
	driveAsync = turnToTargetStupidAsync(45, 25, ch, PI);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "rl20 2-1");
	driveAsync = moveToTargetSimpleAsync(45, 25, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftLowerSimple, coneTimeout);
	configure(armConfig, ARM_PRESTACK - 100, -127, 20);
	armSet(armLowerSimple, &armConfig);
	await(driveAsync, driveTimeout, "rl20 2-2");
	driveAsync = turnToAngleNewAsync(-135, ccw);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "rl20 2-3");
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	driveAsync = moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 10.5, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "rl20 2-4");
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gVelocity.y, -0.05, driveTimeout);
	setDrive(7, 7);
	configure(liftConfig, LIFT_MOBILE_THRESHOLD + 10, 127, 0);
	liftSet(liftRaiseSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	liftTimeoutWhile(liftRaiseSimple, coneTimeout);
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout);
	sleep(300);
	driveAsync = moveToTargetDisSimpleAsync(PI / 4, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	setMobile(-7);
	await(driveAsync, driveTimeout, "rl20 2-5");
	mobileSet(mobileManaged, 0);
}

void auto5RedLeft()
{
	byte driveAsync;
	byte coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	sSimpleConfig liftConfig;
	sSimpleConfig armConfig;
	float _x;
	float _y;

	gMobileCheckLift = true;

	trackPositionTaskKill();
	resetPositionFull(gPosition, 40, 16, 45);
	resetVelocity(gVelocity, gPosition);
	trackPositionTaskAsync();

	liftSet(liftResetEncoder);
	coneTimeout = nPgmTime + 1400;

	// 1
	driveAsync = moveToTargetDisSimpleAsync(PI / 4, 10, gPosition.y, gPosition.x, 60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "rl5 1-1");
	driveAsync = turnToTargetCustomAsync(107, 14, ccw, 0, 35, 0.12);
	driveTimeout = nPgmTime + 3000;
	await(driveAsync, driveTimeout, "rl5 1-2");
	driveAsync = moveToTargetSimpleAsync(107, 14, gPosition.y, gPosition.x, 127, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftResetEncoder, coneTimeout);
	mobileSet(mobileBottom, -1);
	await(driveAsync, driveTimeout, "rl5 1-3");
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout);

	// 2
	driveAsync = turnToTargetCustomAsync(72, 24, ch, PI, 35, 0.12);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "rl5 2-1");
	driveAsync = moveToTargetSimpleAsync(72, 24, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftLowerSimple, coneTimeout);
	configure(armConfig, ARM_PRESTACK - 100, -127, 20);
	armSet(armLowerSimple, &armConfig);
	await(driveAsync, driveTimeout, "rl5 2-2");
	driveAsync = turnToAngleCustomAsync(-0.75 * PI, ccw, 80, 0.2);
	driveTimeout = nPgmTime + 2000;
	await(driveAsync, driveTimeout, "rl5 2-3");
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout);
	driveAsync = moveToTargetDisSimpleAsync(gPosition.a + PI, 6, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, false);
	driveTimeout = nPgmTime + 2000;
	await(driveAsync, driveTimeout, "rl5 2-4");
}

void auto5RedRight()
{
}

void auto5BlueRight()
{
}

void auto5BlueLeft()
{
}

void autoStationaryLeft()
{
	byte driveAsync;
	byte coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	sSimpleConfig liftConfig;
	sSimpleConfig armConfig;
	float _x;
	float _y;

	gMobileCheckLift = true;

	trackPositionTaskKill();
	resetPositionFull(gPosition, 40, 16, 45);
	resetVelocity(gVelocity, gPosition);
	trackPositionTaskAsync();

	liftSet(liftResetEncoder);
	coneTimeout = nPgmTime + 1400;

	// 1
	driveAsync = moveToTargetDisSimpleAsync(PI / 4, 35, gPosition.y, gPosition.x, 70, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	liftTimeoutWhileAsync(liftResetEncoder, coneTimeout);
	//configure(liftConfig,
	await(driveAsync, driveTimeout, "ls 1-1");
	driveAsync = turnToTargetCustomAsync(47, 47, ccw, 0, 60, 0.2);
	driveTimeout = nPgmTime + 2000;
	//await(
}

void autoStationaryRight()
{
}

void autoTest()
{
	byte driveAsync;
	byte coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	sSimpleConfig liftConfig;
	sSimpleConfig armConfig;
	float _x;
	float _y;

	gMobileCheckLift = true;

	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	driveAsync = moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 10.5, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "skills 4-4");
	driveAsync = moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 16, _y, _x, 30, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	await(driveAsync, driveTimeout, "skills 4-5");
	setDrive(7, 7);
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout);
	sleep(300);
	driveAsync = moveToTargetDisSimpleAsync(PI / 4, 13, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	sleep(300);
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 2000;
	await(driveAsync, driveTimeout, "skills 4-6");
	return;
	//for (int i = 0; i < 20; ++i)
	//{
	trackPositionTaskKill();
	resetPositionFull(gPosition, 0, 0, 0);
	trackPositionTaskAsync();
	//setDrive(80, 80);
	//unsigned long timeout = nPgmTime + 2000;
	//while (gPosition.y < 23 && !TimedOut(timeout, "test 1")) sleep(10);
	//setDrive(-15, -15);
	//sleep(200);
	//mobileSet(mobileDownToMiddle);
	//sleep(200);
	//setDrive(10, 10);
	//sleep(300);
	//setDrive(-80, -80);
	//timeout = nPgmTime + 1500;
	//while (gPosition.y > 10 && !TimedOut(timeout, "test 2")) sleep(10);
	//setDrive(0, 0);

	//moveToTargetSimple(72, 0, 127, 0);
	//turnToTargetSimple(0, -10, ccw, 127, 127, 0);
	//trackPositionTaskAsync();

	//moveToTargetSimple(50, 0, 127);
	//turnToTargetSimple(0, 0, ccw, 127, 127);
	//moveToTargetSimple(0, 0, 127, 10);
	//turnToAngleSimple(0, ccw, 127, 127);

	//moveToTargetSimple(5, 0, -80, 0, stopSoft | stopHarsh, true);
	//moveToTargetDisSimple(0, 5, -80, 0, stopSoft | stopHarsh, true);

	//turnToTargetNew(0, 100, cw, 0);
	//turnToTargetNew(100, 100, cw, 0);
	//playSound(soundBlip);
	//sleep(1000);
	//writeDebugStreamLine("%f", radToDeg(gPosition.a - atan2(100 - gPosition.x, 100 - gPosition.y)));
	//writeDebugStreamLine("%f", radToDeg(gPosition.a) - 45);
//}
	moveToTargetSimple(72, 0, 127, 0, stopSoft | stopHarsh, true);
}
