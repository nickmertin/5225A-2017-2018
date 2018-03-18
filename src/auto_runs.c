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
	//autoTest();
	//return;
	selectAuto();
	writeDebugStreamLine("Selected auto: %s %d", gAlliance == allianceBlue ? "blue" : "red", gCurAuto);
	if (gAlliance == allianceBlue)
	{
		switch (gCurAuto)
		{
			case 0: autoSkills(); break;
			case 1: auto20Right(); break;
			case 2: auto20Left(); break;
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
			case 1: auto20Left(); break;
			case 2: auto20Right(); break;
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

#define START_BAR_INTERCEPT 49.7
#define START_BAR_ROBOT_OFFSET 7.5
#define START_BAR_RESET_INTERCEPT (START_BAR_INTERCEPT + START_BAR_ROBOT_OFFSET / sqrt(2))

float calcYCoord(float u, float _a)
{
	const float x = 7.5;
	const float y = 3.8;
	const float b = atan2(y, x);
	const float d2 = x * x + y * y;
	const float d = sqrt(d2);
	const float a = PI / 4.0;

	float u2 = u * u;

	float q2 = u2 + d2 - 2 * u * d * cos(a + PI / 2.0 + b);
	float q = sqrt(q2);

	float c = acos((q2 + d2 - u2) / (2.0 * q * d));
	float g = PI - _a + (PI / 2.0 - b - c);

	return q * cos(g);
}

float calcXCoord(float u, float _a)
{
	const float x = 7.5;
	const float y = 3.8;
	const float b = atan2(y, x);
	const float d2 = x * x + y * y;
	const float d = sqrt(d2);
	const float a = PI / 4.0;

	float u2 = u * u;

	float q2 = u2 + d2 - 2 * u * d * cos(a + PI / 2.0 + b);
	float q = sqrt(q2);

	float c = acos((q2 + d2 - u2) / (2.0 * q * d));
	float g = _a + (PI / 2.0 + b + c);

	return q * cos(g);
}

bool resetSonarFull(unsigned long time, unsigned long maxTime, float a, long minL, long maxL, long minR, long maxR, bool crossField)
{
	unsigned long st = nPgmTime;
	time += st;
	maxTime += st;

	playSound(soundShortBlip);

	float lu = 0;
	float ru = 0;
	int lc = 0;
	int rc = 0;
	while (nPgmTime < time || lc < 10 || rc < 10)
	{
		if (nPgmTime > maxTime)
			return false;

		long l = gSensor[sonarL].value;
		if (l >= minL && l <= maxL)
		{
			lu += l;
			++lc;
		}

		long r = gSensor[sonarR].value;
		if (r >= minR && r <= maxR)
		{
			ru += r;
			++rc;
		}

		writeDebugStreamLine("%d %d", l, r);

		sleep(10);
	}

	playSound(soundShortBlip);

	lu *= 0.0393701 / (float) lc;
	ru *= 0.0393701 / (float) rc;

	writeDebugStreamLine("Ultrasonic %f %f", lu, ru);

	if (crossField)
		a += PI;

	float y = calcYCoord(lu, a);
	float x = calcXCoord(ru, a);
	if (crossField)
	{
		y = 140.5 - y;
		x = 140.5 - x;
		a -= PI;
	}
	resetPositionFull(gPosition, y, x, a);
	resetVelocity(gVelocity, gPosition);
	return true;
}

bool resetSonarYOnly(unsigned long time, unsigned long maxTime, float xInt, float a, long min, long max, bool crossField)
{
	writeDebugStreamLine("%d Start reset Y", nPgmTime);
	unsigned long st = nPgmTime;
	time += st;
	maxTime += st;

	playSound(soundShortBlip);

	float u = 0;
	int c = 0;
	while (nPgmTime < time || c < 10)
	{
		if (nPgmTime > maxTime)
		{
			writeDebugStreamLine("%d MAX TIME %d", nPgmTime, maxTime);
			return false;
		}

		long l = gSensor[sonarL].value;
		if (l >= min && l <= max)
		{
			u += l;
			++c;
		}

		writeDebugStreamLine("%d", l);

		sleep(10);
	}

	playSound(soundShortBlip);

	u *= 0.0393701 / (float) c;

	writeDebugStreamLine("Ultrasonic (L) %f", u);

	if (crossField)
		a += PI;

	float y = calcYCoord(u, a);
	float x = -y + xInt;
	if (crossField)
	{
		y = 140.5 - y;
		x = 140.5 - x;
		a -= PI;
	}
	resetPositionFull(gPosition, y, x, a);
	resetVelocity(gVelocity, gPosition);
	return true;
}

bool resetSonarXOnly(unsigned long time, unsigned long maxTime, float yInt, float a, long min, long max, bool crossField)
{
	writeDebugStreamLine("%d Start reset X", nPgmTime);
	unsigned long st = nPgmTime;
	time += st;
	maxTime += st;

	playSound(soundShortBlip);

	float u = 0;
	int c = 0;
	while (nPgmTime < time || c < 10)
	{
		if (nPgmTime > maxTime)
		{
			writeDebugStreamLine("%d MAX TIME %d", nPgmTime, maxTime);
			return false;
		}

		long r = gSensor[sonarR].value;
		if (r >= min && r <= max)
		{
			u += r;
			++c;
		}

		writeDebugStreamLine("%d", r);

		sleep(10);
	}

	playSound(soundShortBlip);

	u *= 0.0393701 / (float) c;

	writeDebugStreamLine("Ultrasonic (R) %f", u);

	if (crossField)
		a += PI;

	float x = calcXCoord(u, a);
	float y = -x + yInt;
	if (crossField)
	{
		y = 140.5 - y;
		x = 140.5 - x;
		a -= PI;
	}
	resetPositionFull(gPosition, y, x, a);
	resetVelocity(gVelocity, gPosition);
	return true;
}

void driveAgainstStartingBar(word left, word right, word leftSlow, word rightSlow, unsigned long timeout)
{
	if (timeout)
		timeout += nPgmTime;
	byte val = 0;
	while (val != 3 && (!timeout || nPgmTime < timeout))
	{
		writeDebugStreamLine("%d | %d %d | %d %d", nPgmTime, gSensor[lsBarL].value, SensorValue[lsBarL], gSensor[lsBarR].value, SensorValue[lsBarR]);
		if (!(val & 1) && gSensor[lsBarL].value)
		{
			writeDebugStreamLine("%d Saw left", nPgmTime);
			left = leftSlow;
			val |= 1;
		}
		if (!(val & 2) && gSensor[lsBarR].value)
		{
			writeDebugStreamLine("%d Saw right", nPgmTime);
			right = rightSlow;
			val |= 2;
		}
		setDrive(left, right);
		sleep(10);
	}
}

void killAuto(unsigned long timeout)
{
	sleep(timeout - nPgmTime);
	if (competitionState == autonomousState)
	{
		writeDebugStreamLine("KILL AUTO");
		competitionSet(competitionNotRunning);
		autoSimpleSet(autoSimpleNotRunning);
		stackSet(stackNotRunning);
		setDrive(0, 0);
	}
}

NEW_ASYNC_VOID_1(killAuto, unsigned long);

#define DRIVE_AWAIT(routine, major, minor) autoSimpleTimeoutUntil(autoSimpleNotRunning, driveTimeout, TID2(routine, major, minor))

void autoSkills(int segment)
{
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	float _x;
	float _y;

	gMobileCheckLift = true;

#ifdef SKILLS_RESET_AT_START
	trackPositionTaskKill();
	resetPositionFull(gPosition, 47, 14.25, 0);
	resetVelocity(gVelocity, gPosition);
	trackPositionTaskAsync();
#endif

	//segment = 3;

	switch (segment)
	{
	case 1:
		goto skip1;
	case 2:
		goto skip2;
	case 3:
		goto skip3;
	case 4:
		goto skip4;
	}

	//killAutoAsync(gAutoTime + 60000);

	// 1
	moveToTargetSimpleAsync(107, 14.25, 47, 15, 70, 0.5, 6, 55, 14, stopNone, true);
	driveTimeout = nPgmTime + 2000;
	mobileSet(mobileBottom, mfClear);
	DRIVE_AWAIT(skills, 1, 1);
	setDrive(55, 55);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 1, 2), false);
	setDrive(0, 0);
	mobileSet(mobileTop, mfClear);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 1, 3));

	// 2
	moveToTargetSimpleAsync(71, 15, gPosition.y, gPosition.x, -127, 0.5, 4, -90, 0, stopNone, true);
	driveTimeout = nPgmTime + 2000;
	stackSet(stackStack, sfClear);
	DRIVE_AWAIT(skills, 2, 1);
	moveToTargetSimpleAsync(33, 33, gPosition.y, gPosition.x, -127, 1, 0, 0, 0, stopSoft | stopHarsh, false);
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 2, 2);
	turnToAngleNewAlgAsync(-0.75 * PI, ccw, 0.35, 26, 12, true);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 2, 3);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 8, _y, _x, 60, 0, 0, 0, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 2, 4);
	setDrive(50, 50);
	driveTimeout = nPgmTime + 1500;
	sleep(400);
	timeoutWhileLessThanF(VEL_NONE, 0, &gVelocity.y, -0.05, driveTimeout, TID2(skills, 2, 5), true, false);
	setDrive(15, 15);
	sleep(500);
	if (!resetSonarFull(100, 500, -0.75 * PI, 230, 470, 180, 650, false))
		resetPositionFull(gPosition, gPosition.y, gPosition.x, -0.75 * PI);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	mobileSet(mobileDownToMiddle, mfClear);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(skills, 2, 6));
	sleep(300);

	// 3
	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, -7, _y, _x, -60, 0, 0, 0, 0, stopHarsh, false);
	driveTimeout = nPgmTime + 1500;
	liftLowerSimpleAsync(LIFT_BOTTOM + 200, -127, 0);
	DRIVE_AWAIT(skills, 3, 1);
	turnToAngleNewAlgAsync(-0.25 * PI, cw, 0.27, 23, 12, false);
	driveTimeout = nPgmTime + 2000;
	mobileSet(mobileBottom, mfNone);
	DRIVE_AWAIT(skills, 3, 2);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, 1, 0);
	sweepTurnToTargetAsync(_y + 24, _x, 35 * PI / 180, 17, cw, 127, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 3, 3);
	moveToTargetSimpleAsync(71, 47, gPosition.y, gPosition.x, 90, 1, 0, 0, 14, stopNone, false);
	driveTimeout = nPgmTime + 1000;
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 80, 0);
	DRIVE_AWAIT(skills, 3, 4);
	setDrive(55, 55);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 3, 5), false);
	mobileSet(mobileTop, mfClear);
	setDrive(0, 0);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP, coneTimeout, TID2(skills, 3, 6));

	// 4
	turnToTargetNewAlgAsync(28, 6, ccw, 0.45, 30, 11, true, 0);
	driveTimeout = nPgmTime + 2200;
	DRIVE_AWAIT(skills, 4, 1);
	moveToTargetSimpleAsync(28, 6, gPosition.y, gPosition.x, 75, 1.5, 12, 30, 13, stopNone, true);
	driveTimeout = nPgmTime + 2000;
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 80, 0);
	timeoutWhileGreaterThanF(VEL_LOCAL_Y, 1.0, &gPosition.x, 22, driveTimeout, TID2(skils, 4, 2), true, false);
	mobileSet(mobileBottom, mfClear);
	coneTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 4, 3);
	driveAgainstStartingBar(25, 25, 20, 20, 2000);
	timeoutWhileGreaterThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 4, 4), false);
	sleep(500);
	if (segment > -1)
		return;
skip2:
	if (!resetSonarYOnly(100, 500, START_BAR_RESET_INTERCEPT, -0.75 * PI, 600, 900, false) && !resetSonarXOnly(100, 500, START_BAR_RESET_INTERCEPT, -0.75 * PI, 130, 330, false))
	{
		if (segment > -1)
			return;
		resetPositionFull(gPosition, gPosition.y, gPosition.x, -0.75 * PI);
	}

	// 5
	moveToTargetDisSimpleAsync(gPosition.a, -8, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, stopNone, false);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 5, 1);
	turnToTargetNewAlgAsync(60, 36, cw, 0.4, 20, 30, false, 0);
	driveTimeout = nPgmTime + 1500;
	liftLowerSimpleAsync(LIFT_BOTTOM, -70, 0);
	DRIVE_AWAIT(skills, 5, 2);
	moveToTargetSimpleAsync(60, 36, gPosition.y, gPosition.x, 127, 0.5, 0, 0, 5, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 5, 3);
	moveToTargetSimpleAsync(95, 71, gPosition.y, gPosition.x, 127, 1, 6, 55, 14, stopNone, true);
	driveTimeout = nPgmTime + 2500;
	DRIVE_AWAIT(skills, 5, 4);
	setDrive(55, 55);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 5, 6), false);
	mobileSet(mobileUpToMiddle, mfClear);
	coneTimeout = nPgmTime + 1000;

	// 6
	moveToTargetSimpleAsync(135, 113, gPosition.y, gPosition.x, 127, 1, 12, 30, 13, stopNone, true);
	driveTimeout = nPgmTime + 2000;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(skills, 6, 1));
	DRIVE_AWAIT(skills, 6, 2);
	mobileSet(mobileBottom, mfClear);
	coneTimeout = nPgmTime + 1000;
	driveAgainstStartingBar(25, 25, 20, 20, 1500);
	timeoutWhileGreaterThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 6, 3), false);
	sleep(500);
	if (segment > -1)
		return;
skip3:
	mobileSet(mobileBottom, mfNone);
	setDrive(15, 15);
	if (!resetSonarXOnly(100, 500, START_BAR_RESET_INTERCEPT, 0.25 * PI, 560, 830, true) && !resetSonarYOnly(100, 500, START_BAR_RESET_INTERCEPT, 0.25 * PI, 130, 330, true))
	{
		if (segment > -1)
			return;
		resetPositionFull(gPosition, gPosition.y, gPosition.x, 0.25 * PI);
	}

	// 7
	moveToTargetDisSimpleAsync(gPosition.a, -8, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, stopNone, false);
	driveTimeout = nPgmTime + 700;
	DRIVE_AWAIT(skills, 7, 1);
	turnToTargetNewAlgAsync(127.5, 71, ccw, 0.2, 23, 12, false, 0);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 7, 2);
	moveToTargetSimpleAsync(127.5, 71, gPosition.y, gPosition.x, 70, 0.5, 0, 0, 15, stopNone, false);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 7, 3);
	moveToTargetSimpleAsync(128, 35, gPosition.y, gPosition.x, 127, 0.5, 8, 55, 14, stopNone, true);
	driveTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanF(VEL_NONE, 0, &gPosition.x, 58, driveTimeout, TID2(skills, 7, 4));
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, 0);
	DRIVE_AWAIT(skills, 7, 5);
	setDrive(55, 55);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 7, 6), false);
	mobileSet(mobileTop, mfClear);
	setDrive(0, 0);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP, coneTimeout, TID2(skills, 7, 7));
}

void autoBlock()
{
	setDrive(-127, -127);
	sleep(2000);
	setDrive(7, 7);
}

//void auto20Left()
//{
//	unsigned long driveTimeout;
//	unsigned long coneTimeout;
//	float _x;
//	float _y;

//	gMobileCheckLift = true;

//	trackPositionTaskKill();
//	resetPositionFull(gPosition, 40, 16, 45);
//	resetVelocity(gVelocity, gPosition);
//	trackPositionTaskAsync();

//	coneTimeout = nPgmTime + 1400;

//	// 1
//	moveToTargetDisSimpleAsync(gPosition.a, 8, gPosition.y, gPosition.x, 70, 0, 0, 0, 0, stopHarsh, true);
//	driveTimeout = nPgmTime + 2000;
//	mobileSet(mobileBottom, mfClear);
//	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(br20, 1, 1));
//	turnToTargetSimpleAsync(107, 11, ccw, 60, 60, false, 0);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(br20, 1, 2));
//	moveToTargetSimpleAsync(107, 11, gPosition.y, gPosition.x, 127, 0, 0, 0, 12, stopNone, true);
//	driveTimeout = nPgmTime + 2500;
//	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
//	timeoutWhileLessThanF(VEL_LOCAL_Y, 1.0, &gPosition.y, 72, driveTimeout, TID2(br20, 1, 3), true, false);
//	liftRaiseSimpleAsync(gLiftRaiseTarget[0], 80, 0);
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(br20, 1, 3));
//	setDrive(30, 30);
//	driveTimeout = nPgmTime + 1000;
//	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(br20, 1, 4));
//	setDrive(0, 0);
//	mobileSet(mobileTop, mfClear);
//	coneTimeout = nPgmTime + 2000;
//	timeoutWhileLessThanL(VEL_SENSOR(mobilePoti), 0.5, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(br20, 1, 5));

//	// 2
//	turnToTargetSimpleAsync(27, 33, ch, 40, 40, true, PI);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(br20, 2, 1));
//	moveToTargetSimpleAsync(27, 33, gPosition.y, gPosition.x, -127, 0, 30, -30, 4, stopHarsh, true);
//	driveTimeout = nPgmTime + 2500;
//	stackSet(stackStack, sfClear);
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(br20, 2, 2));
//	turnToAngleSimpleAsync(-135, ccw, 127, 127, true);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(turnToAngleSimpleState, driveTimeout, TID2(br20, 2, 3));

//	// 3
//	_x = gPosition.x;
//	_y = gPosition.y;
//	normalize(_x, _y, -1, 56);
//	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 9, _y, _x, 60, 0, 0, 0, 0, stopNone, false);
//	driveTimeout = nPgmTime + 1500;
//	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(br20, 3, 1));
//	setDrive(30, 30);
//	driveTimeout = nPgmTime + 1500;
//	timeoutWhileLessThanF(VEL_NONE, 0, &gVelocity.y, -0.05, driveTimeout, TID2(br20, 3, 2), true, false);
//	setDrive(25, 25);
//	sleep(500);
//	//skip:
//	if (!resetSonarFull(100, 500, -0.75 * PI, 230, 470, 180, 650, false))
//		resetPositionFull(gPosition, gPosition.y, gPosition.x, 225);
//	_x = gPosition.x;
//	_y = gPosition.y;
//	normalize(_x, _y, -1, 56);
//	mobileSet(mobileDownToMiddle, mfClear);
//	coneTimeout = nPgmTime + 1500;
//	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(br20, 3, 3));
//	mobileSet(mobileIdle);
//	sleep(300);

//	// 4
//	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, -7, _y, _x, -60, 0, 0, 0, 0, stopHarsh, false);
//	driveTimeout = nPgmTime + 1500;
//	liftLowerSimpleAsync(LIFT_BOTTOM + 200, -127, 0);
//	sleep(300);
//	mobileSet(mobileBottom, mfNone);
//	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(br20, 4, 1));
//}

//void auto20Right()
//{
//	unsigned long driveTimeout;
//	unsigned long coneTimeout;
//	float _x;
//	float _y;

//	gMobileCheckLift = true;

//	trackPositionTaskKill();
//	resetPositionFull(gPosition, 16, 40, 45);
//	resetVelocity(gVelocity, gPosition);
//	trackPositionTaskAsync();

//	coneTimeout = nPgmTime + 1400;

//	// 1
//	moveToTargetDisSimpleAsync(gPosition.a, 8, gPosition.y, gPosition.x, 70, 0, 0, 0, 0, stopHarsh, true);
//	driveTimeout = nPgmTime + 2000;
//	mobileSet(mobileBottom, mfClear);
//	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(br20, 1, 1));
//	turnToTargetSimpleAsync(11, 107, cw, 60, 60, false, 0);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(br20, 1, 2));
//	moveToTargetSimpleAsync(11, 107, gPosition.y, gPosition.x, 127, 0, 0, 0, 12, stopNone, true);
//	driveTimeout = nPgmTime + 2500;
//	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
//	timeoutWhileLessThanF(VEL_LOCAL_Y, 1.0, &gPosition.x, 72, driveTimeout, TID2(br20, 1, 3), true, false);
//	liftRaiseSimpleAsync(gLiftRaiseTarget[0], 80, 0);
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(br20, 1, 3));
//	setDrive(30, 30);
//	driveTimeout = nPgmTime + 1000;
//	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(br20, 1, 4));
//	setDrive(0, 0);
//	mobileSet(mobileTop, mfClear);
//	coneTimeout = nPgmTime + 2000;
//	timeoutWhileLessThanL(VEL_SENSOR(mobilePoti), 0.5, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(br20, 1, 5));

//	// 2
//	turnToTargetSimpleAsync(33, 27, ch, 40, 40, true, PI);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(br20, 2, 1));
//	moveToTargetSimpleAsync(33, 27, gPosition.y, gPosition.x, -127, 0, 30, -30, 4, stopHarsh, true);
//	driveTimeout = nPgmTime + 2500;
//	stackSet(stackStack, sfClear);
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(br20, 2, 2));
//	turnToAngleSimpleAsync(-135, cw, 127, 127, true);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(turnToAngleSimpleState, driveTimeout, TID2(br20, 2, 3));

//	// 3
//	_x = gPosition.x;
//	_y = gPosition.y;
//	normalize(_x, _y, -1, 56);
//	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 9, _y, _x, 60, 0, 0, 0, 0, stopNone, false);
//	driveTimeout = nPgmTime + 1500;
//	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(br20, 3, 1));
//	setDrive(30, 30);
//	driveTimeout = nPgmTime + 1500;
//	timeoutWhileLessThanF(VEL_NONE, 0, &gVelocity.y, -0.05, driveTimeout, TID2(br20, 3, 2), true, false);
//	setDrive(25, 25);
//	sleep(500);
//	//skip:
//	if (!resetSonarFull(100, 500, -0.75 * PI, 230, 470, 180, 650, false))
//		resetPositionFull(gPosition, gPosition.y, gPosition.x, 225);
//	_x = gPosition.x;
//	_y = gPosition.y;
//	normalize(_x, _y, -1, 56);
//	mobileSet(mobileDownToMiddle, mfClear);
//	coneTimeout = nPgmTime + 1500;
//	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(br20, 3, 3));
//	mobileSet(mobileIdle);
//	sleep(300);

//	// 4
//	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, -7, _y, _x, -60, 0, 0, 0, 0, stopHarsh, false);
//	driveTimeout = nPgmTime + 1500;
//	liftLowerSimpleAsync(LIFT_BOTTOM + 200, -127, 0);
//	sleep(300);
//	mobileSet(mobileBottom, mfNone);
//	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(br20, 4, 1));
//}

//void stationaryLeftCore(bool safe = false)
//{
//	unsigned long driveTimeout;
//	unsigned long coneTimeout;

//	gNumCones = 0;
//	gMobileCheckLift = true;

//	trackPositionTaskKill();
//	resetPositionFull(gPosition, 40, 16, 45);
//	resetVelocity(gVelocity, gPosition);
//	trackPositionTaskAsync();

//	coneTimeout = nPgmTime + 1400;

//	// 1
//	moveToTargetSimpleAsync(47, 23, gPosition.y, gPosition.x, 70, 0, 6, 20, 0, stopSoft | stopHarsh, false);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(ls, 1, 1));
//	turnToTargetSimpleAsync(47, 47, cw, 70, 70, false, 0);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(ls, 1, 2));
//	moveToTargetSimpleAsync(47, 47, gPosition.y, gPosition.x, 50, 0, 8, 25, 14, stopNone, true);
//	driveTimeout = nPgmTime + 2000;
//	stackSet(stackStationaryPrep, sfNone);
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(ls, 1, 3));
//	setDrive(25, 25);
//	driveTimeout = nPgmTime + 1500;
//	sleep(driveTimeout - 1500 - nPgmTime);
//	timeoutWhileGreaterThanF(VEL_NONE, 0, &gVelocity.x, 0.1, driveTimeout, TID2(ls, 1, 4), true, false);
//	setDrive(15, 15);
//	sleep(safe ? 400 : 200);
//	stackSet(stackStationary, sfNone);
//	coneTimeout = nPgmTime + 2000;
//	stackTimeoutWhile(stackStationary, coneTimeout, TID2(ls, 1, 5));
//}

//void stationaryRightCore(bool safe = false)
//{
//	unsigned long driveTimeout;
//	unsigned long coneTimeout;

//	gNumCones = 0;
//	gMobileCheckLift = true;

//	trackPositionTaskKill();
//	resetPositionFull(gPosition, 16, 40, 45);
//	resetVelocity(gVelocity, gPosition);
//	trackPositionTaskAsync();

//	coneTimeout = nPgmTime + 1400;

//	// 1
//	moveToTargetSimpleAsync(23, 47, gPosition.y, gPosition.x, 70, 0, 6, 20, 0, stopSoft | stopHarsh, false);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rs, 1, 1));
//	turnToTargetSimpleAsync(47, 47, ccw, 70, 70, false, 0);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(rs, 1, 2));
//	moveToTargetSimpleAsync(47, 47, gPosition.y, gPosition.x, 50, 0, 8, 25, 14, stopNone, true);
//	driveTimeout = nPgmTime + 2000;
//	stackSet(stackStationaryPrep, sfNone);
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rs, 1, 3));
//	setDrive(25, 25);
//	driveTimeout = nPgmTime + 1500;
//	sleep(driveTimeout - 1500 - nPgmTime);
//	timeoutWhileGreaterThanF(VEL_NONE, 0, &gVelocity.y, 0.1, driveTimeout, TID2(rs, 1, 4), true, false);
//	setDrive(15, 15);
//	sleep(safe ? 400 : 200);
//	stackSet(stackStationary, sfNone);
//	coneTimeout = nPgmTime + 2000;
//	stackTimeoutWhile(stackStationary, coneTimeout, TID2(rs, 1, 5));
//}

//void autoStationaryLeftBlock()
//{
//	unsigned long driveTimeout;
//	unsigned long coneTimeout;

//	stationaryLeftCore(true);

//	// 2
//	turnToTargetCustomAsync(74, 20, ch, PI, 70, 0.2);
//	driveTimeout = nPgmTime + 3000;
//	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(ls+b, 2, 1));
//	moveToTargetSimpleAsync(74, 20, gPosition.y, gPosition.x, -70, 0, 6, 20, 0, stopSoft | stopHarsh, true);
//	driveTimeout = nPgmTime + 2000;
//	timeoutWhileGreaterThanF(VEL_LOCAL_Y, 1.0, &gPosition.x, 36, driveTimeout, TID2(ls+b, 2, 2), true, false);
//	liftLowerSimpleAsync(LIFT_BOTTOM , -127, 0);
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(ls+b, 2, 3));
//	turnToTargetSimpleAsync(126, 70, ch, 127, 127, false, PI);
//	driveTimeout = nPgmTime + 1500;
//	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(ls+b, 2, 4));
//	moveToTargetSimpleAsync(126, 70, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, stopSoft, false);
//	driveTimeout = nPgmTime + 3000;
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(ls+b, 2, 5));
//}

//void autoStationaryRightBlock()
//{
//	unsigned long driveTimeout;
//	unsigned long coneTimeout;

//	stationaryRightCore(true);

//	// 2
//	turnToTargetCustomAsync(20, 74, ch, PI, 70, 0.2);
//	driveTimeout = nPgmTime + 3000;
//	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(rs+b, 2, 1));
//	moveToTargetSimpleAsync(20, 74, gPosition.y, gPosition.x, -70, 0, 6, 20, 0, stopSoft | stopHarsh, true);
//	driveTimeout = nPgmTime + 1500;
//	timeoutWhileGreaterThanF(VEL_LOCAL_Y, 1.0, &gPosition.y, 36, driveTimeout, TID2(rs+b, 2, 2), true, false);
//	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rs+b, 2, 3));
//	turnToTargetSimpleAsync(70, 126, ch, 80, 80, false, PI);
//	driveTimeout = nPgmTime + 1500;
//	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(rs+b, 2, 4));
//	moveToTargetSimpleAsync(70, 126, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, stopSoft, false);
//	driveTimeout = nPgmTime + 3000;
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rs+b, 2, 5));
//}

//void autoStationaryLeft5()
//{
//	unsigned long driveTimeout;
//	unsigned long coneTimeout;

//	stationaryLeftCore(true);

//	// 2
//	turnToTargetCustomAsync(50, 15, ch, PI, 70, 0.2);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(ls+5, 2, 1));
//	moveToTargetSimpleAsync(50, 15, gPosition.y, gPosition.x, -70, 0, 6, -20, 0, stopSoft | stopHarsh, true);
//	driveTimeout = nPgmTime + 3000;
//	sleep(500);
//	mobileSet(mobileBottom, mfNone);
//	timeoutWhileGreaterThanF(VEL_LOCAL_Y, 1.0, &gPosition.x, 36, driveTimeout, TID2(ls+5, 2, 2), true, false);
//	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(ls+5, 2, 3));
//	turnToTargetSimpleAsync(107, 14, ccw, 80, 80, false, 0);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(ls+5, 2, 4));
//	moveToTargetSimpleAsync(107, 14, gPosition.y, gPosition.x, 127, 0, 12, 30, 12, stopNone, true);
//	driveTimeout = nPgmTime + 2500;
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(ls+5, 2, 5));
//	setDrive(30, 30);
//	driveTimeout = nPgmTime + 1500;
//	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(ls+5, 2, 6));
//	setDrive(0, 0);
//	mobileSet(mobileTop, mfClear);
//	coneTimeout = nPgmTime + 2000;
//	timeoutWhileLessThanL(VEL_SENSOR(mobilePoti), 0.5, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(ls+5, 2, 7));

//	// 3
//	turnToTargetCustomAsync(62, 18, ch, PI, 45, 0.05);
//	driveTimeout = nPgmTime + 3000;
//	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
//	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(ls+5, 3, 1));
//	moveToTargetDisSimpleAsync(gPosition.a, -36, gPosition.y, gPosition.x, -127, 0, 12, -40, 0, stopHarsh, true);
//	driveTimeout = nPgmTime + 3000;
//	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(ls+5, 3, 2));
//	turnToAngleCustomAsync(PI / -2, ccw, 127, 0.2);
//	driveTimeout = nPgmTime + 3000;
//	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, -10);
//	autoSimpleTimeoutWhile(turnToAngleCustomState, driveTimeout, TID2(ls+5, 3, 3));
//	mobileSet(mobileBottom, mfClear);
//	coneTimeout = nPgmTime + 2000;
//	timeoutWhileGreaterThanL(VEL_SENSOR(mobilePoti), 0.5, &gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(ls+5, 3, 4));
//	moveToTargetDisSimpleAsync(gPosition.a, -8, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, stopNone, true);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(ls+5, 3, 5));
//}

//void autoStationaryRight5()
//{
//	unsigned long driveTimeout;
//	unsigned long coneTimeout;

//	stationaryRightCore(true);

//	// 2
//	turnToTargetCustomAsync(15, 50, ch, PI, 70, 0.2);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(rs+5, 2, 1));
//	moveToTargetSimpleAsync(15, 50, gPosition.y, gPosition.x, -70, 0, 6, -20, 0, stopSoft | stopHarsh, true);
//	driveTimeout = nPgmTime + 3000;
//	sleep(500);
//	mobileSet(mobileBottom, mfNone);
//	timeoutWhileGreaterThanF(VEL_LOCAL_Y, 1.0, &gPosition.y, 36, driveTimeout, TID2(rs+5, 2, 2), true, false);
//	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rs+5, 2, 3));
//	turnToTargetSimpleAsync(14, 107, cw, 80, 80, false, 0);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(rs+5, 2, 4));
//	moveToTargetSimpleAsync(14, 107, gPosition.y, gPosition.x, 127, 0, 12, 30, 12, stopNone, true);
//	driveTimeout = nPgmTime + 2500;
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rs+5, 2, 5));
//	setDrive(30, 30);
//	driveTimeout = nPgmTime + 1500;
//	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(rs+5, 2, 6));
//	setDrive(0, 0);
//	mobileSet(mobileTop, mfClear);
//	coneTimeout = nPgmTime + 2000;
//	timeoutWhileLessThanL(VEL_SENSOR(mobilePoti), 0.5, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(rs+5, 2, 7));

//	// 3
//	turnToTargetCustomAsync(18, 62, ch, PI, 45, 0.05);
//	driveTimeout = nPgmTime + 3000;
//	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
//	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(rs+5, 3, 1));
//	moveToTargetDisSimpleAsync(gPosition.a, -36, gPosition.y, gPosition.x, -127, 0, 12, -40, 0, stopHarsh, true);
//	driveTimeout = nPgmTime + 3000;
//	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(rs+5, 3, 2));
//	turnToAngleCustomAsync(PI, cw, 127, 0.2);
//	driveTimeout = nPgmTime + 3000;
//	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, -10);
//	autoSimpleTimeoutWhile(turnToAngleCustomState, driveTimeout, TID2(rs+5, 3, 3));
//	mobileSet(mobileBottom, mfClear);
//	coneTimeout = nPgmTime + 2000;
//	timeoutWhileGreaterThanL(VEL_SENSOR(mobilePoti), 0.5, &gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(rs+5, 3, 4));
//	moveToTargetDisSimpleAsync(gPosition.a, -8, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, stopNone, true);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(rs+5, 3, 5));
//}

//void autoStationaryLeft2()
//{
//	unsigned long driveTimeout;
//	unsigned long coneTimeout;

//	stationaryLeftCore();

//	// 2
//	turnToTargetCustomAsync(50, 23, ch, PI, 70, 0.2);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(ls+2, 2, 1));
//	moveToTargetSimpleAsync(50, 23, gPosition.y, gPosition.x, -70, 0, 6, -20, 0, stopSoft | stopHarsh, true);
//	driveTimeout = nPgmTime + 3000;
//	timeoutWhileGreaterThanF(VEL_LOCAL_Y, 1.0, &gPosition.x, 36, driveTimeout, TID2(ls+2, 2, 2), true, false);
//	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(ls+2, 2, 3));
//	turnToTargetSimpleAsync(71, 24, ccw, 80, 80, false, 0);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(ls+2, 2, 4));
//	moveToTargetSimpleAsync(71, 24, gPosition.y, gPosition.x, 70, 0, 12, 30, 11, stopHarsh, true);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(ls+2, 2, 5));
//	stackSet(stackPickupGround, sfNone);
//	coneTimeout = nPgmTime + 1200;
//	stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(ls+2, 2, 6));

//	// 3
//	turnToTargetSimpleAsync(47, 47, cw, 50, 50, false, 0);
//	driveTimeout = nPgmTime + 2000;
//	stackSet(stackStationaryPrep, sfNone);
//	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(ls+2, 3, 1));
//	moveToTargetSimpleAsync(47, 47, gPosition.y, gPosition.x, 70, 0, 8, 25, 14, stopNone, false);
//	driveTimeout = nPgmTime + 2500;
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(ls+2, 3, 2));
//	setDrive(25, 25);
//	driveTimeout = nPgmTime + 1500;
//	timeoutWhileGreaterThanF(VEL_NONE, 0, &gVelocity.x, 0.1, driveTimeout, TID2(ls+2, 3, 3), true, false);
//	setDrive(15, 15);
//	sleep(200);
//	stackSet(stackStationary, sfNone);
//	coneTimeout = nPgmTime + 2000;
//	stackTimeoutWhile(stackStationary, coneTimeout, TID2(ls+2, 3, 4));

//	// 4
//	moveToTargetDisSimpleAsync(gPosition.a, -18, gPosition.y, gPosition.x, -40, 0, 0, 0, 0, stopNone, true);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(ls+2, 4, 1));
//	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
//	coneTimeout = nPgmTime + 1200;
//	liftTimeoutWhile(liftLowerSimpleState, coneTimeout, TID2(ls+2, 4, 2));
//}

//void autoStationaryRight2()
//{
//	unsigned long driveTimeout;
//	unsigned long coneTimeout;

//	stationaryRightCore();

//	// 2
//	turnToTargetCustomAsync(23, 50, ch, PI, 70, 0.2);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(rs+2, 2, 1));
//	moveToTargetSimpleAsync(23, 50, gPosition.y, gPosition.x, -70, 0, 6, -20, 0, stopSoft | stopHarsh, true);
//	driveTimeout = nPgmTime + 3000;
//	timeoutWhileGreaterThanF(VEL_LOCAL_Y, 1.0, &gPosition.y, 36, driveTimeout, TID2(rs+2, 2, 2), true, false);
//	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rs+2, 2, 3));
//	turnToTargetSimpleAsync(24, 71, cw, 80, 80, false, 0);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(rs+2, 2, 4));
//	moveToTargetSimpleAsync(24, 71, gPosition.y, gPosition.x, 70, 0, 12, 30, 11, stopHarsh, true);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rs+2, 2, 5));
//	stackSet(stackPickupGround, sfNone);
//	coneTimeout = nPgmTime + 1200;
//	stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(rs+2, 2, 6));

//	// 3
//	turnToTargetSimpleAsync(47, 47, ccw, 50, 50, false, 0);
//	driveTimeout = nPgmTime + 2000;
//	stackSet(stackStationaryPrep, sfNone);
//	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(rs+2, 3, 1));
//	moveToTargetSimpleAsync(47, 47, gPosition.y, gPosition.x, 70, 0, 8, 25, 14, stopNone, false);
//	driveTimeout = nPgmTime + 2500;
//	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rs+2, 3, 2));
//	setDrive(25, 25);
//	driveTimeout = nPgmTime + 1500;
//	timeoutWhileGreaterThanF(VEL_NONE, 0, &gVelocity.y, 0.1, driveTimeout, TID2(rs+2, 3, 3), true, false);
//	setDrive(15, 15);
//	sleep(200);
//	stackSet(stackStationary, sfNone);
//	coneTimeout = nPgmTime + 2000;
//	stackTimeoutWhile(stackStationary, coneTimeout, TID2(rs+2, 3, 4));

//	// 4
//	moveToTargetDisSimpleAsync(gPosition.a, -18, gPosition.y, gPosition.x, -40, 0, 0, 0, 0, stopNone, true);
//	driveTimeout = nPgmTime + 2000;
//	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(rs+2, 4, 1));
//	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
//	coneTimeout = nPgmTime + 1200;
//	liftTimeoutWhile(liftLowerSimpleState, coneTimeout, TID2(rs+2, 4, 2));
//}

void autoTest ()
{
	writeDebugStreamLine("%006d Start test: resetting", nPgmTime);
	trackPositionTaskKill();
	resetPositionFull(gPosition, 0, 0, PI);
	resetVelocity(gVelocity, gPosition);
	trackPositionTaskAsync();

	////liftSet(liftHoldDown);
	//moveToTargetSimple(48, 0, 0, 0, -127, 0, 0, 0, 0, stopNone, true);
	////mobileSet(mobileBottom, mfClear);
	//sweepTurnToTarget(65, 7, 0.25 * PI, 24, cw, -127, false);
	//moveToTargetSimple(89, 31, 65, 7, -127, 1.5, 0, 0, 0, stopSoft, true);

	//turnToAngleNewAlg(PI / 4, cw, 0.27, 23, 12, true);
	//sleep(1000);
	//writeDebugStreamLine("%.1f", radToDeg(gPosition.a));

	sweepTurnToTarget(24, -24, PI / -2, 24, ccw, -127, false);
	setDrive(7, -7);
	sleep(150);
	setDrive(0, 0);
}
