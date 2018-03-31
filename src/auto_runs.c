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
			/*case 1: auto20Right(); break;
			case 2: auto20Left(); break;
			case 3: autoStationaryRightBlock(); break;
			case 4: autoStationaryLeftBlock(); break;
			case 5: autoStationaryRight5(); break;
			case 6: autoStationaryLeft5(); break;
			case 7: autoStationaryLeft2(); break;*/
		}
	}
	else
	{
		switch (gCurAuto)
		{
			case 0: autoBlock(); break;
			/*case 1: auto20Left(); break;
			case 2: auto20Right(); break;
			case 3: autoStationaryLeftBlock(); break;
			case 4: autoStationaryRightBlock(); break;
			case 5: autoStationaryLeft5(); break;
			case 6: autoStationaryRight5(); break;
			case 7: autoStationaryRight2(); break;*/
		}
	}
}

void normalize(float& x, float& y, float m, float b)
{
	float _b = y + x / m;
	x = (_b - b) / (m + 1 / m);
	y = m * x + b;
}

#define START_BAR_INTERCEPT 43.7
#define START_BAR_ROBOT_OFFSET 6.5
#define START_BAR_RESET_INTERCEPT (START_BAR_INTERCEPT + START_BAR_ROBOT_OFFSET * sqrt(2))

float calcYCoord(float u, float _a)
{
	const float x = 7.7;
	const float y = 2.5;
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
	const float x = 7.7;
	const float y = 2.5;
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
		y = 141.5 - y;
		x = 141.5 - x;
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
		y = 141.5 - y;
		x = 141.5 - x;
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
		y = 141.5 - y;
		x = 141.5 - x;
		a -= PI;
	}
	resetPositionFull(gPosition, y, x, a);
	resetVelocity(gVelocity, gPosition);
	return true;
}

void driveAgainstStartingBar(word left, word right, word leftSlow, word rightSlow, unsigned long timeout)
{
	if (timeout)
	{
		unsigned long st = nPgmTime;
		timeout += st;
	}
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
	unsigned long st = nPgmTime;
	sleep(timeout - st);
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

#if SKILLS_ROUTE == 1

	trackPositionTaskKill();
	resetPositionFull(gPosition, 47, 14.25, 0);
	resetVelocity(gVelocity, gPosition);
	trackPositionTaskAsync();

	//segment = 5;

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
	case 5:
		goto skip5;
	}

	//killAutoAsync(gAutoTime + 60000);

	// 1
	moveToTargetSimpleAsync(107, 14.25, 47, 15, 70, 0, 0.5, 6, 55, 14, stopNone, mttProportional);
	driveTimeout = nPgmTime + 2000;
	liftRaiseSimpleAsync(gLiftRaiseTarget[0], 127, 0);
	sleep(300);
	mobileSet(mobileBottom, mfNone);
	DRIVE_AWAIT(skills, 1, 1);
	setDrive(55, 55);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 1, 2), false);
	setDrive(-7, -7);
	mobileSet(mobileTop, mfClear);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 1, 3));

	// 2
	moveToTargetSimpleAsync(71, 15, gPosition.y, gPosition.x, -127, 0, 0.5, 4, -90, 0, stopNone, mttProportional);
	driveTimeout = nPgmTime + 2000;
	stackSet(stackStack, sfDetach, true);
	DRIVE_AWAIT(skills, 2, 1);
	moveToTargetSimpleAsync(33, 33, gPosition.y, gPosition.x, -127, -90, 1, 0, 0, 0, stopSoft | stopHarsh, mttSimple);
	driveTimeout = nPgmTime + 2300;
	DRIVE_AWAIT(skills, 2, 2);
	turnToAngleNewAlgAsync(-0.75 * PI, ccw, 0.35, 26, 12, true);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 2, 3);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 8, _y, _x, 60, 0, 0, 0, 0, 0, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 2, 4);
	setDrive(45, 45);
	driveTimeout = nPgmTime + 1500;
	stackSet(stackClear, sfNone);
	coneTimeout = nPgmTime + 1000;
	sleep(300);
	timeoutWhileLessThanF(VEL_NONE, 0, &gVelocity.y, -0.05, driveTimeout, TID2(skills, 2, 5));
	setDrive(15, 15);
	stackTimeoutWhile(stackClear, coneTimeout, TID2(skills, 2, 6));
	mobileSet(mobileDownToMiddle, mfNone);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(skills, 2, 7));
	sleep(300);

	// 3
	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, -7, _y, _x, -60, 0, 0, 0, 0, 0, stopHarsh, mttSimple);
	driveTimeout = nPgmTime + 1500;
	liftLowerSimpleAsync(LIFT_BOTTOM + 200, -127, 0);
	DRIVE_AWAIT(skills, 3, 1);
	turnToAngleNewAlgAsync(-0.25 * PI, cw, 0.27, 23, 12, false);
	driveTimeout = nPgmTime + 1500;
	mobileSet(mobileBottom, mfNone);
	DRIVE_AWAIT(skills, 3, 2);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, 1, 0);
	sweepTurnToTargetAsync(_y + 23, _x, 45 * PI / 180, 16.5, cw, 127, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 3, 3);
	moveToTargetSimpleAsync(71, 47, gPosition.y, gPosition.x, 127, 127, 1, 0, 0, 14, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1000;
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 80, 0);
	DRIVE_AWAIT(skills, 3, 4);
	setDrive(40, 40);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 3, 5), false);
	mobileSet(mobileTop, mfClear);
	setDrive(-7, -7);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP, coneTimeout, TID2(skills, 3, 6));

	// 4
	turnToTargetNewAlgAsync(26, 8, cw, 0.55, 35, 20, true, 0);
	driveTimeout = nPgmTime + 2200;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	DRIVE_AWAIT(skills, 4, 1);
	moveToTargetSimpleAsync(26, 8, gPosition.y, gPosition.x, 127, 0, 0.5, 20, 30, 15, stopNone, mttCascading);
	driveTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanF(VEL_NONE, 0, &gPosition.x, 23, driveTimeout, TID2(skils, 4, 2));
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, 0);
	DRIVE_AWAIT(skills, 4, 3);
	mobileSet(mobileBottom, mfNone);
	driveAgainstStartingBar(25, 25, 15, 15, 1500);
	coneTimeout = nPgmTime + 2000;
	sleep(500);
	if (segment > -1)
		return;
skip1:
	if (!resetSonarYOnly(100, 500, START_BAR_RESET_INTERCEPT, -0.75 * PI, 600, 900, false) && !resetSonarXOnly(100, 500, START_BAR_RESET_INTERCEPT, -0.75 * PI, 130, 330, false))
	{
		if (segment > -1)
			return;
		resetPositionFull(gPosition, gPosition.y, gPosition.x, -0.75 * PI);
	}

	// 5
	moveToTargetDisSimpleAsync(gPosition.a, -8, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, 0, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 5, 1);
	turnToTargetNewAlgAsync(71, 47, cw, 0.4, 26, 11, false, 0);
	driveTimeout = nPgmTime + 1700;
	liftLowerSimpleAsync(LIFT_BOTTOM, -70, 0);
	DRIVE_AWAIT(skills, 5, 2);
	moveToTargetSimpleAsync(71, 47, gPosition.y, gPosition.x, 127, 0, 0.5, 0, 0, 5, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 5, 3);
	moveToTargetSimpleAsync(95, 71, gPosition.y, gPosition.x, 127, 127, 0.5, 0, 0, 14, stopNone, mttProportional);
	driveTimeout = nPgmTime + 2500;
	DRIVE_AWAIT(skills, 5, 4);
	setDrive(70, 70);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 5, 6), false);
	mobileSet(mobileUpToMiddle, mfClear);

	// 6
	moveToTargetSimpleAsync(136, 112, gPosition.y, gPosition.x, 127, 70, 1, 12, 30, 13, stopNone, mttProportional);
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 6, 2);
	mobileSet(mobileBottom, mfClear);
	driveAgainstStartingBar(25, 25, 15, 15, 1500);
	sleep(500);
	if (segment > -1)
		return;
skip2:
	mobileSet(mobileBottom, mfNone);
	setDrive(15, 15);
	if (!resetSonarXOnly(100, 500, START_BAR_RESET_INTERCEPT, 0.25 * PI, 560, 830, true) && !resetSonarYOnly(100, 500, START_BAR_RESET_INTERCEPT, 0.25 * PI, 130, 330, true))
	{
		if (segment > -1)
			return;
		resetPositionFull(gPosition, gPosition.y, gPosition.x, 0.25 * PI);
	}

	// 7
	moveToTargetDisSimpleAsync(gPosition.a, -8, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, 0, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 7, 1);
	turnToTargetNewAlgAsync(128, 71, ccw, 0.27, 23, 12, false, 0);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 7, 2);
	moveToTargetSimpleAsync(128, 71, gPosition.y, gPosition.x, 70, 0, 0.5, 0, 0, 16, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 7, 3);
	moveToTargetSimpleAsync(128, 35, gPosition.y, gPosition.x, 127, 70, 0.5, 8, 55, 14, stopNone, mttProportional);
	driveTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanF(VEL_NONE, 0, &gPosition.x, 58, driveTimeout, TID2(skills, 7, 4));
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, 0);
	armSet(armToTarget, ARM_HORIZONTAL);
	DRIVE_AWAIT(skills, 7, 5);
	setDrive(55, 55);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 7, 6), false);
	mobileSet(mobileTop, mfClear);
	coneTimeout = nPgmTime + 2000;
	moveToTargetSimpleAsync(129, 23, gPosition.y, gPosition.x, 55, 55, 0.5, 0, 0, 9, stopNone, mttProportional);
	driveTimeout = nPgmTime + 1300;
	DRIVE_AWAIT(skills, 7, 7);
	setDrive(-6, -6);
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP, coneTimeout, TID2(skills, 7, 8));
	stackSet(stackPickupGround, sfStack | sfDetach, true);
	coneTimeout = nPgmTime + 800;
	stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(skills, 7, 9), false);

	// 8
	moveToTargetSimpleAsync(127, 71, gPosition.y, gPosition.x, -127, 0, 0.5, 6, -90, 0, stopNone, mttProportional);
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 8, 1);
	moveToTargetSimpleAsync(106, 110, gPosition.y, gPosition.x, -127, -127, 1, 0, 0, 0, stopHarsh, mttCascading);
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 8, 2);
	turnToAngleNewAlgAsync(0.25 * PI, cw, 0.35, 28, 11, true);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 8, 3);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 226);
	moveToTargetDisSimpleAsync(0.25 * PI, 8, _y, _x, 60, 0, 0, 0, 0, 0, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 8, 4);
	setDrive(45, 45);
	driveTimeout = nPgmTime + 1500;
	sleep(300);
	timeoutWhileGreaterThanF(VEL_NONE, 0, &gVelocity.x, 0.05, driveTimeout, TID2(skills, 8, 5));
	setDrive(15, 15);
	stackSet(stackClear, sfNone);
	sleep(500);
	if (segment > -1)
		return;
skip3:
	if (!resetSonarFull(100, 500, 0.25 * PI, 230, 470, 180, 650, true))
	{
		if (segment > -1)
			return;
		resetPositionFull(gPosition, gPosition.y, gPosition.x, 0.25 * PI);
	}
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 226);
	mobileSet(mobileDownToMiddle, mfClear);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(skills, 8, 6));
	sleep(300);

	// 9
	moveToTargetDisSimpleAsync(0.25 * PI, -9, _y, _x, -60, 0, 0, 0, 0, 0, stopHarsh, mttSimple);
	driveTimeout = nPgmTime + 1700;
	liftLowerSimpleAsync(LIFT_BOTTOM + 200, -127, 0);
	DRIVE_AWAIT(skills, 9, 1);
	turnToTargetNewAlgAsync(60, 127, cw, 0.27, 23, 13, false, 0);
	driveTimeout = nPgmTime + 2000;
	mobileSet(mobileBottom, mfNone);
	DRIVE_AWAIT(skills, 9, 2);
	moveToTargetSimpleAsync(60, 127, gPosition.y, gPosition.x, 127, 0, 1, 0, 0, 12, stopNone, mttProportional);
	driveTimeout = nPgmTime + 1700;
	DRIVE_AWAIT(skills, 9, 3);
	moveToTargetSimpleAsync(35, 128, gPosition.y, gPosition.x, 127, 127, 0.5, 0, 0, 14, stopNone, mttProportional);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileGreaterThanF(VEL_NONE, 0, &gPosition.y, 60, driveTimeout - 500, TID2(skills, 9, 4));
	liftRaiseSimpleAsync(gLiftRaiseTarget[0], 127, 0);
	armSet(armToTarget, ARM_HORIZONTAL);
	DRIVE_AWAIT(skills, 9, 5);
	setDrive(55, 55);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 9, 6), false);
	moveToTargetSimpleAsync(23, 129, gPosition.y, gPosition.x, 55, 55, 0.5, 0, 0, 9, stopNone, mttProportional);
	driveTimeout = nPgmTime + 1200;
	mobileSet(mobileTop, mfNone);
	coneTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 9, 7);
	setDrive(-6, -6);
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 9, 8));
	stackSet(stackPickupGround, STACK_RAPID_CONFIG(sfClear, 2), true);
	coneTimeout = nPgmTime + 800;
	stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(skills, 9, 9), false);
	coneTimeout = nPgmTime + 1000;
	moveToTargetSimpleAsync(13, 129, gPosition.y, gPosition.x, 55, 30, 0.5, 0, 0, 9, stopHarsh, mttProportional);
	stackTimeoutUntil(stackPickupGround, coneTimeout, TID2(skills, 9, 10), false);
	coneTimeout = nPgmTime + 800;
	stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(skills, 9, 11), false);

	// 10
	moveToTargetSimpleAsync(60, 125, gPosition.y, gPosition.x, -127, 0, 1, 12, -90, 0, stopNone, mttProportional);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 10, 1);
	moveToTargetSimpleAsync(107, 107, gPosition.y, gPosition.x, -127, 0, 1, 0, 0, 3, stopNone, mttCascading);
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 10, 2);
	turnToAngleNewAlgAsync(0.25 * PI, ccw, 0.4, 26, 11, true);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 10, 3);
	moveToTargetDisSimpleAsync(gPosition.a, 1.5, gPosition.y, gPosition.x, 80, 0, 0, 0, 0, 0, stopNone, mttSimple);
	driveTimeout = nPgmTime + 800;
	mobileSet(mobileBottom, mfClear);
	coneTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 10, 4);
	driveAgainstStartingBar(25, 25, 15, 15, 2000);
	timeoutWhileGreaterThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 10, 5));

	// 11
	moveToTargetDisSimpleAsync(gPosition.a, -8, gPosition.y, gPosition.x, -80, 0, 0, 0, 0, 0, stopHarsh, mttSimple);
	driveTimeout = nPgmTime + 1500;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	DRIVE_AWAIT(skills, 11, 1);
	turnToAngleNewAlgAsync(0.75 * PI, cw, 0.27, 23, 12, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 11, 2);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, 1, 0);
	sweepTurnToTargetAsync(_y - 22, _x, 225 * PI / 180, 16, cw, 127, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 11, 3);
	moveToTargetSimpleAsync(72, 94, gPosition.y, gPosition.x, 127, 127, 0.5, 0, 0, 14, stopNone, mttSimple);
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 11, 4);
	setDrive(40, 40);
	driveTimeout = nPgmTime + 700;
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, 0);
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 11, 5), false);
	setDrive(-7, -7);
	mobileSet(mobileTop, mfNone);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 11, 6));

	// 12
	turnToTargetNewAlgAsync(116, 132, cw, 0.55, 35, 20, true, 0);
	driveTimeout = nPgmTime + 1500;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	DRIVE_AWAIT(skills, 12, 1);
	moveToTargetSimpleAsync(116, 132, gPosition.y, gPosition.x, 127, 0, 0.5, 20, 30, 15, stopNone, mttCascading);
	driveTimeout = nPgmTime + 2000;
	timeoutWhileLessThanF(VEL_NONE, 0, &gPosition.x, 119, driveTimeout, TID2(skills, 12, 2), true, false);
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, 0);
	DRIVE_AWAIT(skills, 12, 3);
	mobileSet(mobileBottom, mfNone);
	driveAgainstStartingBar(25, 25, 15, 15, 1500);
	sleep(500);
	if (segment > -1)
		return;
skip4:
	if (!resetSonarXOnly(100, 500, START_BAR_RESET_INTERCEPT, 0.25 * PI, 150, 350, true))
	{
		if (segment > -1)
			return;
		resetPositionFull(gPosition, gPosition.y, gPosition.x, 0.25 * PI);
	}

	// 13
	moveToTargetDisSimpleAsync(gPosition.a, -7, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, 0, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1500;
	liftLowerSimpleAsync(LIFT_BOTTOM, -80, 0);
	DRIVE_AWAIT(skills, 13, 1);
	turnToTargetNewAlgAsync(71, 95, cw, 0.5, 30, 20, false, 0);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 13, 2);
	moveToTargetSimpleAsync(71, 95, gPosition.y, gPosition.x, 127, 0, 0.5, 0, 0, 5, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 13, 3);
	moveToTargetSimpleAsync(47, 71, gPosition.y, gPosition.x, 127, 127, 1, 0, 0, 14, stopNone, mttProportional);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 13, 4);
	setDrive(70, 70);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 13, 5), false);
	mobileSet(mobileUpToMiddle, mfNone);

	// 14
	moveToTargetSimpleAsync(7, 27, gPosition.y, gPosition.x, 127, 70, 1, 10, 30, 15, stopNone, mttCascading);
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 14, 1);
	mobileSet(mobileBottom, mfNone);
	driveAgainstStartingBar(25, 25, 15, 15, 1500);
	sleep(500);
	if (segment > -1)
		return;
skip5:
	if (!resetSonarYOnly(100, 500, START_BAR_RESET_INTERCEPT, -0.75 * PI, 130, 330, false))
	{
		if (segment > -1)
			return;
		resetPositionFull(gPosition, gPosition.y, gPosition.x, -0.75 * PI);
	}

	// 15
	moveToTargetDisSimpleAsync(gPosition.a, -6, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, 0, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 15, 1);
	turnToTargetNewAlgAsync(12, 106, ccw, 0.27, 23, 11, false, 0);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 15, 2);
	moveToTargetSimpleAsync(12, 106, gPosition.y, gPosition.x, 127, 0, 0.5, 0, 0, 14, stopNone, mttSimple);
	driveTimeout = nPgmTime + 2000;
	timeoutWhileLessThanF(VEL_NONE, 0, &gPosition.x, 71, driveTimeout, TID2(skills, 15, 3));
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, 0);
	DRIVE_AWAIT(skills, 15, 4);
	setDrive(55, 55);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 15, 5), false);
	setDrive(-7, -7);
	mobileSet(mobileTop, mfNone);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 15, 6));

	// 16
	moveToTargetSimpleAsync(32, 32, gPosition.y, gPosition.x, -127, 0, 1, 0, 0, 0, stopNone, mttCascading);
	driveTimeout = nPgmTime + 2500;
	DRIVE_AWAIT(skills, 16, 1);
	turnToAngleNewAlgAsync(-0.75 * PI, cw, 0.7, 40, 30, true);
	driveTimeout = nPgmTime + 1500;
	sleep(300);
	mobileSet(mobileBottom, mfNone);
	coneTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 16, 2);
	moveToTargetDisSimpleAsync(gPosition.a, 2, gPosition.y, gPosition.x, 127, 0, 0, 0, 0, 0, stopHarsh, mttSimple);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 16, 3);
	timeoutWhileGreaterThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 16, 4));
	moveToTargetDisSimpleAsync(gPosition.a, -5, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, 0, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 16, 5);

#elif SKILLS_ROUTE == 2

	trackPositionTaskKill();
	resetPositionFull(gPosition, 40, 16, PI / 4);
	resetVelocity(gVelocity, gPosition);
	trackPositionTaskAsync();

	segment = 2;

	switch (segment)
	{
	case 1:
		stackSet(stackClear, STACK_CLEAR_CONFIG(sfNone, mobileDownToMiddle, mfNone));
		coneTimeout = nPgmTime + 2000;
		goto skip1;
	case 2:
		stackSet(stackClear, STACK_CLEAR_CONFIG(sfNone, mobileDownToMiddle, mfNone));
		coneTimeout = nPgmTime + 2000;
		goto skip2;
	}

	// 1
	moveToTargetSimpleAsync(71, 47, 40, 16, 45, 0, 1, 0, 0, 14, stopNone, mttSimple);
	driveTimeout = nPgmTime + 2000;
	liftRaiseSimpleAsync(gLiftRaiseTarget[0] - 70, 127, 0);
	sleep(300);
	mobileSet(mobileBottom, mfNone);
	DRIVE_AWAIT(skills, 1, 1);
	setDrive(55, 55);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 1, 2), false);
	mobileSet(mobileTop, mfClear);

	// 2
	coneTimeout = nPgmTime + 2000;
	moveToTargetSimpleAsync(95, 71, gPosition.y, gPosition.x, 127, 70, 1, 6, 70, 14, stopNone, mttSimple);
	driveTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 2, 1));
	stackSet(stackStack, sfDetach);
	coneTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 2, 2);
	moveToTargetSimpleAsync(136, 112, gPosition.y, gPosition.x, 127, 70, 1, 42, 50, 0, stopNone, mttCascading);
	driveTimeout = nPgmTime + 2000;
	stackTimeoutUntil(stackNotRunning, coneTimeout, TID2(skills, 2, 3));
	stackSet(stackTiltPrep, sfTilt);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanF(VEL_NONE, 0, &gPosition.y, 100, driveTimeout, TID2(skills, 2, 4));
	while (!gSensor[lsField].value)
	{
		if (TimedOut(driveTimeout, TID2(skills, 2, 5), false, VEL_NONE, 0, 0))
		{
			_y = gPosition.y;
			_x = gPosition.x;
			normalize(_x, _y, -1, 209);
			goto noLine1;
		}
		writeDebugStreamLine("%d", gSensor[lsField].rawValue);
		sleep(10);
	}
	_y = gPosition.y;
	_x = gPosition.x;
noLine1:
	autoSimpleSet(autoSimpleNotRunning);
	setDrive(0, 0);
	stackTimeoutUntil(stackNotRunning, coneTimeout, TID2(skills, 2, 6));
	moveToTargetDisSimpleAsync(0.25 * PI, 11, _y, _x, 127, 0, 0, 0, 0, 0, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 2, 7);
	stackSet(stackDetachMobile, sfNone);
	coneTimeout = nPgmTime + 1000;
	stackTimeoutWhile(stackDetachMobile, coneTimeout, TID2(skills, 2, 8));

	// 3
	moveToTargetDisSimpleAsync(gPosition.a, -3, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, 0, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 3, 1);
	turnToTargetNewAlgAsync(108, 108, cw, 0.35, 23, 12, true, 0);
	driveTimeout = nPgmTime + 1500;
	armSet(armToTarget, ARM_PRESTACK - 500);
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	DRIVE_AWAIT(skills, 3, 2);
	moveToTargetSimpleAsync(108, 108, gPosition.y, gPosition.x, 127, 50, 0.5, 0, 0, 6, stopNone, mttCascading);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 3, 3);
	turnToAngleNewAlgAsync(0.25 * PI, ccw, 0.35, 23, 12, true);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 3, 4);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 226);
	moveToTargetDisSimpleAsync(0.25 * PI, 8, _y, _x, 60, 0, 0, 0, 0, 0, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 3, 5);
	setDrive(45, 45);
	driveTimeout = nPgmTime + 1500;
	sleep(300);
	timeoutWhileGreaterThanF(VEL_NONE, 0, &gVelocity.x, 0.05, driveTimeout, TID2(skills, 3, 6));
	setDrive(15, 15);
	stackSet(stackClear, STACK_CLEAR_CONFIG(sfNone, mobileDownToMiddle, mfNone));
	coneTimeout = nPgmTime + 2000;
	mobileTimeoutUntil(mobileDownToMiddle, coneTimeout - 1000, TID2(skills, 3, 7));
	if (segment > -1)
		return;
skip1:
	if (!resetSonarFull(100, 500, 0.25 * PI, 230, 470, 180, 650, true))
	{
		if (segment > -1)
			return;
		resetPositionFull(gPosition, gPosition.y, gPosition.x, 0.25 * PI);
	}
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 226);
	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(skills, 3, 8));
	sleep(300);

	// 4
	moveToTargetDisSimpleAsync(0.25 * PI, -7, _y, _x, -60, 0, 0, 0, 0, 0, stopHarsh, mttSimple);
	driveTimeout = nPgmTime + 1500;
	liftLowerSimpleAsync(LIFT_BOTTOM + 200, -127, 0);
	DRIVE_AWAIT(skills, 4, 1);
	turnToAngleNewAlgAsync(0.75 * PI, cw, 0.27, 23, 12, false);
	driveTimeout = nPgmTime + 1500;
	mobileSet(mobileBottom, mfNone);
	DRIVE_AWAIT(skills, 4, 2);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, 1, 0);
	sweepTurnToTargetAsync(_y - 23, _x, 225 * PI / 180, 16.5, cw, 127, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 4, 3);
	moveToTargetSimpleAsync(71, 95, gPosition.y, gPosition.x, 127, 127, 1, 0, 0, 14, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1000;
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 80, 0);
	DRIVE_AWAIT(skills, 4, 4);
	setDrive(40, 40);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 4, 5), false);
	mobileSet(mobileTop, mfClear);

	// 5
	coneTimeout = nPgmTime + 2000;
	moveToTargetSimpleAsync(47, 71, gPosition.y, gPosition.x, 127, 70, 1, 6, 70, 14, stopNone, mttSimple);
	driveTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 5, 1));
	stackSet(stackTiltPrep, sfNone);
	DRIVE_AWAIT(skills, 5, 2);
	moveToTargetSimpleAsync(7, 27, gPosition.y, gPosition.x, 127, 70, 1, 42, 50, 0, stopNone, mttCascading);
	driveTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanF(VEL_NONE, 0, &gPosition.y, 41, driveTimeout, TID2(skills, 5, 3));
	stackSet(stackTiltMobile, sfNone);
	coneTimeout = nPgmTime + 2000;
	while (!gSensor[lsField].value)
	{
		if (TimedOut(driveTimeout, TID2(skills, 5, 4), false, VEL_NONE, 0, 0))
		{
			_y = gPosition.y;
			_x = gPosition.x;
			normalize(_x, _y, -1, 74);
			goto noLine2;
		}
		writeDebugStreamLine("%d", gSensor[lsField].rawValue);
		sleep(10);
	}
	_y = gPosition.y;
	_x = gPosition.x;
noLine2:
	moveToTargetDisSimpleAsync(-0.75 * PI, 11, _y, _x, 127, 0, 0, 0, 0, 0, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 5, 5);
	stackSet(stackDetachMobile, sfNone);
	coneTimeout = nPgmTime + 1000;
	stackTimeoutWhile(stackDetachMobile, coneTimeout, TID2(skills, 5, 6));

	// 6
	moveToTargetDisSimpleAsync(gPosition.a, -3, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, 0, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 6, 1);
	turnToTargetNewAlgAsync(34, 34, cw, 0.35, 23, 12, true, 0);
	driveTimeout = nPgmTime + 1500;
	armSet(armToTarget, ARM_PRESTACK - 500);
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	DRIVE_AWAIT(skills, 6, 2);
	moveToTargetSimpleAsync(33, 33, gPosition.y, gPosition.x, 127, 50, 0.5, 0, 0, 0, stopNone, mttCascading);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 6, 3);
	turnToAngleNewAlgAsync(-0.75 * PI, ccw, 0.35, 23, 12, true);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 6, 4);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	moveToTargetDisSimpleAsync(-0.75 * PI, 8, _y, _x, 60, 0, 0, 0, 0, 0, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 6, 5);
	setDrive(45, 45);
	driveTimeout = nPgmTime + 1500;
	sleep(300);
	timeoutWhileGreaterThanF(VEL_NONE, 0, &gVelocity.x, 0.05, driveTimeout, TID2(skills, 6, 6));
	setDrive(15, 15);
	stackSet(stackClear, STACK_CLEAR_CONFIG(sfNone, mobileDownToMiddle, mfNone));
	coneTimeout = nPgmTime + 2000;
	mobileTimeoutUntil(mobileDownToMiddle, coneTimeout - 1000, TID2(skills, 3, 7));
	if (segment > -1)
		return;
skip2:
	if (!resetSonarFull(100, 500, -0.75 * PI, 230, 470, 180, 650, false))
	{
		if (segment > -1)
			return;
		resetPositionFull(gPosition, gPosition.y, gPosition.x, -0.75 * PI);
	}
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(skills, 6, 8));
	sleep(300);

	// 7
	moveToTargetDisSimpleAsync(-0.75 * PI, -7, _y, _x, -60, 0, 0, 0, 0, 0, stopHarsh, mttSimple);
	driveTimeout = nPgmTime + 1500;
	liftLowerSimpleAsync(LIFT_BOTTOM + 200, -127, 0);
	DRIVE_AWAIT(skills, 7, 1);
	turnToTargetNewAlgAsync(17, 71, ccw, 0.27, 23, 11, false, 0); // 15.5 71
	driveTimeout = nPgmTime + 1500;
	mobileSet(mobileBottom, mfNone);
	DRIVE_AWAIT(skills, 7, 2);
	moveToTargetSimpleAsync(17, 71, gPosition.y, gPosition.x, 127, 40, 0.5, 0, 0, 12, stopNone, mttCascading); // 15.5 71
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 7, 3);
	moveToTargetSimpleAsync(15.5, 107, gPosition.y, gPosition.x, 127, 127, 0.5, 8, 30, 14, stopNone, mttSimple); // 13 107
	driveTimeout = nPgmTime + 1000;
	liftRaiseSimpleAsync(gLiftRaiseTarget[1], 127, 0);
	armSet(armToTarget, ARM_HORIZONTAL);
	DRIVE_AWAIT(skills, 7, 4);
	setDrive(55, 55);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 7, 5), false);
	setDrive(-10, -10);
	mobileSet(mobileTop, mfNone);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_HALFWAY, coneTimeout - 500, TID2(skills, 7, 6));
	moveToTargetSimpleAsync(13, 118, gPosition.y, gPosition.x, 45, 45, 0.5, 0, 0, 9, stopNone, mttCascading); // 13 118
	driveTimeout = nPgmTime + 1200;

	// 8
	DRIVE_AWAIT(skills, 8, 1);
	setDrive(-6, -6);
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 8, 2));
	stackSet(stackPickupGround, STACK_RAPID_CONFIG(sfDetach, 3)); // sfDetach 3
	gWall = true;
	coneTimeout = nPgmTime + 900;
	stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(skills, 8, 3));
	coneTimeout = nPgmTime + 1200;
	moveToTargetSimpleAsync(12.5, 128, gPosition.y, gPosition.x, 70, 0, 0.5, 0, 0, 9, stopSoft, mttSimple); // 12 12 | 55
	stackTimeoutUntil(stackPickupGround, coneTimeout, TID2(skills, 8, 4));
	coneTimeout = nPgmTime + 900;
	stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(skills, 8, 5));
	coneTimeout = nPgmTime + 1200;
	turnToTargetNewAlgAsync(23, 128, ccw, 0.27, 23, 12, true, 0);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 8, 6);
	moveToTargetSimpleAsync(23, 128, gPosition.y, gPosition.x, 55, 0, 0.5, 0, 0, 9, stopSoft, mttSimple);
	stackTimeoutUntil(stackPickupGround, coneTimeout, TID2(skills, 8, 7));
	coneTimeout = nPgmTime + 900;
	stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(skills, 8, 8));
	coneTimeout = nPgmTime + 1200;
	turnToTargetNewAlgAsync(24, 128, ccw, 0.25, 50, 10, true, 0);
	driveTimeout = nPgmTime + 1200;
	DRIVE_AWAIT(skills, 8, 10);
	moveToTargetSimpleAsync(24, 128, gPosition.y, gPosition.x, 127, 40, 0.5, 0, 0, 7, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 8, 11);
	moveToTargetSimpleAsync(41, 128, gPosition.y, gPosition.x, 127, 127, 0.5, 0, 0, 3, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 8, 12);
	moveToTargetSimpleAsync(95, 120, gPosition.y, gPosition.x, 127, 127, 1, 0, 0, 0, stopSoft, mttSimple);
	driveTimeout = nPgmTime + 3000;
	stackSet(stackTiltPrep, sfNone);
	timeoutWhileLessThanF(VEL_NONE, 0, &gPosition.y, 58, driveTimeout, TID2(skills, 8, 13));
	while (!gSensor[lsField].value)
	{
		if (TimedOut(driveTimeout, TID2(skills, 8, 14), false, VEL_NONE, 0, 0))
			goto noLine3;
		writeDebugStreamLine("%d", gSensor[lsField].rawValue);
		sleep(10);
	}
	_y = gPosition.y;
	_x = gPosition.x;
	normalize(_x, _y, -1, 212 - 9 * cos(gPosition.a));
	resetPositionFull(gPosition, _y, _x, gPosition.a);
noLine3:
	stackSet(stackTiltMobile, sfNone);
	coneTimeout = nPgmTime + 1200;
	stackTimeoutWhile(stackTiltMobile, coneTimeout, TID2(skills, 8, 15));
	DRIVE_AWAIT(skills, 8, 16);
	turnToAngleNewAlgAsync(0.25 * PI, cw, 0.15, 55, 10, true);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 8, 17);
	moveToTargetSimpleAsync(112, 136, gPosition.y, gPosition.x, 127, 0, 0.5, 0, 0, 16, stopNone, mttSimple);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 8, 18);

	// 9
	stackSet(stackDetachMobile, sfNone);
	coneTimeout = nPgmTime + 1000;
	stackTimeoutWhile(stackDetachMobile, coneTimeout, TID2(skills, 9, 1));
	//moveToTargetSimpleAsync(108, 108, gPosition.y, gPosition.x, -127, -40, 0.5, 0, 0, 0, stopNone, mttCascading);
	sweepTurnToTargetAsync(108, 108, -0.25 * PI, 11, cw, -127, true);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 9, 2);
	moveToTargetSimpleAsync(108, 108, gPosition.y, gPosition.x, -127, -127, 0.5, 0, 0, 0, stopNone, mttCascading);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 9, 3);
	turnToAngleNewAlgAsync(0.25 * PI, ccw, 0.3, 23, 12, true);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 9, 4);
	stackSet(stackClear, STACK_CLEAR_CONFIG(sfNone, mobileBottom, mfNone));
	coneTimeout = nPgmTime + 2500;
	driveAgainstStartingBar(55, 55, 15, 15, 2500);
	timeoutWhileGreaterThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(stack, 9, 5));
	resetPositionFull(gPosition, gPosition.y, gPosition.x, 0.25 * PI);

#endif
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
	//writeDebugStreamLine("%006d Start test: resetting", nPgmTime);
	//trackPositionTaskKill();
	//resetPositionFull(gPosition, 0, 0, PI);
	//resetVelocity(gVelocity, gPosition);
	//trackPositionTaskAsync();

	////liftSet(liftHoldDown);
	//moveToTargetSimple(48, 0, 0, 0, -127, 0, 0, 0, 0, stopNone, true);
	////mobileSet(mobileBottom, mfClear);
	//sweepTurnToTarget(65, 7, 0.25 * PI, 24, cw, -127, false);
	//moveToTargetSimple(89, 31, 65, 7, -127, 1.5, 0, 0, 0, stopSoft, true);

	//turnToAngleNewAlg(PI / 4, cw, 0.27, 23, 12, true);
	//sleep(1000);
	//writeDebugStreamLine("%.1f", radToDeg(gPosition.a));

	//sweepTurnToTarget(24, -24, PI / -2, 24, ccw, -127, false);
	//setDrive(7, -7);
	//sleep(150);
	//setDrive(0, 0);

	resetSonarYOnly(100, 500, START_BAR_RESET_INTERCEPT, -0.75 * PI, 600, 900, false);
}
