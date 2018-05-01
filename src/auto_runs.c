void selectAuto()
{
	if (gAutoLocked) return;
	updateSensorInput(autoPoti);
	int autoVal = gSensor[autoPoti].value - 2048;
	tAlliance oldAlliance = gAlliance;
	if (autoVal < 0) gAlliance = allianceBlue;
	else gAlliance = allianceRed;

	autoVal = abs(autoVal);
	int selected = autoVal * AUTO_OPTIONS_COUNT / 2048;
	if (oldAlliance != gAlliance || selected - gCurAuto != 1 || autoVal % (2048 / AUTO_OPTIONS_COUNT) > 100)
		gCurAuto = selected;
}

void runAuto()
{
#if SKILLS_ROUTE == 0
	selectAuto();
	if (LOGS) writeDebugStreamLine("Selected auto: %s %d", gAlliance == allianceBlue ? "blue" : "red", gCurAuto);

#ifdef AUTO_CUTOFF
	killAutoAsync(gAutoTime + AUTO_CUTOFF);
#endif

	if (gAlliance == allianceBlue)
	{
		switch (gCurAuto)
		{
			case 0: /* No auto */ break;
			case 1: /* 20 - autoloader */ auto20Right(gAutoCones); break;
			case 2: /* 20 - audience */ auto20Left(gAutoCones); break;
			case 3: /* 5 - autoloader */ auto5Right(gAutoCones); break;
			case 4: /* 5 - audience */ auto5Left(gAutoCones); break;
			case 5: /* 1s + block - autoloader */ autoSBRight(false, false); break;
			case 6: /* 1s + block - audience */ autoSBLeft(false, false); break;
			case 7: /* 1s + BOOM KAPOW - autoloader */ autoSBRight(false, true); break;
			case 8: /* 2s + block - audience */ autoSBLeft(true, false); break;
			case 9: /* Side block - autoloader */ autoSideBlockRight(1); break;
			case 10: /* Side block - audience */ autoSideBlockLeft(1); break;
		}
	}
	else
	{
		switch (gCurAuto)
		{
			case 0: /* Dumb block */ autoBlock(); break;
			case 1: /* 20 - autoloader */ auto20Left(gAutoCones); break;
			case 2: /* 20 - audience */ auto20Right(gAutoCones); break;
			case 3: /* 5 - autoloader */ auto5Left(gAutoCones); break;
			case 4: /* 5 - audience */ auto5Right(gAutoCones); break;
			case 5: /* 1s + block - autoloader */ autoSBLeft(false, false); break;
			case 6: /* 1s + block - audience */ autoSBRight(false, false); break;
			case 7: /* 1s + BOOM KAPOW - autoloader */ autoSBLeft(false, true); break;
			case 8: /* 2s + block - audience */ autoSBRight(true, false); break;
			case 9: /* Side block - autoloader */ autoSideBlockLeft(1); break;
			case 10: /* Side block - audience */ autoSideBlockRight(1); break;
		}
	}
#elif SKILLS_ROUTE < 0
	autoTest();
#else
	autoSkills(-1);
#endif
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

		if (LOGS) writeDebugStreamLine("%d %d", l, r);

		sleep(10);
	}

	playSound(soundShortBlip);

	lu *= 0.0393701 / (float) lc;
	ru *= 0.0393701 / (float) rc;

	if (LOGS) writeDebugStreamLine("Ultrasonic %f %f", lu, ru);

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
	if (LOGS) writeDebugStreamLine("%d Start reset Y", nPgmTime);
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
			if (LOGS) writeDebugStreamLine("%d MAX TIME %d", nPgmTime, maxTime);
			return false;
		}

		long l = gSensor[sonarL].value;
		if (l >= min && l <= max)
		{
			u += l;
			++c;
		}

		if (LOGS) writeDebugStreamLine("%d", l);

		sleep(10);
	}

	playSound(soundShortBlip);

	u *= 0.0393701 / (float) c;

	if (LOGS) writeDebugStreamLine("Ultrasonic (L) %f", u);

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
	if (LOGS) writeDebugStreamLine("%d Start reset X", nPgmTime);
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
			if (LOGS) writeDebugStreamLine("%d MAX TIME %d", nPgmTime, maxTime);
			return false;
		}

		long r = gSensor[sonarR].value;
		if (r >= min && r <= max)
		{
			u += r;
			++c;
		}

		if (LOGS) writeDebugStreamLine("%d", r);

		sleep(10);
	}

	playSound(soundShortBlip);

	u *= 0.0393701 / (float) c;

	if (LOGS) writeDebugStreamLine("Ultrasonic (R) %f", u);

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

#if SKILLS_ROUTE != 0

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
		if (LOGS) writeDebugStreamLine("%d | %d %d | %d %d", nPgmTime, gSensor[lsBarL].value, SensorValue[lsBarL], gSensor[lsBarR].value, SensorValue[lsBarR]);
		if (!(val & 1) && gSensor[lsBarL].value)
		{
			if (LOGS) writeDebugStreamLine("%d Saw left", nPgmTime);
			left = leftSlow;
			val |= 1;
		}
		if (!(val & 2) && gSensor[lsBarR].value)
		{
			if (LOGS) writeDebugStreamLine("%d Saw right", nPgmTime);
			right = rightSlow;
			val |= 2;
		}
		setDrive(left, right);
		sleep(10);
	}
}

#endif

void killAuto(unsigned long timeout)
{
	unsigned long st = nPgmTime;
	sleep(timeout - st);
	if (competitionState == autonomousState)
	{
		if (LOGS) writeDebugStreamLine("KILL AUTO");
		competitionSet(competitionNotRunning);
		autoSimpleSet(autoSimpleNotRunning);
		stackSet(stackNotRunning, sfNone);
		setDrive(0, 0);
	}
}

#define DRIVE_AWAIT(routine, major, minor) autoSimpleTimeoutUntil(autoSimpleNotRunning, driveTimeout, TID2(routine, major, minor))

void autoSkills(int segment)
{
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	float _x;
	float _y;

	gMobileCheckLift = true;

#if SKILLS_ROUTE == 1

	tStop(trackPositionTask);
	resetPositionFull(gPosition, 47, 14.25, 0);
	resetVelocity(gVelocity, gPosition);
	tStart(trackPositionTask);

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

#ifdef SKILLS_CUTOFF
	killAutoAsync(gAutoTime + SKILLS_CUTOFF);
#endif

	// 1
	moveToTargetSimpleAsync(107, 14.25, 47, 15, 70, 0, 0.5, 6, 55, 14, stopNone, mttProportional, false, false);
	driveTimeout = nPgmTime + 2000;
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, -10);
	sleep(300);
	mobileSet(mobileBottom, mfNone);
	DRIVE_AWAIT(skills, 1, 1);
	setDrive(55, 55);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 1, 2), false);
	setDrive(-7, -7);
	mobileSet(mobileTop, mfNone);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 1, 3));

	// 2
	moveToTargetSimpleAsync(71, 15, gPosition.y, gPosition.x, -127, 0, 0.5, 0, 0, 0, stopNone, mttCascading, false, false);
	driveTimeout = nPgmTime + 2000;
	stackSet(stackStack, sfDetach | sfNoResetAuto, true);
	DRIVE_AWAIT(skills, 2, 1);
	moveToTargetSimpleAsync(33, 33, gPosition.y, gPosition.x, -127, -90, 0.5, 0, 0, 0, stopSoft | stopHarsh, mttSimple, false);
	driveTimeout = nPgmTime + 2300;
	DRIVE_AWAIT(skills, 2, 2);
	turnToAngleNewAlgAsync(-0.75 * PI, ccw, 0.35, 26, 12, true, true, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 2, 3);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 8, _y, _x, 60, 0, 0, 0, 0, 0, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 2, 4);
	setDrive(45, 45);
	driveTimeout = nPgmTime + 1500;
	stackSet(stackClear, sfNoResetAuto);
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
	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, -7, _y, _x, -60, 0, 0, 0, 0, 0, stopHarsh, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	liftLowerSimpleAsync(LIFT_BOTTOM + 200, -127, 0);
	sleep(400);
	mobileSet(mobileBottom, mfNone);
	DRIVE_AWAIT(skills, 3, 1);
	turnToAngleNewAlgAsync(-0.25 * PI, cw, 0.32, 26, 12, false, true, false);
	driveTimeout = nPgmTime + 1500;
	liftSet(liftToBottom, -127);
	DRIVE_AWAIT(skills, 3, 2);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, 1, 0);
	sweepTurnToTargetAsync(54, 30, 35 * PI / 180, 16.5, cw, 127, true, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 3, 3);
	moveToTargetSimpleAsync(71, 47, gPosition.y, gPosition.x, 127, 127, 1, 0, 0, 13, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1000;
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, -20);
	DRIVE_AWAIT(skills, 3, 4);
	setDrive(40, 40);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 3, 5), false);
	mobileSet(mobileTop, mfClear);
	setDrive(-7, -7);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP, coneTimeout, TID2(skills, 3, 6));

	// 4
	turnToTargetNewAlgAsync(26, 8, cw, 0.65, 35, 20, true, true, 0, false);
	driveTimeout = nPgmTime + 2200;
	liftSet(liftToBottom, -127);
	DRIVE_AWAIT(skills, 4, 1);
	moveToTargetSimpleAsync(26, 8, gPosition.y, gPosition.x, 127, 0, 0.5, 20, 30, 15, stopNone, mttCascading, false);
	driveTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanF(VEL_NONE, 0, &gPosition.x, 23, driveTimeout, TID2(skils, 4, 2));
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, -20);
	DRIVE_AWAIT(skills, 4, 3);
	mobileSet(mobileBottom, mfNone);
	coneTimeout = nPgmTime + 2000;
	driveAgainstStartingBar(25, 25, 15, 15, 1500);
	sleep(700);
	if (segment > -1)
		return;
skip1:
	if (!resetSonarXOnly(100, 500, START_BAR_RESET_INTERCEPT, -0.75 * PI, 130, 330, false) && !resetSonarYOnly(100, 500, START_BAR_RESET_INTERCEPT, -0.75 * PI, 600, 900, false))
	{
		if (segment > -1)
			return;
		resetPositionFull(gPosition, gPosition.y, gPosition.x, -0.75 * PI);
	}
	timeoutWhileGreaterThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 4, 4));

	// 5
	moveToTargetDisSimpleAsync(gPosition.a, -8, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, 0, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 5, 1);
	turnToTargetNewAlgAsync(71, 47, cw, 0.5, 28, 11, false, true, 0, false);
	driveTimeout = nPgmTime + 1700;
	liftSet(liftToBottom, -70);
	DRIVE_AWAIT(skills, 5, 2);
	moveToTargetSimpleAsync(71, 47, gPosition.y, gPosition.x, 127, 0, 0.5, 0, 0, 5, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 5, 3);
	moveToTargetSimpleAsync(95, 71, gPosition.y, gPosition.x, 127, 127, 0.5, 0, 0, 14, stopNone, mttProportional, false);
	driveTimeout = nPgmTime + 2500;
	DRIVE_AWAIT(skills, 5, 4);
	setDrive(70, 70);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 5, 6), false);
	mobileSet(mobileUpToMiddle, mfClear);

	// 6
	moveToTargetSimpleAsync(136, 112, gPosition.y, gPosition.x, 127, 70, 1, 10, 30, 15, stopNone, mttCascading, false);
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 6, 2);
	mobileSet(mobileBottom, mfClear);
	coneTimeout = nPgmTime + 2000;
	driveAgainstStartingBar(20, 20, 15, 15, 1500);
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
	timeoutWhileGreaterThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 6, 3));

	// 7
	moveToTargetDisSimpleAsync(gPosition.a, -8, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, 0, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 7, 1);
	turnToTargetNewAlgAsync(128, 71, ccw, 0.29, 23, 12, false, true, 0, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 7, 2);
	moveToTargetSimpleAsync(128, 71, gPosition.y, gPosition.x, 70, 0, 0.5, 0, 0, 16, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 7, 3);
	moveToTargetSimpleAsync(130, 35, gPosition.y, gPosition.x, 127, 70, 0.5, 8, 55, 14, stopNone, mttProportional, false);
	driveTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanF(VEL_NONE, 0, &gPosition.x, 58, driveTimeout, TID2(skills, 7, 4));
	liftRaiseSimpleAsync(gLiftRaiseTarget[1], 127, -20);
	armSet(armToTarget, ARM_HORIZONTAL);
	DRIVE_AWAIT(skills, 7, 5);
	setDrive(55, 55);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 7, 6), false);
	setDrive(-7, -7);
	mobileSet(mobileTop, mfClear);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_HALFWAY, coneTimeout, TID2(skills, 7, 7));
	moveToTargetSimpleAsync(131, 23, gPosition.y, gPosition.x, 55, 55, 0.5, 0, 0, 9.5, stopNone, mttCascading, false);
	driveTimeout = nPgmTime + 1300;
	DRIVE_AWAIT(skills, 7, 8);
	setDrive(-6, -6);
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 7, 9));
	if (SKILLS_1_SAFE)
	{
		stackSet(stackPickupGround, sfStack | sfDetach | sfNoResetAuto, true);
	}
	else
	{
		stackSet(stackPickupGround, STACK_RAPID_CONFIG(sfDetach | sfNoResetAuto, 2), true);
		coneTimeout = nPgmTime + 1500;
		stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(skills, 7, 10), false);
		coneTimeout = nPgmTime + 1500;
		moveToTargetSimpleAsync(130, 13, gPosition.y, gPosition.x, 55, 30, 0.5, 0, 0, 9.5, stopHarsh, mttCascading, false);
		stackTimeoutUntil(stackPickupGround, coneTimeout, TID2(skills, 7, 11), false);
	}
	coneTimeout = nPgmTime + 1500;
	stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(skills, 7, 12), false);

	// 8
	moveToTargetSimpleAsync(127, 71, gPosition.y, gPosition.x, -127, 0, 0.5, 0, 0, 0, stopNone, mttCascading, false);
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 8, 1);
	moveToTargetSimpleAsync(105, 112, gPosition.y, gPosition.x, -127, -127, 0.5, 0, 0, 0, stopHarsh, mttCascading, false);
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 8, 2);
	turnToAngleNewAlgAsync(0.25 * PI, cw, 0.35, 28, 11, true, true, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 8, 3);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 226);
	moveToTargetDisSimpleAsync(0.25 * PI, 8, _y, _x, 60, 0, 0, 0, 0, 0, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 8, 4);
	setDrive(45, 45);
	driveTimeout = nPgmTime + 1500;
	sleep(300);
	timeoutWhileGreaterThanF(VEL_NONE, 0, &gVelocity.x, 0.05, driveTimeout, TID2(skills, 8, 5));
	setDrive(15, 15);
	stackSet(stackClear, sfNoResetAuto);
	coneTimeout = nPgmTime + 2000;
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
	stackTimeoutUntil(stackNotRunning, coneTimeout, TID2(skills, 8, 6));
	mobileSet(mobileDownToMiddle, mfClear);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(skills, 8, 7));
	sleep(300);

	// 9
	moveToTargetDisSimpleAsync(0.25 * PI, -9, _y, _x, -60, 0, 0, 0, 0, 0, stopHarsh, mttSimple, false);
	driveTimeout = nPgmTime + 1700;
	liftLowerSimpleAsync(LIFT_BOTTOM + 200, -127, 0);
	sleep(400);
	mobileSet(mobileBottom, mfNone);
	liftSet(liftToBottom, -127);
	DRIVE_AWAIT(skills, 9, 1);
	turnToTargetNewAlgAsync(60, 126, cw, 0.35, 26, 13, false, true, 0, false);
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 9, 2);
	moveToTargetSimpleAsync(60, 126, gPosition.y, gPosition.x, 127, 0, 1, 0, 0, 12, stopNone, mttProportional, false);
	driveTimeout = nPgmTime + 1700;
	DRIVE_AWAIT(skills, 9, 3);
	moveToTargetSimpleAsync(35, 128, gPosition.y, gPosition.x, 127, 127, 0.5, 0, 0, 14, stopNone, mttProportional, false);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileGreaterThanF(VEL_NONE, 0, &gPosition.y, 60, driveTimeout - 500, TID2(skills, 9, 4));
	liftRaiseSimpleAsync(gLiftRaiseTarget[1], 127, -20);
	armSet(armToTarget, ARM_HORIZONTAL);
	DRIVE_AWAIT(skills, 9, 5);
	setDrive(55, 55);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 9, 6), false);
	setDrive(-7, -7);
	mobileSet(mobileTop, mfNone);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_HALFWAY, coneTimeout, TID2(skills, 9, 7));
	moveToTargetSimpleAsync(23, 130, gPosition.y, gPosition.x, 55, 55, 0.5, 0, 0, 9.5, stopNone, mttCascading, false);
	driveTimeout = nPgmTime + 1200;
	DRIVE_AWAIT(skills, 9, 8);
	setDrive(-6, -6);
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 9, 9));
	stackSet(stackPickupGround, STACK_RAPID_CONFIG(sfClear | sfNoResetAuto, 2), true);
	coneTimeout = nPgmTime + 1500;
	stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(skills, 9, 10), false);
	coneTimeout = nPgmTime + 1500;
	moveToTargetSimpleAsync(13, 130, gPosition.y, gPosition.x, 55, 30, 0.5, 0, 0, 9.5, stopHarsh, mttCascading, false);
	stackTimeoutUntil(stackPickupGround, coneTimeout, TID2(skills, 9, 11), false);
	coneTimeout = nPgmTime + 1500;
	stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(skills, 9, 12), false);

	// 10
	moveToTargetSimpleAsync(60, 125, gPosition.y, gPosition.x, -127, 0, 1, 12, -90, 0, stopNone, mttProportional, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 10, 1);
	moveToTargetSimpleAsync(107, 107, gPosition.y, gPosition.x, -127, 0, 1, 0, 0, 6, stopNone, mttCascading, false);
	driveTimeout = nPgmTime + 2000;
	stackSet(stackClear, sfNoResetAuto, true);
	DRIVE_AWAIT(skills, 10, 2);
	turnToAngleNewAlgAsync(0.25 * PI, ccw, 0.4, 26, 11, true, true, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 10, 3);
	moveToTargetDisSimpleAsync(gPosition.a, 1.5, gPosition.y, gPosition.x, 80, 0, 0, 0, 0, 0, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 800;
	mobileSet(mobileBottom, mfClear);
	coneTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 10, 4);
	driveAgainstStartingBar(25, 25, 15, 15, 2000);
	timeoutWhileGreaterThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 10, 5));

	// 11
	moveToTargetDisSimpleAsync(gPosition.a, -8, gPosition.y, gPosition.x, -80, 0, 0, 0, 0, 0, stopHarsh, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	liftSet(liftToBottom, -127);
	DRIVE_AWAIT(skills, 11, 1);
	turnToAngleNewAlgAsync(0.75 * PI, cw, 0.5, 23, 12, false, true, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 11, 2);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, 1, 0);
	sweepTurnToTargetAsync(92, 112, 215 * PI / 180, 16, cw, 127, true, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 11, 3);
	moveToTargetSimpleAsync(72, 94, gPosition.y, gPosition.x, 127, 127, 0.5, 0, 0, 13, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 11, 4);
	setDrive(40, 40);
	driveTimeout = nPgmTime + 700;
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, -20);
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 11, 5), false);
	setDrive(-7, -7);
	mobileSet(mobileTop, mfNone);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 11, 6));

	// 12
	turnToTargetNewAlgAsync(112, 136, cw, 0.75, 35, 20, true, true, 0, false);
	driveTimeout = nPgmTime + 1500;
	liftSet(liftToBottom, -127);
	DRIVE_AWAIT(skills, 12, 1);
	moveToTargetSimpleAsync(112, 136, gPosition.y, gPosition.x, 127, 0, 0.5, 20, 30, 15, stopNone, mttCascading, false);
	driveTimeout = nPgmTime + 2000;
	timeoutWhileLessThanF(VEL_NONE, 0, &gPosition.x, 119, driveTimeout, TID2(skills, 12, 2), true, false);
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, -20);
	DRIVE_AWAIT(skills, 12, 3);
	mobileSet(mobileBottom, mfNone);
	coneTimeout = nPgmTime + 2000;
	driveAgainstStartingBar(25, 25, 15, 15, 1500);
	sleep(700);
	if (segment > -1)
		return;
skip4:
	if (!resetSonarXOnly(100, 500, START_BAR_RESET_INTERCEPT, 0.25 * PI, 150, 350, true))
	{
		if (segment > -1)
			return;
		resetPositionFull(gPosition, gPosition.y, gPosition.x, 0.25 * PI);
	}
	timeoutWhileGreaterThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 12, 4));

	// 13
	moveToTargetDisSimpleAsync(gPosition.a, -7, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, 0, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	liftSet(liftToBottom, -80);
	DRIVE_AWAIT(skills, 13, 1);
	turnToTargetNewAlgAsync(71, 95, cw, 0.55, 30, 20, false, true, 0, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 13, 2);
	moveToTargetSimpleAsync(71, 95, gPosition.y, gPosition.x, 127, 0, 0.5, 0, 0, 5, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 13, 3);
	moveToTargetSimpleAsync(46, 72, gPosition.y, gPosition.x, 127, 127, 1, 0, 0, 14, stopNone, mttProportional, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 13, 4);
	setDrive(70, 70);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 13, 5), false);
	mobileSet(mobileUpToMiddle, mfNone);

	// 14
	moveToTargetSimpleAsync(5, 30, gPosition.y, gPosition.x, 127, 70, 1, 10, 30, 15, stopNone, mttCascading, false);
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 14, 1);
	mobileSet(mobileBottom, mfNone);
	coneTimeout = nPgmTime + 2000;
	driveAgainstStartingBar(25, 25, 15, 15, 1500);
	sleep(200);
	if (segment > -1)
		return;
skip5:
	if (!resetSonarYOnly(100, 500, START_BAR_RESET_INTERCEPT, -0.75 * PI, 130, 330, false))
	{
		if (segment > -1)
			return;
		resetPositionFull(gPosition, gPosition.y, gPosition.x, -0.75 * PI);
	}
	timeoutWhileGreaterThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 14, 2));

	// 15
	moveToTargetDisSimpleAsync(gPosition.a, -6, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, 0, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 15, 1);
	turnToTargetNewAlgAsync(12, 106, ccw, 0.55, 23, 13, false, true, 0, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 15, 2);
	moveToTargetSimpleAsync(12, 106, gPosition.y, gPosition.x, 127, 0, 0.5, 0, 0, 14, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 2000;
	timeoutWhileLessThanF(VEL_NONE, 0, &gPosition.x, 71, driveTimeout, TID2(skills, 15, 3));
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, -20);
	DRIVE_AWAIT(skills, 15, 4);
	setDrive(55, 55);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 15, 5), false);
	setDrive(-7, -7);
	mobileSet(mobileUpToMiddle, mfNone);
	coneTimeout = nPgmTime + 2000;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(skills, 15, 6));

	// 16
	moveToTargetSimpleAsync(32, 32, gPosition.y, gPosition.x, -127, 0, 1, 0, 0, 0, stopNone, mttCascading, false);
	driveTimeout = nPgmTime + 2500;
	DRIVE_AWAIT(skills, 16, 1);
	turnToAngleNewAlgAsync(-0.75 * PI, cw, 0.7, 40, 30, true, true, false);
	driveTimeout = nPgmTime + 1500;
	coneTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 16, 2);
	moveToTargetDisSimpleAsync(gPosition.a, 5, gPosition.y, gPosition.x, 127, 0, 0, 0, 0, 0, stopHarsh, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	sleep(500);
	if (nPgmTime - gAutoTime < 59300)
	{
		mobileSet(mobileBottom, mfNone);
		timeoutWhileGreaterThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 16, 3));
	}
	DRIVE_AWAIT(skills, 16, 4);
	moveToTargetDisSimpleAsync(gPosition.a, -5, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, 0, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 16, 5);

#elif SKILLS_ROUTE == 2

	tStop(trackPositionTask);
	resetPositionFull(gPosition, 40, 16, PI / 4);
	resetVelocity(gVelocity, gPosition);
	tStart(trackPositionTask);

	segment = 2;

	switch (segment)
	{
	case 1:
		stackSet(stackClear, STACK_CLEAR_CONFIG(sfNoResetAuto, mobileDownToMiddle, mfNone));
		coneTimeout = nPgmTime + 2000;
		goto skip1;
	case 2:
		stackSet(stackClear, STACK_CLEAR_CONFIG(sfNoResetAuto, mobileDownToMiddle, mfNone));
		coneTimeout = nPgmTime + 2000;
		goto skip2;
	}

	// 1
	moveToTargetSimpleAsync(71, 47, 40, 16, 45, 0, 1, 0, 0, 14, stopNone, mttSimple, false);
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
	moveToTargetSimpleAsync(95, 71, gPosition.y, gPosition.x, 127, 70, 1, 6, 70, 14, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 2, 1));
	stackSet(stackStack, sfDetach | sfNoResetAuto);
	coneTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 2, 2);
	moveToTargetSimpleAsync(136, 112, gPosition.y, gPosition.x, 127, 70, 1, 42, 50, 0, stopNone, mttCascading, false);
	driveTimeout = nPgmTime + 2000;
	stackTimeoutUntil(stackNotRunning, coneTimeout, TID2(skills, 2, 3));
	stackSet(stackTiltPrep, sfTilt | sfNoResetAuto);
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
		if (LOGS) writeDebugStreamLine("%d", gSensor[lsField].rawValue);
		sleep(10);
	}
	_y = gPosition.y;
	_x = gPosition.x;
noLine1:
	autoSimpleSet(autoSimpleNotRunning);
	setDrive(0, 0);
	stackTimeoutUntil(stackNotRunning, coneTimeout, TID2(skills, 2, 6));
	moveToTargetDisSimpleAsync(0.25 * PI, 11, _y, _x, 127, 0, 0, 0, 0, 0, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 2, 7);
	stackSet(stackDetachMobile, sfNoResetAuto);
	coneTimeout = nPgmTime + 1000;
	stackTimeoutWhile(stackDetachMobile, coneTimeout, TID2(skills, 2, 8));

	// 3
	moveToTargetDisSimpleAsync(gPosition.a, -3, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, 0, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 3, 1);
	turnToTargetNewAlgAsync(108, 108, cw, 0.35, 23, 12, true, true, 0, false);
	driveTimeout = nPgmTime + 1500;
	armSet(armToTarget, ARM_PRESTACK - 500);
	liftSet(liftToBottom, -127);
	DRIVE_AWAIT(skills, 3, 2);
	moveToTargetSimpleAsync(108, 108, gPosition.y, gPosition.x, 127, 50, 0.5, 0, 0, 6, stopNone, mttCascading, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 3, 3);
	turnToAngleNewAlgAsync(0.25 * PI, ccw, 0.35, 23, 12, true, true, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 3, 4);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 226);
	moveToTargetDisSimpleAsync(0.25 * PI, 8, _y, _x, 60, 0, 0, 0, 0, 0, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 3, 5);
	setDrive(45, 45);
	driveTimeout = nPgmTime + 1500;
	sleep(300);
	timeoutWhileGreaterThanF(VEL_NONE, 0, &gVelocity.x, 0.05, driveTimeout, TID2(skills, 3, 6));
	setDrive(15, 15);
	stackSet(stackClear, STACK_CLEAR_CONFIG(sfNoResetAuto, mobileDownToMiddle, mfNone));
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
	moveToTargetDisSimpleAsync(0.25 * PI, -7, _y, _x, -60, 0, 0, 0, 0, 0, stopHarsh, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	liftLowerSimpleAsync(LIFT_BOTTOM + 200, -127, 0);
	DRIVE_AWAIT(skills, 4, 1);
	turnToAngleNewAlgAsync(0.75 * PI, cw, 0.27, 23, 12, false, true, false);
	driveTimeout = nPgmTime + 1500;
	mobileSet(mobileBottom, mfNone);
	DRIVE_AWAIT(skills, 4, 2);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, 1, 0);
	sweepTurnToTargetAsync(_y - 23, _x, 225 * PI / 180, 16.5, cw, 127, false, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 4, 3);
	moveToTargetSimpleAsync(71, 95, gPosition.y, gPosition.x, 127, 127, 1, 0, 0, 14, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1000;
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 80, 0);
	DRIVE_AWAIT(skills, 4, 4);
	setDrive(40, 40);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 4, 5), false);
	mobileSet(mobileTop, mfClear);

	// 5
	coneTimeout = nPgmTime + 2000;
	moveToTargetSimpleAsync(47, 71, gPosition.y, gPosition.x, 127, 70, 1, 6, 70, 14, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 5, 1));
	stackSet(stackTiltPrep, sfNoResetAuto);
	DRIVE_AWAIT(skills, 5, 2);
	moveToTargetSimpleAsync(7, 27, gPosition.y, gPosition.x, 127, 70, 1, 42, 50, 0, stopNone, mttCascading, false);
	driveTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanF(VEL_NONE, 0, &gPosition.y, 41, driveTimeout, TID2(skills, 5, 3));
	stackSet(stackTiltMobile, sfNoResetAuto);
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
		if (LOGS) writeDebugStreamLine("%d", gSensor[lsField].rawValue);
		sleep(10);
	}
	_y = gPosition.y;
	_x = gPosition.x;
noLine2:
	moveToTargetDisSimpleAsync(-0.75 * PI, 11, _y, _x, 127, 0, 0, 0, 0, 0, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 5, 5);
	stackSet(stackDetachMobile, sfNoResetAuto);
	coneTimeout = nPgmTime + 1000;
	stackTimeoutWhile(stackDetachMobile, coneTimeout, TID2(skills, 5, 6));

	// 6
	moveToTargetDisSimpleAsync(gPosition.a, -3, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, 0, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 6, 1);
	turnToTargetNewAlgAsync(34, 34, cw, 0.35, 23, 12, true, true, 0, false);
	driveTimeout = nPgmTime + 1500;
	armSet(armToTarget, ARM_PRESTACK - 500);
	liftSet(liftToBottom, -127);
	DRIVE_AWAIT(skills, 6, 2);
	moveToTargetSimpleAsync(33, 33, gPosition.y, gPosition.x, 127, 50, 0.5, 0, 0, 0, stopNone, mttCascading, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 6, 3);
	turnToAngleNewAlgAsync(-0.75 * PI, ccw, 0.35, 23, 12, true, true, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 6, 4);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	moveToTargetDisSimpleAsync(-0.75 * PI, 8, _y, _x, 60, 0, 0, 0, 0, 0, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 6, 5);
	setDrive(45, 45);
	driveTimeout = nPgmTime + 1500;
	sleep(300);
	timeoutWhileGreaterThanF(VEL_NONE, 0, &gVelocity.x, 0.05, driveTimeout, TID2(skills, 6, 6));
	setDrive(15, 15);
	stackSet(stackClear, STACK_CLEAR_CONFIG(sfNoResetAuto, mobileDownToMiddle, mfNone));
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
	moveToTargetDisSimpleAsync(-0.75 * PI, -7, _y, _x, -60, 0, 0, 0, 0, 0, stopHarsh, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	liftLowerSimpleAsync(LIFT_BOTTOM + 200, -127, 0);
	DRIVE_AWAIT(skills, 7, 1);
	turnToTargetNewAlgAsync(17, 71, ccw, 0.27, 23, 11, false, true, 0, false); // 15.5 71
	driveTimeout = nPgmTime + 1500;
	mobileSet(mobileBottom, mfNone);
	DRIVE_AWAIT(skills, 7, 2);
	moveToTargetSimpleAsync(17, 71, gPosition.y, gPosition.x, 127, 40, 0.5, 0, 0, 12, stopNone, mttCascading, false); // 15.5 71
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 7, 3);
	moveToTargetSimpleAsync(15.5, 107, gPosition.y, gPosition.x, 127, 127, 0.5, 8, 30, 14, stopNone, mttSimple, false); // 13 107
	driveTimeout = nPgmTime + 1500;
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
	moveToTargetSimpleAsync(13, 118, gPosition.y, gPosition.x, 45, 45, 0.5, 0, 0, 9, stopNone, mttCascading, false); // 13 118
	driveTimeout = nPgmTime + 1200;

	// 8
	DRIVE_AWAIT(skills, 8, 1);
	setDrive(-6, -6);
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 8, 2));
	stackSet(stackPickupGround, STACK_RAPID_CONFIG(sfReturn | sfNoResetAuto, 3)); // sfDetach 3
	gWall = true;
	coneTimeout = nPgmTime + 900;
	stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(skills, 8, 3));
	coneTimeout = nPgmTime + 1200;
	moveToTargetSimpleAsync(12.5, 128, gPosition.y, gPosition.x, 70, 0, 0.5, 0, 0, 9, stopSoft, mttSimple, false); // 12 12 | 55
	stackTimeoutUntil(stackPickupGround, coneTimeout, TID2(skills, 8, 4));
	coneTimeout = nPgmTime + 900;
	stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(skills, 8, 5));
	coneTimeout = nPgmTime + 1200;
	turnToTargetNewAlgAsync(23, 128, ccw, 0.27, 23, 9, true, true, 0, false);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 8, 6);
	moveToTargetSimpleAsync(23, 128, gPosition.y, gPosition.x, 55, 0, 0.5, 0, 0, 9, stopSoft, mttSimple, false);
	stackTimeoutUntil(stackPickupGround, coneTimeout, TID2(skills, 8, 7));
	coneTimeout = nPgmTime + 900;
	stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(skills, 8, 8));
	coneTimeout = nPgmTime + 1200;
	turnToTargetNewAlgAsync(24, 128, ccw, 0.25, 50, 10, true, false, 0, false);
	driveTimeout = nPgmTime + 1200;
	DRIVE_AWAIT(skills, 8, 10);
	moveToTargetSimpleAsync(24, 128, gPosition.y, gPosition.x, 127, 40, 0.5, 0, 0, 7, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 8, 11);
	moveToTargetSimpleAsync(41, 128, gPosition.y, gPosition.x, 127, 127, 0.5, 0, 0, 3, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 8, 12);
	moveToTargetSimpleAsync(95, 120, gPosition.y, gPosition.x, 127, 127, 1, 0, 0, 0, stopSoft, mttSimple, false);
	driveTimeout = nPgmTime + 3000;
	stackSet(stackTiltPrep, sfNoResetAuto);
	timeoutWhileLessThanF(VEL_NONE, 0, &gPosition.y, 58, driveTimeout, TID2(skills, 8, 13));
	while (!gSensor[lsField].value)
	{
		if (TimedOut(driveTimeout, TID2(skills, 8, 14), false, VEL_NONE, 0, 0))
			goto noLine3;
		if (LOGS) writeDebugStreamLine("%d", gSensor[lsField].rawValue);
		sleep(10);
	}
	_y = gPosition.y;
	_x = gPosition.x;
	normalize(_x, _y, -1, 212 - 9 * cos(gPosition.a));
	resetPositionFull(gPosition, _y, _x, gPosition.a);
noLine3:
	stackSet(stackTiltMobile, sfNoResetAuto);
	coneTimeout = nPgmTime + 1200;
	stackTimeoutWhile(stackTiltMobile, coneTimeout, TID2(skills, 8, 15));
	DRIVE_AWAIT(skills, 8, 16);
	turnToAngleNewAlgAsync(0.25 * PI, cw, 0.15, 55, 10, true, true, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 8, 17);
	moveToTargetSimpleAsync(112, 136, gPosition.y, gPosition.x, 127, 0, 0.5, 0, 0, 16, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 8, 18);

	// 9
	stackSet(stackDetachMobile, sfNoResetAuto);
	coneTimeout = nPgmTime + 1000;
	stackTimeoutWhile(stackDetachMobile, coneTimeout, TID2(skills, 9, 1));
	sweepTurnToTargetAsync(108, 108, -0.25 * PI, 11, cw, -127, true, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 9, 2);
	moveToTargetSimpleAsync(108, 108, gPosition.y, gPosition.x, -127, -127, 0.5, 0, 0, 2, stopNone, mttCascading, false);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 9, 3);
	turnToAngleNewAlgAsync(0.25 * PI, ccw, 0.3, 23, 12, true, true, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 9, 4);
	stackSet(stackClear, STACK_CLEAR_CONFIG(sfNoResetAuto, mobileBottom, mfNone));
	coneTimeout = nPgmTime + 2500;
	driveAgainstStartingBar(55, 55, 15, 15, 2500);
	resetPositionFull(gPosition, gPosition.y, gPosition.x, 0.25 * PI);
	timeoutWhileGreaterThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(stack, 9, 5));

	// 10
	moveToTargetDisSimpleAsync(0.25 * PI, -7, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, 0, stopHarsh, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 10, 1);
	turnToTargetNewAlgAsync(124.5, 83, ccw, 0.27, 23, 11, false, true, 0, false); // 15.5 71
	driveTimeout = nPgmTime + 1500;
	liftSet(liftToBottom, -127);
	DRIVE_AWAIT(skills, 10, 2);
	moveToTargetSimpleAsync(124.5, 83, gPosition.y, gPosition.x, 127, 40, 0.5, 0, 0, 12, stopNone, mttCascading, false); // 15.5 71
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(skills, 10, 3);
	moveToTargetSimpleAsync(126, 35, gPosition.y, gPosition.x, 127, 127, 0.5, 8, 30, 14, stopNone, mttSimple, false); // 13 107
	driveTimeout = nPgmTime + 1500;
	liftRaiseSimpleAsync(gLiftRaiseTarget[1], 127, 0);
	armSet(armToTarget, ARM_HORIZONTAL);
	DRIVE_AWAIT(skills, 10, 4);
	setDrive(55, 55);
	driveTimeout = nPgmTime + 700;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(skills, 10, 5), false);
	setDrive(-10, -10);
	mobileSet(mobileTop, mfNone);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_HALFWAY, coneTimeout - 500, TID2(skills, 10, 6));
	moveToTargetSimpleAsync(126.5, 23.5, gPosition.y, gPosition.x, 45, 45, 0.5, 0, 0, 9, stopNone, mttCascading, false); // 13 118
	driveTimeout = nPgmTime + 1200;

	// 11
	DRIVE_AWAIT(skills, 11, 1);
	setDrive(-6, -6);
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 11, 2));
	stackSet(stackPickupGround, STACK_RAPID_CONFIG(sfReturn | sfNoResetAuto, 3)); // sfDetach 3
	gWall = true;
	coneTimeout = nPgmTime + 900;
	stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(skills, 11, 3));
	coneTimeout = nPgmTime + 1200;
	moveToTargetSimpleAsync(127, 13.5, gPosition.y, gPosition.x, 70, 0, 0.5, 0, 0, 9, stopSoft, mttSimple, false); // 12 12 | 55
	stackTimeoutUntil(stackPickupGround, coneTimeout, TID2(skills, 11, 4));
	coneTimeout = nPgmTime + 900;
	stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(skills, 11, 5));
	coneTimeout = nPgmTime + 1200;
	turnToTargetNewAlgAsync(118.5, 13.5, ccw, 0.27, 23, 9, true, true, 0, false);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 11, 6);
	moveToTargetSimpleAsync(118.5, 13.5, gPosition.y, gPosition.x, 55, 0, 0.5, 0, 0, 9, stopSoft, mttSimple, false);
	stackTimeoutUntil(stackPickupGround, coneTimeout, TID2(skills, 11, 7));
	coneTimeout = nPgmTime + 900;
	stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(skills, 11, 8));
	coneTimeout = nPgmTime + 1200;
	turnToTargetNewAlgAsync(119.5, 13.5, ccw, 0.25, 50, 10, true, false, 0, false);
	driveTimeout = nPgmTime + 1200;
	DRIVE_AWAIT(skills, 11, 10);
	moveToTargetSimpleAsync(119.5, 13.5, gPosition.y, gPosition.x, 127, 40, 0.5, 0, 0, 7, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 11, 11);
	moveToTargetSimpleAsync(100.5, 13.5, gPosition.y, gPosition.x, 127, 127, 0.5, 0, 0, 3, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 11, 12);
	moveToTargetSimpleAsync(47, 41, gPosition.y, gPosition.x, 127, 127, 1, 0, 0, 0, stopSoft, mttSimple, false);
	driveTimeout = nPgmTime + 3000;
	stackSet(stackTiltPrep, sfNoResetAuto);
	timeoutWhileGreaterThanF(VEL_NONE, 0, &gPosition.y, 84, driveTimeout, TID2(skills, 11, 13));
	while (!gSensor[lsField].value)
	{
		if (TimedOut(driveTimeout, TID2(skills, 11, 14), false, VEL_NONE, 0, 0))
			goto noLine4;
		if (LOGS) writeDebugStreamLine("%d", gSensor[lsField].rawValue);
		sleep(10);
	}
	_y = gPosition.y;
	_x = gPosition.x;
	normalize(_x, _y, -1, 71 + 9 * cos(gPosition.a));
	resetPositionFull(gPosition, _y, _x, gPosition.a);
noLine4:
	stackSet(stackTiltMobile, sfNoResetAuto);
	coneTimeout = nPgmTime + 1200;
	stackTimeoutWhile(stackTiltMobile, coneTimeout, TID2(skills, 11, 15));
	DRIVE_AWAIT(skills, 11, 16);
	turnToAngleNewAlgAsync(-0.75 * PI, cw, 0.15, 55, 10, true, true, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 11, 17);
	moveToTargetSimpleAsync(28, 6, gPosition.y, gPosition.x, 127, 0, 0.5, 0, 0, 16, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 11, 18);

	// 12
	stackSet(stackDetachMobile, sfNoResetAuto);
	coneTimeout = nPgmTime + 1000;
	stackTimeoutWhile(stackDetachMobile, coneTimeout, TID2(skills, 12, 1));
	sweepTurnToTargetAsync(34, 34, 0.75 * PI, 11, cw, -127, true, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 12, 2);
	moveToTargetSimpleAsync(34, 34, gPosition.y, gPosition.x, -127, -127, 0.5, 0, 0, 2, stopNone, mttCascading, false);
	driveTimeout = nPgmTime + 1000;
	DRIVE_AWAIT(skills, 12, 3);
	turnToAngleNewAlgAsync(-0.75 * PI, ccw, 0.3, 23, 12, true, true, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 12, 4);
	stackSet(stackClear, STACK_CLEAR_CONFIG(sfNoResetAuto, mobileBottom, mfNone));
	coneTimeout = nPgmTime + 2500;
	driveAgainstStartingBar(55, 55, 15, 15, 2500);
	timeoutWhileGreaterThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(stack, 12, 5));
	moveToTargetDisSimpleAsync(gPosition.a, -7, gPosition.y, gPosition.x, -127, 0, 0, 0, 0, 0, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(skills, 12, 6);

#endif
}

#if SKILLS_ROUTE == 0

void resetLeft()
{
	if (gAutoStartSkills)
		resetPositionFull(gPosition, 47, 14.25, 0);
	else
		resetPositionFull(gPosition, 40, 17.25, 0.25 * PI);
	resetVelocity(gVelocity, gPosition);
	tStart(trackPositionTask);
}

void resetRight()
{
	if (gAutoStartSkills)
		resetPositionFull(gPosition, 14.25, 47, 0.5 * PI);
	else
		resetPositionFull(gPosition, 17.25, 40, 0.25 * PI);
	resetVelocity(gVelocity, gPosition);
	tStart(trackPositionTask);
}

void pickupMobileLeft(int cones)
{
	unsigned long driveTimeout;
	unsigned long coneTimeout;

	resetLeft();

	// 1
	if (gAutoStartSkills)
	{
		stackSet(stackClear, STACK_CLEAR_CONFIG(sfNoResetAuto, mobileBottom, mfNone));
		armSet(armHold);
	}
	else
	{
		moveToTargetDisSimpleAsync(0.25 * PI, 4, gPosition.y, gPosition.x, 127, 0, 0, 0, 0, 0, stopNone, mttSimple, false);
		driveTimeout = nPgmTime + 1000;
		liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, -20);
		armSet(armHold);
		DRIVE_AWAIT(pickupMobileLeft, 1, 1);
		turnToTargetNewAlgAsync(71, 15, ccw, 0.27, 23, 12, false, true, 0, false);
		driveTimeout = nPgmTime + 1500;
		mobileSet(mobileBottom, mfNone);
		DRIVE_AWAIT(pickupMobileLeft, 1, 2);
	}
	moveToTargetSimpleAsync(71, 15, gPosition.y, gPosition.x, 127, 50, 0.5, 0, 0, 14, stopNone, mttProportional, true);
	driveTimeout = nPgmTime + 3500;
	DRIVE_AWAIT(pickupMobileLeft, 1, 3);
	moveToTargetSimpleAsync(106, 13, gPosition.y, gPosition.x, 127, 127, 0.5, 0, 0, 14, stopNone, mttSimple, true);
	driveTimeout = nPgmTime + 3500;
	DRIVE_AWAIT(pickupMobileLeft, 1, 4);
	setDrive(40, 40);
	driveTimeout = nPgmTime + 1000;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(pickupMobileLeft, 1, 5), false);
	mobileSet(mobileTop, mfNone);
	coneTimeout = nPgmTime + 1500;
	setDrive(-7, -7);

	// 2
	if (cones > 0) {
		if (cones == 1) {
			timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(pickupMobileLeft, 2, 1));
		} else {
			if (cones > 4) cones = 4;
			timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_HALFWAY, coneTimeout, TID2(pickupMobileLeft, 2, 2));
			moveToTargetSimpleAsync(119, 12, gPosition.y, gPosition.x, 70, 70, 0.5, 0, 0, 9.5, stopHarsh, mttCascading, false);
		}
		timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(pickupMobileLeft, 2, 3));
		stackSet(stackStack, STACK_RAPID_CONFIG(sfDetach | sfNoResetAuto, cones), true);
		coneTimeout = nPgmTime + 1500;
		if (cones == 1) return;
		stackTimeoutUntil(stackPickupGround, coneTimeout, TID2(pickupMobileLeft, 2, 4), false);
		coneTimeout = nPgmTime + 1500;
		if (stackState != stackPickupGround) {
			if (gSensor[armPoti].value < ARM_RELEASE)
				stackSet(stackStack, sfNoResetAuto | sfDetach, true);
			return;
		}
		if (cones == 2) return;
		stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(pickupMobileLeft, 2, 5), false);
		coneTimeout = nPgmTime + 1500;
		if (stackState == stackNotRunning) {
			stackSet(stackStack, sfNoResetAuto | sfDetach, true);
			return;
		}
		moveToTargetSimpleAsync(129, 12, gPosition.y, gPosition.x, 70, 30, 0.5, 0, 0, 9.5, stopHarsh, mttCascading, false);
		stackTimeoutUntil(stackPickupGround, coneTimeout, TID2(pickupMobileLeft, 2, 6), false);
		coneTimeout = nPgmTime + 1500;
		if (stackState != stackPickupGround) {
			if (gSensor[armPoti].value < ARM_RELEASE)
				stackSet(stackStack, sfNoResetAuto | sfDetach, true);
			return;
		}
		if (cones == 3) return;
		sleep(400);
		moveToTargetSimpleAsync(138, 12, gPosition.y, gPosition.x, 55, 55, 0.5, 0, 0, 11, stopHarsh, mttSimple, true);
		stackTimeoutUntil(stackNotRunning, coneTimeout, TID2(pickupMobileLeft, 2, 7), false);
		if (stackState == stackNotRunning) {
			stackSet(stackStack, sfNoResetAuto | sfDetach, true);
			return;
		}
	} else {
		timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(pickupMobileLeft, 2, 11));
	}
}

void pickupMobileRight(int cones)
{
	unsigned long driveTimeout;
	unsigned long coneTimeout;

	resetRight();

	// 1
	if (gAutoStartSkills)
	{
		stackSet(stackClear, STACK_CLEAR_CONFIG(sfNoResetAuto, mobileBottom, mfNone));
		armSet(armHold);
	}
	else
	{
		moveToTargetDisSimpleAsync(0.25 * PI, 4, gPosition.y, gPosition.x, 127, 0, 0, 0, 0, 0, stopNone, mttSimple, false);
		driveTimeout = nPgmTime + 1000;
		liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, -20);
		armSet(armHold);
		DRIVE_AWAIT(pickupMobileRight, 1, 1);
		turnToTargetNewAlgAsync(15, 71, cw, 0.27, 23, 12, false, true, 0, false);
		driveTimeout = nPgmTime + 1500;
		mobileSet(mobileBottom, mfNone);
		DRIVE_AWAIT(pickupMobileRight, 1, 2);
	}
	moveToTargetSimpleAsync(15, 71, gPosition.y, gPosition.x, 127, 50, 0.5, 0, 0, 14, stopNone, mttProportional, true);
	driveTimeout = nPgmTime + 3500;
	DRIVE_AWAIT(pickupMobileRight, 1, 3);
	moveToTargetSimpleAsync(13, 106, gPosition.y, gPosition.x, 127, 127, 0.5, 0, 0, 14, stopNone, mttSimple, true);
	driveTimeout = nPgmTime + 3500;
	DRIVE_AWAIT(pickupMobileRight, 1, 4);
	setDrive(40, 40);
	driveTimeout = nPgmTime + 1000;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(pickupMobileRight, 1, 5), false);
	mobileSet(mobileTop, mfNone);
	coneTimeout = nPgmTime + 1500;
	setDrive(-7, -7);

	// 2
	if (cones > 0) {
		if (cones == 1) {
			timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(pickupMobileRight, 2, 1));
		} else {
			if (cones > 4) cones = 4;
			timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_HALFWAY, coneTimeout, TID2(pickupMobileRight, 2, 2));
			moveToTargetSimpleAsync(12, 119, gPosition.y, gPosition.x, 70, 70, 0.5, 0, 0, 9.5, stopHarsh, mttCascading, false);
		}
		timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(pickupMobileRight, 2, 3));
		stackSet(stackStack, STACK_RAPID_CONFIG(sfDetach | sfNoResetAuto, cones), true);
		coneTimeout = nPgmTime + 1500;
		if (cones == 1) return;
		stackTimeoutUntil(stackPickupGround, coneTimeout, TID2(pickupMobileRight, 2, 4), false);
		coneTimeout = nPgmTime + 1500;
		if (stackState != stackPickupGround) {
			if (gSensor[armPoti].value < ARM_RELEASE)
				stackSet(stackStack, sfNoResetAuto | sfDetach, true);
			return;
		}
		if (cones == 2) return;
		stackTimeoutWhile(stackPickupGround, coneTimeout, TID2(pickupMobileRight, 2, 5), false);
		coneTimeout = nPgmTime + 1500;
		if (stackState == stackNotRunning) {
			stackSet(stackStack, sfNoResetAuto | sfDetach, true);
			return;
		}
		moveToTargetSimpleAsync(12, 129, gPosition.y, gPosition.x, 70, 30, 0.5, 0, 0, 9.5, stopHarsh, mttCascading, false);
		stackTimeoutUntil(stackPickupGround, coneTimeout, TID2(pickupMobileRight, 2, 6), false);
		coneTimeout = nPgmTime + 1500;
		if (stackState != stackPickupGround) {
			if (gSensor[armPoti].value < ARM_RELEASE)
				stackSet(stackStack, sfNoResetAuto | sfDetach, true);
			return;
		}
		if (cones == 3) return;
		sleep(400);
		moveToTargetSimpleAsync(12, 138, gPosition.y, gPosition.x, 55, 55, 0.5, 0, 0, 11, stopHarsh, mttSimple, true);
		stackTimeoutUntil(stackNotRunning, coneTimeout, TID2(pickupMobileRight, 2, 7), false);
		if (stackState == stackNotRunning) {
			stackSet(stackStack, sfNoResetAuto | sfDetach, true);
			return;
		}
	} else {
		timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(pickupMobileRight, 2, 11));
	}
}

void score20()
{
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	float _x;
	float _y;

	// 1
	turnToAngleNewAlgAsync(-0.75 * PI, ch, 0.35, 26, 12, true, true, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(score20, 1, 1);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	moveToTargetDisSimpleAsync(-0.75 * PI, 8, _y, _x, 60, 0, 0, 0, 0, 0, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(score20, 1, 2);
	setDrive(45, 45);
	driveTimeout = nPgmTime + 1500;
	stackSet(stackClear, STACK_CLEAR_CONFIG(sfNoResetAuto, mobileDownToMiddle, mfNone));
	coneTimeout = nPgmTime + 2500;
	sleep(300);
	timeoutWhileLessThanF(VEL_NONE, 0, &gVelocity.y, -0.05, driveTimeout, TID2(score20, 1, 3));
	setDrive(15, 15);
	sleep(400);
	resetSonarFull(100, 300, -0.75 * PI, 180, 650, 180, 650, false);
	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(score20, 1, 4));
	liftLowerSimpleAsync(LIFT_BOTTOM + 200, -127, 0);
	sleep(300);

	// 2
	moveToTargetDisSimpleAsync(-0.75 * PI, -5, _y, _x, -60, 0, 0, 0, 0, 0, stopSoft, mttSimple, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(score20, 2, 1);

	gNumCones = 0;
}

void stationaryLeft(bool secondCone)
{
	unsigned long driveTimeout;
	unsigned long coneTimeout;

	resetLeft();
	mobileSet(mobileTop, mfNone);

	// 1
	stackSet(stackStationaryPrep, sfNoResetAuto);
	moveToTargetSimpleAsync(48, 48, gPosition.y, gPosition.x, 70, 0, 0.5, 0, 0, 5, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(stationaryLeft, 1, 1);
	setDrive(15, 15);
	sleep(500);
	stackSet(stackStationary, sfNoResetAuto);
	coneTimeout = nPgmTime + 2000;
	stackTimeoutUntil(stackNotRunning, coneTimeout, TID2(stationaryLeft, 1, 2));

	if (secondCone)
	{
		// 2
		moveToTargetDisSimpleAsync(gPosition.a, -6, gPosition.y, gPosition.x, -50, 0, 0, 0, 0, 0, stopNone, mttSimple, true);
		driveTimeout = nPgmTime + 3000;
		DRIVE_AWAIT(stationaryLeft, 2, 1);
		turnToTargetNewAlgAsync(71, 23, ccw, 0.27, 23, 12, false, true, 0, false);
		driveTimeout = nPgmTime + 1500;
		liftSet(liftToBottom, -127);
		DRIVE_AWAIT(stationaryLeft, 2, 2);
		moveToTargetSimpleAsync(71, 23, gPosition.y, gPosition.x, 127, 30, 0.5, 0, 0, 9.5, stopSoft, mttCascading, true);
		driveTimeout = nPgmTime + 3000;
		DRIVE_AWAIT(stationaryLeft, 2, 3);
		stackSet(stackPickupGround, sfNoResetAuto | sfNoResetArm, true);
		coneTimeout = nPgmTime + 1500;
		stackTimeoutUntil(stackNotRunning, coneTimeout, TID2(stationaryLeft, 2, 4));
		armRaiseSimpleAsync(ARM_TOP, 127, 0);

		// 3
		turnToTargetNewAlgAsync(48, 48, ch, 0.4, 26, 11, false, true, 0, false);
		driveTimeout = nPgmTime + 2000;
		DRIVE_AWAIT(stationaryLeft, 3, 1);
		moveToTargetSimpleAsync(48, 48, gPosition.y, gPosition.x, 80, 0, 0.5, 0, 0, 5, stopNone, mttSimple, true);
		driveTimeout = nPgmTime + 3000;
		stackSet(stackStationaryPrep, sfNoResetAuto);
		DRIVE_AWAIT(stationaryLeft, 3, 2);
		setDrive(15, 15);
		sleep(500);
		stackSet(stackStationary, sfNoResetAuto);
		coneTimeout = nPgmTime + 2000;
		stackTimeoutUntil(stackNotRunning, coneTimeout, TID2(stationaryLeft, 3, 3));
	}
}

void stationaryRight(bool secondCone)
{
	unsigned long driveTimeout;
	unsigned long coneTimeout;

	resetRight();
	mobileSet(mobileTop, mfNone);

	// 1
	stackSet(stackStationaryPrep, sfNoResetAuto);
	moveToTargetSimpleAsync(48, 48, gPosition.y, gPosition.x, 70, 0, 0.5, 0, 0, 5, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(stationaryRight, 1, 1);
	setDrive(15, 15);
	sleep(500);
	stackSet(stackStationary, sfNoResetAuto);
	coneTimeout = nPgmTime + 2000;
	stackTimeoutUntil(stackNotRunning, coneTimeout, TID2(stationaryRight, 1, 2));

	if (secondCone)
	{
		// 2
		moveToTargetDisSimpleAsync(gPosition.a, -6, gPosition.y, gPosition.x, -50, 0, 0, 0, 0, 0, stopNone, mttSimple, true);
		driveTimeout = nPgmTime + 3000;
		DRIVE_AWAIT(stationaryRight, 2, 1);
		turnToTargetNewAlgAsync(23, 71, ccw, 0.27, 23, 12, false, true, 0, false);
		driveTimeout = nPgmTime + 1500;
		liftSet(liftToBottom, -127);
		DRIVE_AWAIT(stationaryRight, 2, 2);
		moveToTargetSimpleAsync(23, 71, gPosition.y, gPosition.x, 127, 30, 0.5, 0, 0, 9.5, stopSoft, mttCascading, true);
		driveTimeout = nPgmTime + 3000;
		DRIVE_AWAIT(stationaryRight, 2, 3);
		stackSet(stackPickupGround, sfNoResetAuto, true);
		coneTimeout = nPgmTime + 1500;
		stackTimeoutUntil(stackNotRunning, coneTimeout, TID2(stationaryLeft, 2, 4));
		armRaiseSimpleAsync(ARM_TOP, 127, 0);

		// 3
		turnToTargetNewAlgAsync(48, 48, ch, 0.4, 26, 11, false, true, 0, false);
		driveTimeout = nPgmTime + 2000;
		DRIVE_AWAIT(stationaryLeft, 3, 1);
		moveToTargetSimpleAsync(48, 48, gPosition.y, gPosition.x, 80, 0, 0.5, 0, 0, 5, stopNone, mttSimple, true);
		driveTimeout = nPgmTime + 3000;
		stackSet(stackStationaryPrep, sfNoResetAuto);
		DRIVE_AWAIT(stationaryLeft, 3, 2);
		setDrive(15, 15);
		sleep(500);
		stackSet(stackStationary, sfNoResetAuto);
		coneTimeout = nPgmTime + 2000;
		stackTimeoutUntil(stackNotRunning, coneTimeout, TID2(stationaryLeft, 3, 3));
	}
}

void autoBlock()
{
	setDrive(-127, -127);
	sleep(2000);
	setDrive(7, 7);
}

void auto20Left(int cones)
{
	unsigned long driveTimeout;
	unsigned long coneTimeout;

	pickupMobileLeft(cones);

	// 1
	moveToTargetSimpleAsync(71, 15, gPosition.y, gPosition.x, -127, -40, 0.5, 0, 0, 0, stopNone, mttCascading, true);
	driveTimeout = nPgmTime + 4000;
	DRIVE_AWAIT(auto20Left, 1, 1);
	moveToTargetSimpleAsync(31, 31, gPosition.y, gPosition.x, -127, -127, 0.5, 0, 0, 0, stopHarsh, mttCascading, true);
	driveTimeout = nPgmTime + 3000;
	DRIVE_AWAIT(auto20Left, 1, 2);

	score20();

	// 2
	turnToAngleNewAlgAsync(-0.25 * PI, cw, 0.27, 23, 12, false, false, false);
	driveTimeout = nPgmTime + 1500;
	mobileSet(mobileTop, mfClear);
	DRIVE_AWAIT(auto20Left, 2, 1);
	moveToTargetDisSimpleAsync(gPosition.a, 2, gPosition.y, gPosition.x, 127, 0, 0, 0, 0, 0, stopNone, mttSimple, true);
	driveTimeout = nPgmTime + 3000;
	DRIVE_AWAIT(auto20Left, 2, 2);
	moveToTargetSimpleAsync(71, 47, gPosition.y, gPosition.x, 127, 127, 0.5, 0, 0, 7, stopNone, mttCascading, true);
	driveTimeout = nPgmTime + 5000;
	liftRaiseSimpleAsync(LIFT_LOADER_PICKUP, 127, -10);
	coneTimeout = nPgmTime + 1100;
	DRIVE_AWAIT(auto20Left, 2, 3);
	liftTimeoutWhile(liftRaiseSimpleState, coneTimeout, TID2(auto20Left, 2, 4));
	stackSet(stackTip, sfNone);
	coneTimeout = nPgmTime + 2000;
	stackTimeoutUntil(stackNotRunning, coneTimeout, TID2(auto20Left, 2, 5));
}

void auto20Right(int cones)
{
	unsigned long driveTimeout;
	unsigned long coneTimeout;

	pickupMobileRight(cones);

	// 1
	moveToTargetSimpleAsync(15, 71, gPosition.y, gPosition.x, -127, -40, 0.5, 0, 0, 0, stopNone, mttCascading, true);
	driveTimeout = nPgmTime + 4000;
	DRIVE_AWAIT(auto20Right, 1, 1);
	moveToTargetSimpleAsync(31, 31, gPosition.y, gPosition.x, -127, -127, 0.5, 0, 0, 0, stopHarsh, mttCascading, true);
	driveTimeout = nPgmTime + 3000;
	DRIVE_AWAIT(auto20Right, 1, 2);

	score20();

	// 2
	turnToAngleNewAlgAsync(0.75 * PI, ccw, 0.27, 23, 12, false, false, false);
	driveTimeout = nPgmTime + 1500;
	mobileSet(mobileTop, mfClear);
	DRIVE_AWAIT(auto20Right, 2, 1);
	moveToTargetDisSimpleAsync(gPosition.a, 2, gPosition.y, gPosition.x, 127, 0, 0, 0, 0, 0, stopNone, mttSimple, true);
	driveTimeout = nPgmTime + 3000;
	DRIVE_AWAIT(auto20Right, 2, 2);
	moveToTargetSimpleAsync(47, 71, gPosition.y, gPosition.x, 127, 127, 0.5, 0, 0, 7, stopNone, mttCascading, true);
	driveTimeout = nPgmTime + 5000;
	liftRaiseSimpleAsync(LIFT_LOADER_PICKUP, 127, -10);
	coneTimeout = nPgmTime + 1100;
	DRIVE_AWAIT(auto20Right, 2, 3);
	liftTimeoutWhile(liftRaiseSimpleState, coneTimeout, TID2(auto20Right, 2, 4));
	stackSet(stackTip, sfNone);
	coneTimeout = nPgmTime + 2000;
	stackTimeoutUntil(stackNotRunning, coneTimeout, TID2(auto20Right, 2, 5));
}

void auto5Left(int cones)
{
	unsigned long driveTimeout;
	unsigned long coneTimeout;

	pickupMobileLeft(cones);

	// 1
	moveToTargetSimpleAsync(71, 15, gPosition.y, gPosition.x, -127, -40, 0.5, 0, 0, 0, stopNone, mttCascading, true);
	driveTimeout = nPgmTime + 4000;
	DRIVE_AWAIT(auto5Left, 1, 1);
	moveToTargetSimpleAsync(60, 20, gPosition.y, gPosition.x, -127, -127, 0.5, 0, 0, 0, stopHarsh, mttCascading, true);
	driveTimeout = nPgmTime + 3000;
	DRIVE_AWAIT(auto5Left, 1, 2);
	turnToAngleNewAlgAsync(-0.75 * PI, ccw, 0.4, 26, 12, true, true, false);
	driveTimeout = nPgmTime + 1500;
	stackSet(stackClear, STACK_CLEAR_CONFIG(sfNoResetAuto, mobileBottomSlow, mfNone), true);
	coneTimeout = nPgmTime + 2500;
	DRIVE_AWAIT(auto5Left, 1, 3);
	moveToTargetDisSimpleAsync(-0.75 * PI, -2, gPosition.y, gPosition.x, -70, 0, 0, 0, 0, 0, stopSoft, mttSimple, true);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileGreaterThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_BOTTOM + 100, coneTimeout, TID2(auto5Left, 1, 4));
	DRIVE_AWAIT(auto5Left, 1, 5);
	moveToTargetSimpleAsync(121, 71, gPosition.y, gPosition.x, -127, -30, 0.5, 0, 0, 0, stopSoft, mttCascading, true);
	driveTimeout = nPgmTime + 5000;
	sleep(300);
	mobileSet(mobileTop, mfNone);
	sleep(700);
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	//DRIVE_AWAIT(auto5Left, 1, 6);
	autoSimpleTimeoutUntil(autoSimpleNotRunning, driveTimeout, TID2(auto5Left, 1, 6), false);
	setDrive(-10, -10);
	sleep(10000);
}

void auto5Right(int cones)
{
	unsigned long driveTimeout;
	unsigned long coneTimeout;

	pickupMobileRight(cones);

	// 1
	moveToTargetSimpleAsync(15, 71, gPosition.y, gPosition.x, -127, -40, 0.5, 0, 0, 0, stopNone, mttCascading, true);
	driveTimeout = nPgmTime + 4000;
	DRIVE_AWAIT(auto5Right, 1, 1);
	moveToTargetSimpleAsync(20, 60, gPosition.y, gPosition.x, -127, -127, 0.5, 0, 0, 0, stopHarsh, mttCascading, true);
	driveTimeout = nPgmTime + 3000;
	DRIVE_AWAIT(auto5Right, 1, 2);
	turnToAngleNewAlgAsync(-0.75 * PI, cw, 0.4, 26, 12, true, true, false);
	driveTimeout = nPgmTime + 1500;
	stackSet(stackClear, STACK_CLEAR_CONFIG(sfNoResetAuto, mobileBottomSlow, mfNone), true);
	coneTimeout = nPgmTime + 2500;
	DRIVE_AWAIT(auto5Right, 1, 3);
	moveToTargetDisSimpleAsync(-0.75 * PI, -2, gPosition.y, gPosition.x, -70, 0, 0, 0, 0, 0, stopSoft, mttSimple, true);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileGreaterThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_BOTTOM + 100, coneTimeout, TID2(auto5Right, 1, 4));
	DRIVE_AWAIT(auto5Right, 1, 5);
	moveToTargetSimpleAsync(71, 121, gPosition.y, gPosition.x, -127, -30, 0.5, 0, 0, 0, stopSoft, mttCascading, true);
	driveTimeout = nPgmTime + 5000;
	sleep(300);
	mobileSet(mobileTop, mfNone);
	sleep(700);
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	//DRIVE_AWAIT(auto5Right, 1, 6);
	autoSimpleTimeoutUntil(autoSimpleNotRunning, driveTimeout, TID2(auto5Right, 1, 6), false);
	setDrive(-10, -10);
	sleep(10000);
}

void autoSBLeft(bool secondCone, bool boomKapow)
{
	unsigned long driveTimeout;
	unsigned long coneTimeout;

	stationaryLeft(secondCone);

	// 1
	turnToTargetNewAlgAsync(71, 23, ch, 0.4, 40, 5, true, true, PI, false);
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(autoSBLeft, 1, 1);
	moveToTargetSimpleAsync(71, 23, gPosition.y, gPosition.x, -50, 0, 0.5, 0, 0, 18, stopNone, mttSimple, true);
	driveTimeout = nPgmTime + 5000;
	sleep(500);
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	DRIVE_AWAIT(autoSBLeft, 1, 2);
	moveToTargetSimpleAsync(119, 71, gPosition.y, gPosition.x, -127, -70, 0.5, 0, 0, 0, stopSoft, boomKapow ? mttCascading : mttSimple, true);
	driveTimeout = nPgmTime + 5000;
	//DRIVE_AWAIT(autoSBLeft, 1, 3);
	autoSimpleTimeoutUntil(autoSimpleNotRunning, driveTimeout, TID2(autoSBLeft, 1, 3), boomKapow);

	if (boomKapow)
	{
		// 2
		turnToTargetNewAlgAsync(129, 35, ch, 0.27, 23, 12, false, true, PI, false);
		driveTimeout = nPgmTime + 1500;
		DRIVE_AWAIT(autoSBLeft, 2, 1);
		moveToTargetSimpleAsync(129, 35, gPosition.y, gPosition.x, -127, -30, 0.5, 18, -127, 0, stopNone, mttCascading, true);
		driveTimeout = nPgmTime + 2500;
		DRIVE_AWAIT(autoSBLeft, 2, 2);
	}
	else
	{
		setDrive(-10, -10);
		sleep(10000);
	}
}

void autoSBRight(bool secondCone, bool boomKapow)
{
	unsigned long driveTimeout;
	unsigned long coneTimeout;

	stationaryRight(secondCone);

	// 1
	turnToTargetNewAlgAsync(23, 71, ch, 0.4, 40, 5, true, true, PI, false);
	driveTimeout = nPgmTime + 2000;
	DRIVE_AWAIT(autoSBRight, 1, 1);
	moveToTargetSimpleAsync(23, 71, gPosition.y, gPosition.x, -50, 0, 0.5, 0, 0, 18, stopNone, mttSimple, true);
	driveTimeout = nPgmTime + 5000;
	sleep(500);
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	DRIVE_AWAIT(autoSBRight, 1, 2);
	moveToTargetSimpleAsync(71, 119, gPosition.y, gPosition.x, -127, -70, 0.5, 0, 0, 0, stopSoft, boomKapow ? mttCascading : mttSimple, true);
	driveTimeout = nPgmTime + 5000;
	//DRIVE_AWAIT(autoSBRight, 1, 3);
	autoSimpleTimeoutUntil(autoSimpleNotRunning, driveTimeout, TID2(autoSBRight, 1, 3), boomKapow);

	if (boomKapow)
	{
		// 2
		turnToTargetNewAlgAsync(35, 129, ch, 0.27, 23, 12, false, true, PI, false);
		driveTimeout = nPgmTime + 1500;
		DRIVE_AWAIT(autoSBRight, 2, 1);
		moveToTargetSimpleAsync(35, 129, gPosition.y, gPosition.x, -127, -30, 0.5, 18, -127, 0, stopNone, mttCascading, true);
		driveTimeout = nPgmTime + 2500;
		DRIVE_AWAIT(autoSBRight, 2, 2);
	}
	else
	{
		setDrive(-10, -10);
		sleep(10000);
	}
}

void autoSideBlockLeft(int cones)
{
	unsigned long driveTimeout;
	unsigned long coneTimeout;

	pickupMobileLeft(cones);

	// 1
	turnToTargetNewAlgAsync(120, 60, ch, 0.45, 30, 12, true, false, PI, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(autoSideBlockLeft, 1, 1);
	moveToTargetSimpleAsync(120, 60, gPosition.y, gPosition.x, -127, -30, 0.5, 24, -127, 0, stopNone, mttCascading, true);
	driveTimeout = nPgmTime + 6000;
	stackSet(stackClear, STACK_CLEAR_CONFIG(sfNoResetAuto, mobileBottom, mfNone));
	DRIVE_AWAIT(autoSideBlockLeft, 1, 2);
	turnToTargetNewAlgAsync(95, 71, ch, 0.27, 23, 12, false, true, 0, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(autoSideBlockLeft, 1, 3);
	moveToTargetSimpleAsync(96, 71, gPosition.y, gPosition.x, 127, 30, 0.5, 0, 0, 14, stopNone, mttCascading, true);
	driveTimeout = nPgmTime + 5000;
	DRIVE_AWAIT(autoSideBlockLeft, 1, 4);
	setDrive(40, 40);
	driveTimeout = nPgmTime + 1000;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(autoSideBlockLeft, 1, 5), false);
	mobileSet(mobileTop, mfNone);
	coneTimeout = nPgmTime + 1500;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(autoSideBlockLeft, 1, 6));

	// 2
	turnToTargetNewAlgAsync(71, 45, ch, 0.45, 35, 12, true, true, 0.1 * PI, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(autoSideBlockLeft, 2, 1);
	moveToTargetSimpleAsync(71, 45, gPosition.y, gPosition.x, 127, 30, 0.5, 0, 0, 2, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 2000;
	liftRaiseSimpleAsync(LIFT_LOADER_PICKUP, 127, -10);
	DRIVE_AWAIT(autoSideBlockLeft, 2, 2);
	stackSet(stackTip, sfNone);
	coneTimeout = nPgmTime + 3000;
	stackTimeoutUntil(stackNotRunning, coneTimeout, TID2(autoSideBlockLeft, 2, 3));
}

void autoSideBlockRight(int cones)
{
	unsigned long driveTimeout;
	unsigned long coneTimeout;

	pickupMobileRight(cones);

	// 1
	turnToTargetNewAlgAsync(60, 120, ch, 0.45, 30, 12, true, false, PI, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(autoSideBlockRight, 1, 1);
	moveToTargetSimpleAsync(60, 120, gPosition.y, gPosition.x, -127, -30, 0.5, 24, -127, 0, stopNone, mttCascading, true);
	driveTimeout = nPgmTime + 6000;
	stackSet(stackClear, STACK_CLEAR_CONFIG(sfNoResetAuto, mobileBottom, mfNone));
	DRIVE_AWAIT(autoSideBlockRight, 1, 2);
	turnToTargetNewAlgAsync(71, 95, ch, 0.27, 23, 12, false, true, 0, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(autoSideBlockRight, 1, 3);
	moveToTargetSimpleAsync(71, 95, gPosition.y, gPosition.x, 127, 30, 0.5, 0, 0, 14, stopNone, mttCascading, true);
	driveTimeout = nPgmTime + 5000;
	DRIVE_AWAIT(autoSideBlockRight, 1, 4);
	setDrive(40, 40);
	driveTimeout = nPgmTime + 1000;
	timeoutWhileFalse((bool *) &gSensor[lsMobile].value, driveTimeout, TID2(autoSideBlockRight, 1, 5), false);
	mobileSet(mobileTop, mfNone);
	coneTimeout = nPgmTime + 1500;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(autoSideBlockRight, 1, 6));

	// 2
	turnToTargetNewAlgAsync(45, 71, ch, 0.45, 35, 12, true, true, 0.1 * PI, false);
	driveTimeout = nPgmTime + 1500;
	DRIVE_AWAIT(autoSideBlockRight, 2, 1);
	moveToTargetSimpleAsync(45, 71, gPosition.y, gPosition.x, 127, 30, 0.5, 0, 0, 2, stopNone, mttSimple, false);
	driveTimeout = nPgmTime + 2000;
	liftRaiseSimpleAsync(LIFT_LOADER_PICKUP, 127, -10);
	DRIVE_AWAIT(autoSideBlockRight, 2, 2);
	stackSet(stackTip, sfNone);
	coneTimeout = nPgmTime + 3000;
	stackTimeoutUntil(stackNotRunning, coneTimeout, TID2(autoSideBlockRight, 2, 3));
}

#elif SKILLS_ROUTE < 0

void autoTest()
{
	unsigned long coneTimeout;

	mobileSet(mobileTop, mfClear);
	coneTimeout = nPgmTime + 1500;
	timeoutWhileLessThanL(VEL_NONE, 0, &gSensor[mobilePoti].value, MOBILE_TOP - 100, coneTimeout, TID1(autoTest, 1));
	stackSet(stackStack, STACK_RAPID_CONFIG(sfReturn | sfNoResetAuto, 3), true);
	gWall = true;
	coneTimeout = nPgmTime + 5000;
	stackTimeoutUntil(stackNotRunning, coneTimeout, TID1(autoTest, 2));
}

#endif
