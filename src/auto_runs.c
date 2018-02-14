void selectAuto()
{
	updateSensorInput(autoPoti);
	int autoVal = gSensor[autoPoti].value - 2048;
	if (autoVal < 0) gAlliance = allianceBlue;
	else gAlliance = allianceRed;

	autoVal = abs(autoVal);
	if (autoVal < 275) gCurAuto = 0;
	else if (autoVal < 550) gCurAuto = 1;
	else if (autoVal < 825) gCurAuto = 2;
	else if (autoVal < 1100) gCurAuto = 3;
	else if (autoVal < 1375) gCurAuto = 4;
	else if (autoVal < 1650) gCurAuto = 5;
	else if (autoVal < 1925) gCurAuto = 6;
	else gCurAuto = 7;
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
			case 3: autoStationaryRightBlock(); break;
			case 4: autoStationaryLeftBlock(); break;
			case 5: autoStationaryRight5(); break;
			case 6: autoStationaryLeft5(); break;
			case 7: autoStationaryLeft2(); break;
		}
	}
	else
	{
		switch (gCurAuto)
		{
			case 0: autoBlock(); break;
			case 1: auto20RedLeft(); break;
			case 2: auto20RedRight(); break;
			case 3: autoStationaryLeftBlock(); break;
			case 4: autoStationaryRightBlock(); break;
			case 5: autoStationaryLeft5(); break;
			case 6: autoStationaryRight5(); break;
			case 7: autoStationaryRight2(); break;
		}
	}
}

void normalize(float& x, float& y, float m, float b)
{
	float _b = y + x / m;
	x = (_b - b) / (m + 1 / m);
	y = m * x + b;
}

void autoSkills()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
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
	moveToTargetSimpleAsync(71, 47, 40, 16, 127, 24, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftResetEncoder, coneTimeout, "skills 1-1");
	configure(liftConfig, LIFT_MOBILE_THRESHOLD, 127, -10);
	liftSet(liftRaiseSimple, &liftConfig);
	coneTimeout = nPgmTime + 1000;
	timeoutWhileLessThanL(&gSensor[liftEnc].value, LIFT_MOBILE_THRESHOLD, coneTimeout, "skills 1-2");
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 1000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, "skills 1-3");
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "skills 1-4");
	setDrive(0, 0);
	moveToTargetSimpleAsync(71, 47, gPosition.y, gPosition.x, 127, 4, stopSoft, true);
	driveTimeout = nPgmTime + 1000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "skills 1-5");
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, "skills 1-6");

	// 2
	turnToTargetNewAsync(39, 14, ch, 0);
	driveTimeout = nPgmTime + 5000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, "skills 2-1");
	moveToTargetSimpleAsync(39, 14, gPosition.y, gPosition.x, 80, 2, stopSoft, true);
	driveTimeout = nPgmTime + 2000;
	liftTimeoutWhile(liftLowerSimple, coneTimeout, "skills 2-2");
	configure(armConfig, ARM_PRESTACK - 100, -127, 20);
	armSet(armLowerSimple, &armConfig);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "skills 2-2");
	//coneAsync = clearArmAsync();
	//coneTimeout = nPgmTime + 2000;
	//await(coneAsync, coneTimeout, "skills 2-3");
	configure(liftConfig, LIFT_MOBILE_THRESHOLD + 10, 127, 0);
	liftSet(liftRaiseSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	liftTimeoutWhile(liftRaiseSimple, coneTimeout, "skills 2-3");
	mobileSet(mobileBottom, 0);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, "skills 2-4");

	// 3
	moveToTargetDisSimpleAsync(gPosition.a + PI, 8, gPosition.y, gPosition.x, -80, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "skills 3-1");
	turnToTargetNewAsync(16, 60, cw, PI);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	configure(armConfig, ARM_TOP, 127, 0);
	armSet(armRaiseSimple, &armConfig);
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, "skills 3-2");
	moveToTargetSimpleAsync(16, 60, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "skills 3-3");
	turnToAngleNewAsync(0, cw);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToAngleNewState, driveTimeout, "skills 3-4");
	resetBlueRightAsync();
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(resetBlueRightState, driveTimeout, "skills 3-5");

	// 4
	moveToTargetDisSimpleAsync(0, 19, 0, gPosition.x, 80, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "skills 4-1");
	turnToTargetNewAsync(13, 107, cw, 0);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, "skills 4-2");
	moveToTargetSimpleAsync(13, 107, gPosition.y, gPosition.x, 80, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "skills 4-3");
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, "skills 4-4");

	// 5
	turnToTargetNewAsync(30, 40, ch, PI);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, "skills 5-1");
	moveToTargetSimpleAsync(30, 40, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "skills 5-2");
	turnToAngleNewAsync(-135, cw);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToAngleNewState, driveTimeout, "skills 5-3");
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 10.5, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "skills 5-4");
	//moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 15, _y, _x, 30, 0, stopNone, false);
	//driveTimeout = nPgmTime + 1500;
	//await(driveAsync, driveTimeout, "skills 5-5");
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gVelocity.y, -0.05, driveTimeout, "skills 5-5");
	setDrive(7, 7);
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, "skills 5-6");
	sleep(300);
	moveToTargetDisSimpleAsync(PI / 4, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	setMobile(-7);
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "skills 5-7");
	mobileSet(mobileManaged, 0);

	// 6
	turnToTargetNewAsync(16, 60, cw, PI);
	driveTimeout = nPgmTime + 3000;
	mobileSet(mobileBottom, 0);
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, "skills 6-1");
	moveToTargetSimpleAsync(16, 60, gPosition.y, gPosition.x, -127, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "skills 6-2");
	turnToAngleNewAsync(0, cw);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToAngleNewState, driveTimeout, "skills 6-3");
	resetBlueRightAsync();
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(resetBlueRightState, driveTimeout, "skills 6-4");

	// 7
	moveToTargetDisSimpleAsync(0, 18, 0, gPosition.x, 80, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "skills 7-1");
	turnToTargetStupidAsync(47, 71, ch, 0);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToTargetStupidState, driveTimeout, "skills 7-2");
	moveToTargetSimpleAsync(47, 71, gPosition.y, gPosition.x, 80, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "skills 7-3");
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, "skills 7-4");

	// 8
	turnToTargetNewAsync(14, 39, ccw, 0);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, "skills 8-1");
	moveToTargetSimpleAsync(14, 39, gPosition.y, gPosition.x, 80, 2, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "skills 8-2");
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, "skills 8-3");

	// 9
	moveToTargetDisSimpleAsync(gPosition.a + PI, 7, gPosition.y, gPosition.x, -80, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "skills 9-1");
	turnToTargetNewAsync(52, 18, ccw, PI);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, "skills 9-2");
	moveToTargetSimpleAsync(52, 18, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "skills 9-3");
	turnToAngleNewAsync(90, ccw);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToAngleNewState, driveTimeout, "skills 9-4");
	setDrive(-30, -30);
	driveTimeout = nPgmTime + 1500;
	sleep(500);
	timeoutWhileLessThanF(&gVelocity.x, -0.1, driveTimeout, "skills 9-5");
	setDrive(-7, -7);
	sleep(500);
	resetPositionFull(gPosition, gPosition.y, 8.25, 90);
	//return;
	skip:

	// 10
	moveToTargetDisSimpleAsync(PI / 2, 31, gPosition.y, 0, 50, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	//mobileSet(mobileBottom, -1);
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "skills 10-1");
	//return;
	turnToTargetCustomAsync(95, 71, ccw, 0, 25, 0.05);
	driveTimeout = nPgmTime + 8000;
	//configure(liftConfig, LIFT_BOTTOM, -127, 0);
	//liftSet(liftLowerSimple, &liftConfig);
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "skills 10-2");
	//return;
	moveToTargetSimpleAsync(94, 72, gPosition.y, gPosition.x, 80, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "skills 10-3");
	//return;
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, "skills 10-4");

	// 11
	turnToTargetStupidAsync(112, 95, ch, 0);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToTargetStupidState, driveTimeout, "skills 11-1");
	moveToTargetSimpleAsync(112, 95, gPosition.y, gPosition.x, 80, 2, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "skills 11-2");
	turnToTargetCustomAsync(110, 110, cw, 0, 45, 0.3);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "skills 11-3");
	moveToTargetSimpleAsync(110, 110, gPosition.y, gPosition.x, 80, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "skills 11-4");
	turnToAngleCustomAsync(45, ccw, 70, 0.2);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToAngleCustomState, driveTimeout, "skills 11-5");
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 228);
	moveToTargetDisSimpleAsync(PI / 4, 12, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "skills 11-6");
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileGreaterThanF(&gVelocity.y, 0.05, driveTimeout, "skills 11-7");
	setDrive(7, 7);
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, "skills 11-8");
	sleep(300);
	moveToTargetDisSimpleAsync(-0.75 * PI, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "skills 11-9");
	mobileSet(mobileTop, -1);
	return;

	// 12
	turnToTargetNewAsync(23, 106, ccw, PI);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, "skills 12-1");
	moveToTargetSimpleAsync(23, 106, gPosition.y, gPosition.x, 127, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "skills 12-2");
}

void autoBlock()
{
	setDrive(-127, -127);
	sleep(2000);
	setDrive(7, 7);
}

void auto20RedLeft()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
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
	moveToTargetDisSimpleAsync(PI / 4, 10, gPosition.y, gPosition.x, 60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "rl20 1-1");
	turnToTargetCustomAsync(107, 13, ccw, 0, 40, 0.2);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftResetEncoder, coneTimeout, "rl20 1-2");
	configure(liftConfig, gLiftRaiseTarget[0], 70, -15);
	liftSet(liftRaiseSimple, &liftConfig);
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "rl20 1-3");
	moveToTargetSimpleAsync(107, 13, gPosition.y, gPosition.x, 127, 4, stopSoft, false);
	driveTimeout = nPgmTime + 3000;
	mobileSet(mobileBottom, -1);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "rl20 1-4");
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, "rl20 1-5");

	// 2
	turnToTargetStupidAsync(45, 25, ch, PI);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(turnToTargetStupidState, driveTimeout, "rl20 2-1");
	moveToTargetSimpleAsync(45, 25, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftLowerSimple, coneTimeout, "rl20 2-2");
	configure(armConfig, ARM_PRESTACK - 100, -127, 20);
	armSet(armLowerSimple, &armConfig);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "rl20 2-3");
	turnToAngleCustomAsync(-0.75 * PI, ccw, 70, 0.3);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToAngleCustomState, driveTimeout, "rl20 2-4");
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 10.5, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "rl20 2-5");
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gVelocity.y, -0.05, driveTimeout, "rl20 2-6");
	setDrive(7, 7);
	configure(liftConfig, LIFT_MOBILE_THRESHOLD + 10, 127, 0);
	liftSet(liftRaiseSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	liftTimeoutWhile(liftRaiseSimple, coneTimeout, "rl20 2-7");
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, "rl20 2-8");
	sleep(300);
	moveToTargetDisSimpleAsync(PI / 4, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	setMobile(-7);
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "rl20 2-9");
	mobileSet(mobileManaged, 0);
}

void auto20RedRight()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
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
	moveToTargetDisSimpleAsync(PI / 4, 6, gPosition.y, gPosition.x, 60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "rr20 1-1");
	turnToTargetCustomAsync(14, 107, cw, 0, 40, 0.2);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "rr20 1-2");
	moveToTargetSimpleAsync(14, 107, gPosition.y, gPosition.x, 70, 4, stopSoft, false);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftResetEncoder, coneTimeout, "rr20 1-3");
	configure(liftConfig, gLiftRaiseTarget[0], 127, -15);
	liftSet(liftRaiseSimple, &liftConfig);
	mobileSet(mobileBottom, -1);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "rr20 1-4");
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, "rr20 1-5");

	// 2
	turnToTargetStupidAsync(25, 45, ch, PI);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(turnToTargetStupidState, driveTimeout, "rr20 2-1");
	moveToTargetSimpleAsync(25, 45, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftLowerSimple, coneTimeout, "rr20 2-2");
	configure(armConfig, ARM_PRESTACK - 100, -127, 20);
	armSet(armLowerSimple, &armConfig);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "rr20 2-3");
	turnToAngleCustomAsync(-0.75 * PI, cw, 70, 0.3);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToAngleCustomState, driveTimeout, "rr20 2-4");
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 10.5, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "rr20 2-5");
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gVelocity.y, -0.05, driveTimeout, "rr20 2-6");
	setDrive(7, 7);
	configure(liftConfig, LIFT_MOBILE_THRESHOLD + 10, 127, 0);
	liftSet(liftRaiseSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	liftTimeoutWhile(liftRaiseSimple, coneTimeout, "rr20 2-7");
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, "rr20 2-8");
	sleep(300);
	moveToTargetDisSimpleAsync(PI / 4, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	setMobile(-7);
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "rr20 2-9");
	mobileSet(mobileManaged, 0);
}

void auto20BlueRight()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
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
	moveToTargetDisSimpleAsync(PI / 4, 10, gPosition.y, gPosition.x, 60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "br20 1-1");
	turnToTargetCustomAsync(14, 107, cw, 0, 40, 0.2);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "br20 1-2");
	moveToTargetSimpleAsync(14, 107, gPosition.y, gPosition.x, 70, 4, stopSoft, false);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftResetEncoder, coneTimeout, "br20 1-3");
	configure(liftConfig, gLiftRaiseTarget[0], 127, -15);
	liftSet(liftRaiseSimple, &liftConfig);
	mobileSet(mobileBottom, -1);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "br20 1-4");
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, "br20 1-5");

	// 2
	turnToTargetNewAsync(25, 45, ch, PI);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, "br20 2-1");
	moveToTargetSimpleAsync(25, 45, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftLowerSimple, coneTimeout, "br20 2-2");
	configure(armConfig, ARM_PRESTACK - 100, -127, 20);
	armSet(armLowerSimple, &armConfig);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "br20 2-3");
	turnToAngleCustomAsync(-0.75 * PI, cw, 70, 0.3);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToAngleCustomState, driveTimeout, "br20 2-4");
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 10.5, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "br20 2-5");
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gVelocity.y, -0.05, driveTimeout, "br20 2-6");
	setDrive(7, 7);
	configure(liftConfig, LIFT_MOBILE_THRESHOLD + 10, 127, 0);
	liftSet(liftRaiseSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	liftTimeoutWhile(liftRaiseSimple, coneTimeout, "br20 2-7");
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, "br20 2-8");
	sleep(300);
	moveToTargetDisSimpleAsync(PI / 4, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	setMobile(-7);
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "br20 2-9");
	mobileSet(mobileManaged, 0);
}

void auto20BlueLeft()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
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
	moveToTargetDisSimpleAsync(PI / 4, 6, gPosition.y, gPosition.x, 60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "bl20 1-1");
	turnToTargetCustomAsync(107, 14, ccw, 0, 40, 0.2);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "bl20 1-2");
	moveToTargetSimpleAsync(107, 14, gPosition.y, gPosition.x, 70, 4, stopSoft, false);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftResetEncoder, coneTimeout, "bl20 1-3");
	configure(liftConfig, gLiftRaiseTarget[0], 127, -15);
	liftSet(liftRaiseSimple, &liftConfig);
	mobileSet(mobileBottom, -1);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "bl20 1-4");
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, "bl20 1-5");

	// 2
	turnToTargetStupidAsync(45, 25, ch, PI);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(turnToTargetStupidState, driveTimeout, "rl20 2-1");
	moveToTargetSimpleAsync(45, 25, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftLowerSimple, coneTimeout, "rl20 2-2");
	configure(armConfig, ARM_PRESTACK - 100, -127, 20);
	armSet(armLowerSimple, &armConfig);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "rl20 2-3");
	turnToAngleCustomAsync(-0.75 * PI, ccw, 70, 0.3);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToAngleCustomState, driveTimeout, "rl20 2-4");
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 10.5, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "rl20 2-5");
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gVelocity.y, -0.05, driveTimeout, "rl20 2-6");
	setDrive(7, 7);
	configure(liftConfig, LIFT_MOBILE_THRESHOLD + 10, 127, 0);
	liftSet(liftRaiseSimple, &liftConfig);
	coneTimeout = nPgmTime + 1500;
	liftTimeoutWhile(liftRaiseSimple, coneTimeout, "rl20 2-7");
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, "rl20 2-8");
	sleep(300);
	moveToTargetDisSimpleAsync(PI / 4, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	setMobile(-7);
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "rl20 2-9");
	mobileSet(mobileManaged, 0);
}

void stationaryLeftCore(bool goToWall = true)
{
	unsigned long driveAsync;
	unsigned long coneAsync;
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
	moveToTargetSimpleAsync(47, 23, gPosition.y, gPosition.x, 70, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "ls 1-1");
	turnToTargetCustomAsync(47, 47, cw, 0, 60, 0.3);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "ls 1-2");
	setDrive(40, 40);
	driveTimeout = nPgmTime + 2000;
	liftTimeoutWhile(liftResetEncoder, coneTimeout, "ls 1-3");
	configure(liftConfig, 83, 127, -15);
	liftSet(liftRaiseSimple, &liftConfig);
	configure(armConfig, 2600, -127, 25);
	armSet(armLowerSimple, &armConfig);
	sleep(driveTimeout - 1500 - nPgmTime);
	timeoutWhileGreaterThanF(&gVelocity.x, 0.1, driveTimeout, "ls 1-4");
	setDrive(15, 15);
	configure(liftConfig, 47, -127, 25);
	liftSet(liftLowerSimple, &liftConfig);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[liftEnc].value, 47, coneTimeout, "ls 1-5");
	configure(armConfig, ARM_HORIZONTAL, -127, 25);
	armSet(armLowerSimple, &armConfig);
	coneTimeout = nPgmTime + 1500;
	timeoutWhileGreaterThanL(&gSensor[armPoti].value, ARM_HORIZONTAL + 200, coneTimeout, "ls 1-6");

	// 2
	configure(liftConfig, 82, 127, -15);
	liftSet(liftRaiseSimple, &liftConfig);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[liftEnc].value, 75, coneTimeout, "ls 2-1");
	configure(armConfig, ARM_TOP, 127, -15);
	armSet(armRaiseSimple, &armConfig);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[armPoti].value, ARM_TOP - 200, coneTimeout, "ls 2-2");
	if (goToWall)
	{
		turnToTargetCustomAsync(52, 18, cw, PI, 45, 0.05);
		driveTimeout = nPgmTime + 3000;
		autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "ls 2-3");
		moveToTargetSimpleAsync(52, 18, gPosition.y, gPosition.x, -70, 0, stopSoft | stopHarsh, true);
		driveTimeout = nPgmTime + 2000;
		coneTimeout = nPgmTime + 1500;
		timeoutWhileLessThanF(&gPosition.y, 50, coneTimeout, "ls 2-4");
		configure(liftConfig, LIFT_BOTTOM + 2, -127, 0);
		liftSet(liftLowerSimple, &liftConfig);
		coneTimeout = nPgmTime + 2000;
		autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "ls 2-5");
		turnToAngleStupidAsync(PI / 2, ch);
		driveTimeout = nPgmTime + 3000;
		autoSimpleTimeoutWhile(turnToAngleStupidState, driveTimeout, "ls 2-6");
		resetBlueLeftAsync();
		driveTimeout = nPgmTime + 3000;
		autoSimpleTimeoutWhile(resetBlueLeftState, driveTimeout, "ls 2-7");
	}
}

void stationaryRightCore(bool goToWall = true)
{
	unsigned long driveAsync;
	unsigned long coneAsync;
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
	moveToTargetSimpleAsync(23, 47, gPosition.y, gPosition.x, 70, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "rs 1-1");
	turnToTargetCustomAsync(47, 47, ccw, 0, 60, 0.3);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "rs 1-2");
	setDrive(40, 40);
	driveTimeout = nPgmTime + 2000;
	liftTimeoutWhile(liftResetEncoder, coneTimeout, "rs 1-3");
	configure(liftConfig, 83, 127, -15);
	liftSet(liftRaiseSimple, &liftConfig);
	configure(armConfig, 2600, -127, 25);
	armSet(armLowerSimple, &armConfig);
	sleep(driveTimeout - 1500 - nPgmTime);
	timeoutWhileGreaterThanF(&gVelocity.y, 0.1, driveTimeout, "rs 1-4");
	setDrive(15, 15);
	configure(liftConfig, 52, -127, 25);
	liftSet(liftLowerSimple, &liftConfig);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[liftEnc].value, 52, coneTimeout, "rs 1-5");
	configure(armConfig, ARM_HORIZONTAL, -127, 25);
	armSet(armLowerSimple, &armConfig);
	coneTimeout = nPgmTime + 1500;
	timeoutWhileGreaterThanL(&gSensor[armPoti].value, ARM_HORIZONTAL + 200, coneTimeout, "rs 1-6");

	// 2
	configure(liftConfig, 82, 127, -15);
	liftSet(liftRaiseSimple, &liftConfig);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[liftEnc].value, 75, coneTimeout, "rs 1-7");
	configure(armConfig, ARM_TOP, 127, -15);
	armSet(armRaiseSimple, &armConfig);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[armPoti].value, ARM_TOP - 200, coneTimeout, "rs 1-8");
	if (goToWall)
	{
		turnToTargetCustomAsync(18, 52, ccw, PI, 45, 0.05);
		driveTimeout = nPgmTime + 3000;
		autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "rs 2-3");
		moveToTargetSimpleAsync(18, 52, gPosition.y, gPosition.x, -70, 0, stopSoft | stopHarsh, true);
		driveTimeout = nPgmTime + 2000;
		coneTimeout = nPgmTime + 1500;
		timeoutWhileLessThanF(&gPosition.x, 50, coneTimeout, "rs 2-4");
		configure(liftConfig, LIFT_BOTTOM + 2, -127, 0);
		liftSet(liftLowerSimple, &liftConfig);
		coneTimeout = nPgmTime + 2000;
		autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "rs 2-5");
		turnToAngleStupidAsync(0, ch);
		driveTimeout = nPgmTime + 3000;
		autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "rs 2-6");
		resetBlueRightAsync();
		driveTimeout = nPgmTime + 3000;
		autoSimpleTimeoutWhile(resetBlueRightState, driveTimeout, "rs 2-7");
	}
}

void autoStationaryLeftBlock()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	sSimpleConfig liftConfig;
	sSimpleConfig armConfig;
	float _x;
	float _y;

	stationaryLeftCore(false);

	// 2
	turnToTargetCustomAsync(71, 23, ch, PI, 70, 0.2);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "ls+b 2-1");
	moveToTargetSimpleAsync(71, 23, gPosition.y, gPosition.x, -70, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gPosition.y, 50, driveTimeout, "ls+b 2-2");
	configure(liftConfig, LIFT_BOTTOM + 2, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "ls+b 2-3");
	turnToTargetCustomAsync(119, 71, ch, PI, 60, 0.3);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "ls+b 2-4");
	moveToTargetSimpleAsync(119, 71, gPosition.y, gPosition.x, -127, 0, stopSoft, false);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "ls+b 2-5");
}

void autoStationaryRightBlock()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	sSimpleConfig liftConfig;
	sSimpleConfig armConfig;
	float _x;
	float _y;

	stationaryRightCore(false);

	// 2
	turnToTargetCustomAsync(23, 71, ch, PI, 70, 0.2);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "rs+b 2-1");
	moveToTargetSimpleAsync(23, 71, gPosition.y, gPosition.x, -70, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gPosition.x, 50, driveTimeout, "rs+b 2-2");
	configure(liftConfig, LIFT_BOTTOM + 2, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "rs+b 2-3");
	turnToTargetCustomAsync(71, 119, ch, PI, 60, 0.3);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "rs+b 2-4");
	moveToTargetSimpleAsync(71, 119, gPosition.y, gPosition.x, -127, 0, stopSoft, false);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "rs+b 2-5");
}

void autoStationaryLeft5()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	sSimpleConfig liftConfig;
	sSimpleConfig armConfig;
	float _x;
	float _y;

	stationaryLeftCore();

	// 3
	moveToTargetDisSimpleAsync(PI / 2, 19, gPosition.y, 0, 50, 0, stopHarsh, false);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "ls+5 3-1");
	turnToTargetCustomAsync(107, 14, ccw, 0, 45, 0.15);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_MOBILE_THRESHOLD, 127, -10);
	liftSet(liftRaiseSimple, &liftConfig);
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "ls+5 3-2");
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 1000, coneTimeout, "ls+5 3-3");
	moveToTargetSimpleAsync(107, 14, gPosition.y, gPosition.x, 127, 6, stopSoft, true);
	driveTimeout = nPgmTime + 3000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, "ls+5 3-4");
	configure(liftConfig, LIFT_BOTTOM + 2, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "ls+5 3-5");
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, "ls+5 3-6");

	// 4
	turnToTargetCustomAsync(62, 18, ch, PI, 45, 0.05);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM + 2, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "ls+5 4-1");
	moveToTargetDisSimpleAsync(gPosition.a, -36, gPosition.y, gPosition.x, -127, 0, stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "ls+5 4-2");
	turnToAngleCustomAsync(PI / -2, ccw, 127, 0.2);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_MOBILE_THRESHOLD, 127, -10);
	liftSet(liftRaiseSimple, &liftConfig);
	autoSimpleTimeoutWhile(turnToAngleCustomState, driveTimeout, "ls+5 4-3");
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, "ls+5 4-4");
	moveToTargetDisSimpleAsync(gPosition.a, -8, gPosition.y, gPosition.x, -60, 0, stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "ls+5 4-5");
}

void autoStationaryRight5()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	sSimpleConfig liftConfig;
	sSimpleConfig armConfig;
	float _x;
	float _y;

	stationaryRightCore();

	// 3
	moveToTargetDisSimpleAsync(0, 19, 0, gPosition.x, 50, 0, stopHarsh, false);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "rs+5 3-1");
	turnToTargetNewAsync(14, 107, cw, 0);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_MOBILE_THRESHOLD, 127, -10);
	liftSet(liftRaiseSimple, &liftConfig);
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, "rs+5 3-2");
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 1000, coneTimeout, "rs+5 3-3");
	moveToTargetSimpleAsync(14, 107, gPosition.y, gPosition.x, 127, 6, stopSoft, true);
	driveTimeout = nPgmTime + 3000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, "rs+5 3-4");
	configure(liftConfig, LIFT_BOTTOM + 2, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "rs+5 3-5");
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, "rs+5 3-5");

	// 4
	turnToTargetCustomAsync(18, 62, ch, PI, 45, 0.05);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_BOTTOM + 2, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "rs+5 4-1");
	moveToTargetDisSimpleAsync(gPosition.a, -36, gPosition.y, gPosition.x, -127, 0, stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "rs+5 4-2");
	turnToAngleCustomAsync(-PI, cw, 127, 0.2);
	driveTimeout = nPgmTime + 3000;
	configure(liftConfig, LIFT_MOBILE_THRESHOLD, 127, -10);
	liftSet(liftRaiseSimple, &liftConfig);
	autoSimpleTimeoutWhile(turnToAngleCustomState, driveTimeout, "rs+5 4-3");
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, "rs+5 4-4");
	moveToTargetDisSimpleAsync(gPosition.a, -8, gPosition.y, gPosition.x, -60, 0, stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "rs+5 4-5");
}

void autoStationaryLeft2()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	sSimpleConfig liftConfig;
	sSimpleConfig armConfig;
	float _x;
	float _y;

	stationaryLeftCore();

	// 3
	moveToTargetDisSimpleAsync(PI / 2, 14, 0, gPosition.x, 50, 0, stopHarsh, false);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "ls+2 3-1");
	turnToTargetCustomAsync(71, 23, ccw, 0, 45, 0.15);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "ls+2 3-2");
	moveToTargetSimpleAsync(71, 23, gPosition.y, gPosition.x, 40, 11, stopHarsh, false);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "ls+2 3-3");
	configure(armConfig, ARM_BOTTOM + 250, -127, 0);
	armSet(armLowerSimple, &armConfig);
	coneTimeout = nPgmTime + 2000;
	armTimeoutWhile(armLowerSimple, coneTimeout, "ls+2 3-4");

	// 4
	turnToTargetCustomAsync(47, 47, cw, 0, 60, 0.3);
	driveTimeout = nPgmTime + 2000;
	configure(liftConfig, 90, 127, -15);
	liftSet(liftRaiseSimple, &liftConfig);
	configure(armConfig, 2400, 127, -25);
	armSet(armRaiseSimple, &armConfig);
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "ls+2 4-1");
	setDrive(40, 40);
	driveTimeout = nPgmTime + 3000;
	sleep(500);
	timeoutWhileGreaterThanF(&gVelocity.x, 0.1, driveTimeout, "ls+2 4-2");
	setDrive(15, 15);
	sleep(500);
	configure(liftConfig, 55, -127, 25);
	liftSet(liftLowerSimple, &liftConfig);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[liftEnc].value, 55, coneTimeout, "ls+2 4-3");
	configure(armConfig, ARM_HORIZONTAL, -127, 25);
	armSet(armLowerSimple, &armConfig);
	coneTimeout = nPgmTime + 1500;
	timeoutWhileGreaterThanL(&gSensor[armPoti].value, ARM_HORIZONTAL + 200, coneTimeout, "ls+2 4-4");

	// 5
	configure(liftConfig, 95, 127, -15);
	liftSet(liftRaiseSimple, &liftConfig);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[liftEnc].value, 85, coneTimeout, "ls+2 5-1");
	configure(armConfig, ARM_TOP, 127, -15);
	armSet(armRaiseSimple, &armConfig);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[armPoti].value, ARM_TOP - 200, coneTimeout, "ls+2 5-2");
	moveToTargetDisSimpleAsync(gPosition.a, -18, gPosition.y, gPosition.x, -40, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "ls+2 5-3");
	configure(liftConfig, LIFT_BOTTOM + 2, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	coneTimeout = nPgmTime + 1200;
	liftTimeoutWhile(liftLowerSimple, coneTimeout, "ls+2 5-4");
}

void autoStationaryRight2()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	sSimpleConfig liftConfig;
	sSimpleConfig armConfig;
	float _x;
	float _y;

	stationaryRightCore();

	// 3
	moveToTargetDisSimpleAsync(0, 14, 0, gPosition.x, 50, 0, stopHarsh, false);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "rs+2 3-1");
	turnToTargetNewAsync(23, 71, cw, 0);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, "rs+2 3-2");
	moveToTargetSimpleAsync(23, 71, gPosition.y, gPosition.x, 40, 11, stopHarsh, false);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, "rs+2 3-3");
	configure(armConfig, ARM_BOTTOM + 250, -127, 0);
	armSet(armLowerSimple, &armConfig);
	coneTimeout = nPgmTime + 2000;
	armTimeoutWhile(armLowerSimple, coneTimeout, "rs+2 3-4");

	// 4
	turnToTargetCustomAsync(47, 47, ccw, 0, 60, 0.3);
	driveTimeout = nPgmTime + 2000;
	configure(liftConfig, 90, 127, -15);
	liftSet(liftRaiseSimple, &liftConfig);
	configure(armConfig, 2400, 127, -25);
	armSet(armRaiseSimple, &armConfig);
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, "rs+2 4-1");
	setDrive(40, 40);
	driveTimeout = nPgmTime + 3000;
	sleep(500);
	timeoutWhileGreaterThanF(&gVelocity.y, 0.1, driveTimeout, "rs+2 4-2");
	setDrive(15, 15);
	sleep(500);
	configure(liftConfig, 55, -127, 25);
	liftSet(liftLowerSimple, &liftConfig);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[liftEnc].value, 55, coneTimeout, "rs+2 4-3");
	configure(armConfig, ARM_HORIZONTAL, -127, 25);
	armSet(armLowerSimple, &armConfig);
	coneTimeout = nPgmTime + 1500;
	timeoutWhileGreaterThanL(&gSensor[armPoti].value, ARM_HORIZONTAL + 200, coneTimeout, "rs+2 4-4");

	// 5
	configure(liftConfig, 95, 127, -15);
	liftSet(liftRaiseSimple, &liftConfig);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[liftEnc].value, 85, coneTimeout, "rs+2 5-1");
	configure(armConfig, ARM_TOP, 127, -15);
	armSet(armRaiseSimple, &armConfig);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[armPoti].value, ARM_TOP - 200, coneTimeout, "rs+2 5-2");
	moveToTargetDisSimpleAsync(gPosition.a, -18, gPosition.y, gPosition.x, -40, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, "rs+2 5-3");
	configure(liftConfig, LIFT_BOTTOM + 2, -127, 0);
	liftSet(liftLowerSimple, &liftConfig);
	coneTimeout = nPgmTime + 1200;
	liftTimeoutWhile(liftLowerSimple, coneTimeout, "rs+2 5-4");
}

void autoTest()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	sSimpleConfig liftConfig;
	sSimpleConfig armConfig;
	float _x;
	float _y;

	gMobileCheckLift = true;
	resetPositionFull(gPosition, 0, 0, 0);
	resetVelocity(gVelocity, gPosition);

	liftSet(liftResetEncoder);
	coneTimeout = nPgmTime + 1400;

	liftTimeoutWhile(liftResetEncoder, coneTimeout, "test 1");
	liftSet(liftManaged);
	setLift(10);
	setDrive(40, 40);
	driveTimeout = nPgmTime + 2000;
	sleep(500);
	timeoutWhileGreaterThanF(&gVelocity.y, 0.1, driveTimeout, "test 2");
	setDrive(7, 7);
	sleep(5000);
}
