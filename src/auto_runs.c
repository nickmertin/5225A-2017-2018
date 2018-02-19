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

#define START_BAR_INTERCEPT 49.7
#define START_BAR_ROBOT_OFFSET 7.5
#define START_BAR_RESET_INTERCEPT (START_BAR_INTERCEPT + START_BAR_ROBOT_OFFSET / sqrt(2))

float calcYCoord(float u, float _a)
{
	const float x = 7.8;
	const float y = 1.5;
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
	time += nPgmTime;
	maxTime += nPgmTime;

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
	resetPositionFullRad(gPosition, y, x, a);
	resetVelocity(gVelocity, gPosition);
	return true;
}

bool resetSonarYOnly(unsigned long time, unsigned long maxTime, float xInt, float a, long min, long max, bool crossField)
{
	time += nPgmTime;
	maxTime += nPgmTime;

	playSound(soundShortBlip);

	float u = 0;
	int c = 0;
	while (nPgmTime < time || c < 10)
	{
		if (nPgmTime > maxTime)
			return false;

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
	resetPositionFullRad(gPosition, y, x, a);
	resetVelocity(gVelocity, gPosition);
	return true;
}

bool resetSonarXOnly(unsigned long time, unsigned long maxTime, float yInt, float a, long min, long max, bool crossField)
{
	time += nPgmTime;
	maxTime += nPgmTime;

	playSound(soundShortBlip);

	float u = 0;
	int c = 0;
	while (nPgmTime < time || c < 10)
	{
		if (nPgmTime > maxTime)
			return false;

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
	resetPositionFullRad(gPosition, y, x, a);
	resetVelocity(gVelocity, gPosition);
	return true;
}

//#define DIST_FROM

//void resetOnBase(float y, float x)
//{
//	resetPositionFullRad(gPosition, y -
//}

void killAuto(unsigned long timeout)
{
	sleep(timeout - nPgmTime);
	writeDebugStreamLine("KILL AUTO");
	competitionSet(competitionNotRunning);
	autoSimpleSet(autoSimpleNotRunning);
	stackSet(stackNotRunning);
	setDrive(0, 0);
}

NEW_ASYNC_VOID_1(killAuto, unsigned long);

void autoSkills()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	float _x;
	float _y;

	gMobileCheckLift = true;

#ifdef SKILLS_RESET_AT_START
	trackPositionTaskKill();
	resetPositionFull(gPosition, 16, 40, 45);
	//resetPositionFull(gPosition, 56, 8.25, 90);
	resetVelocity(gVelocity, gPosition);
	trackPositionTaskAsync();
#endif

	killAutoAsync(gAutoTime + 60000);

	//resetQuadratureEncoder(liftEnc);
	//goto skip;

	// 1
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 3000;
	moveToTargetSimple(47, 71, 16, 40, 40, 12, stopNone, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 1, 1));
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1000;
	timeoutWhileFalse((bool *) &gSensor[limMobile].value, driveTimeout, TID2(skills, 1, 2));
	setDrive(0, 0);
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 1, 3));

	// 2
	turnToTargetSimpleAsync(6.5, 28, cw, 127, 127, true, 0);
	driveTimeout = nPgmTime + 2300;
	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(skills, 2, 1));
	moveToTargetSimpleAsync(6.5, 28, gPosition.y, gPosition.x, 127, 0, 0, 17, stopSoft, true);
	driveTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanF(&gPosition.y, 34, driveTimeout, TID2(skills, 2, 2));
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 2, 3));
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 2, 4));

	// 3
	moveToTargetDisSimpleAsync(gPosition.a, -6, gPosition.y, gPosition.x, -70, 0, 0, 0, stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 3, 1));
	turnToTargetSimpleAsync(11, 107, ccw, 60, 60, false, 0);
	driveTimeout = nPgmTime + 2000;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(skills, 3, 2));
	moveToTargetSimpleAsync(11, 107, gPosition.y, gPosition.x, 127, 0, 0, 12, stopNone, true);
	driveTimeout = nPgmTime + 2500;
	timeoutWhileLessThanF(&gPosition.x, 72, driveTimeout, TID2(skills, 3, 3));
	liftRaiseSimpleAsync(gLiftRaiseTarget[0], 80, 0);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 3, 3));
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1000;
	timeoutWhileFalse((bool *) &gSensor[limMobile].value, driveTimeout, TID2(skills, 3, 4));
	setDrive(0, 0);
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 3, 5));

	// 4
	turnToTargetSimpleAsync(33, 27, ch, 40, 40, true, PI);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(skills, 4, 1));
	moveToTargetSimpleAsync(33, 27, gPosition.y, gPosition.x, -127, 30, -30, 4, stopHarsh, true);
	driveTimeout = nPgmTime + 2500;
	stackSet(stackStack, sfClear);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 4, 2));
	turnToAngleSimpleAsync(-135, cw, 127, 127, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToAngleSimpleState, driveTimeout, TID2(skills, 4, 3));

	// 5
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 10.5, _y, _x, 60, 0, 0, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 5, 1));
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gVelocity.y, -0.05, driveTimeout, TID2(skills, 5, 2));
	setDrive(7, 7);
	sleep(500);
	//skip:
	if (!resetSonarFull(100, 500, -0.75 * PI, 230, 470, 180, 650, false))
		resetPositionFull(gPosition, gPosition.y, gPosition.x, 225);
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(skills, 5, 3));
	mobileSet(mobileIdle);
	sleep(300);
	//return;

	// 6
	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, -7, _y, _x, -60, 0, 0, 0, stopHarsh, false);
	driveTimeout = nPgmTime + 1500;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	mobileSet(mobileBottom, 0);
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 6, 1));
	turnToTargetSimpleAsync(49, 25, cw, 80, 80, false, 0);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(skills, 6, 2));

	// 7
	moveToTargetSimpleAsync(49, 25, gPosition.y, gPosition.x, 90, 2, 30, -3, stopHarsh, true);
	driveTimeout = nPgmTime + 1800;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 7, 1));
	turnToTargetSimpleAsync(71, 47, cw, 80, 80, false, 0);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(skills, 7, 2));
	//return;

	// 8
	moveToTargetSimpleAsync(71, 47, gPosition.y, gPosition.x, 90, 0, 0, 12, stopNone, true);
	driveTimeout = nPgmTime + 2000;
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 80, 0);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 8, 1));
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1000;
	timeoutWhileFalse((bool *) &gSensor[limMobile].value, driveTimeout, TID2(skills, 8, 2));
	mobileSet(mobileTop, -1);
	setDrive(25, 25);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gPosition.y, 70, driveTimeout, TID2(skills, 8, 3));
	setDrive(-7, -7);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP, coneTimeout, TID2(skills, 8, 4));
	sleep(100);
	//return;

	// 9
	turnToTargetSimpleAsync(72, 59, cw, 60, 60, true, 0);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(skills, 9, 1));
	moveToTargetDisSimpleAsync(gPosition.a, 1, gPosition.y, gPosition.x, 40, 0, 0, 0, stopHarsh, false);
	driveTimeout = nPgmTime + 1000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 9, 2));
	stackSet(stackPickupGround, sfStack | sfClear);
	while (stackState == stackPickupGround) sleep(10);

	turnToTargetSimpleAsync(28, 6, cw, 127, 127, true, 0);
	driveTimeout = nPgmTime + 2200;
	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(skills, 9, 3));
	moveToTargetSimpleAsync(28, 6, gPosition.y, gPosition.x, 75, 20, 30, 13, stopSoft, true);
	driveTimeout = nPgmTime + 2000;
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 80, 0);
	timeoutWhileGreaterThanF(&gPosition.x, 22, driveTimeout, TID2(skils, 9, 4));
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 9, 5));
	setDrive(25, 25);
	driveTimeout = nPgmTime + 1000;
	timeoutWhileLessThanF(&gVelocity.y, -0.05, driveTimeout, TID2(skills, 9, 6));
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 9, 7));
	sleep(500);
	if (!resetSonarYOnly(100, 500, START_BAR_RESET_INTERCEPT, -0.75 * PI, 600, 900, false) && !resetSonarXOnly(100, 500, START_BAR_RESET_INTERCEPT, -0.75 * PI, 130, 330, false))
		resetPositionFull(gPosition, gPosition.y, gPosition.x, 225);
	//return;

	// 10
	moveToTargetDisSimpleAsync(gPosition.a, -7, gPosition.y, gPosition.x, -90, 0, 0, 0, stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 10, 1));
	turnToTargetSimpleAsync(78, 52, cw, 80, 80, false, 0);
	driveTimeout = nPgmTime + 2000;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(skills, 10, 2));
	moveToTargetSimpleAsync(78, 52, gPosition.y, gPosition.x, 127, 27, 30, 0, stopSoft, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 10, 3));
	turnToTargetSimpleAsync(95, 70, ch, 60, 60, false, 0);
	driveTimeout = nPgmTime + 1000;
	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(skills, 10, 4));
	moveToTargetSimpleAsync(95, 70, gPosition.y, gPosition.x, 127, 0, 0, 12, stopNone, true);
	driveTimeout = nPgmTime + 1500;
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 80, 0);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 10, 5));
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1000;
	timeoutWhileFalse((bool *) &gSensor[limMobile].value, driveTimeout, TID2(skills, 10, 6));
	setDrive(0, 0);
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 10, 7));
	turnToTargetSimpleAsync(109, 80, ch, 60, 60, true, 0);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(skills, 10, 8));
	moveToTargetSimpleAsync(109, 80, gPosition.y, gPosition.x, 90, 4, 30, 4, stopSoft, true);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 10, 9));
	turnToTargetSimpleAsync(136, 113, ch, 40, 40, true, 0);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(skills, 10, 10));
	moveToTargetSimpleAsync(136, 113, gPosition.y, gPosition.x, 127, 0, 0, 10, stopSoft, true);
	driveTimeout = nPgmTime + 2000;
	timeoutWhileLessThanF(&gPosition.y, 116, driveTimeout, TID2(skills, 10, 11));
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 10, 12));
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 10, 13));
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1000;
	timeoutWhileGreaterThanF(&gVelocity.x, 0.05, driveTimeout, TID2(skills, 10, 14));
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 10, 15));
	setDrive(30, 40);
	sleep(500);
	setDrive(15, 15);
	//skip:
	if (!resetSonarXOnly(100, 500, START_BAR_RESET_INTERCEPT, 0.25 * PI, 560, 830, true) && !resetSonarYOnly(100, 500, START_BAR_RESET_INTERCEPT, 0.25 * PI, 130, 330, true))
		resetPositionFull(gPosition, gPosition.y, gPosition.x, 45);
	setDrive(-90, 50);
	sleep(300);
	setDrive(0, 0);
	//return;

	// 11
	moveToTargetDisSimpleAsync(gPosition.a, -10, gPosition.y, gPosition.x, -90, 0, 0, 0, stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 11, 1));
	turnToTargetSimpleAsync(89, 118, cw, 80, 80, false, 0);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(skills, 11, 2));
	moveToTargetSimpleAsync(89, 118, gPosition.y, gPosition.x, 70, 24, 35, 0, stopHarsh, true);
	driveTimeout = nPgmTime + 1800;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 11, 3));
	turnToTargetSimpleAsync(70, 95, cw, 60, 60, false, 0);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(skills, 11, 4));

	// 12
	moveToTargetSimpleAsync(70, 95, gPosition.y, gPosition.x, 127, 0, 0, 12, stopNone, true);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 12, 1));
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1000;
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 80, 0);
	timeoutWhileFalse((bool *) &gSensor[limMobile].value, driveTimeout, TID2(skills, 12, 2));
	setDrive(0, 0);
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 12, 3));

	// 13
	turnToTargetSimpleAsync(112.5, 136.5, ccw, 60, 60, true, 0);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(skills, 13, 1));
	moveToTargetSimpleAsync(112.5, 136.5, gPosition.y, gPosition.x, 127, 25, 30, 13, stopSoft, true);
	driveTimeout = nPgmTime + 2000;
	timeoutWhileLessThanF(&gPosition.x, 116, driveTimeout, TID2(skills, 13, 2));
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 13, 3));
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 13, 11));
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileGreaterThanF(&gVelocity.y, 0.05, driveTimeout, TID2(skills, 13, 4));
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 13, 5));
	setDrive(30, 20);
	sleep(500);
	setDrive(15, 15);
	//skip:
	if (!resetSonarXOnly(100, 500, START_BAR_RESET_INTERCEPT, 0.25 * PI, 130, 380, true) && !resetSonarYOnly(100, 500, START_BAR_RESET_INTERCEPT, 0.25 * PI, 600, 900, true))
		resetPositionFull(gPosition, gPosition.y, gPosition.x, 45);
	//return;

	// 14
	moveToTargetDisSimpleAsync(gPosition.a, -7, gPosition.y, gPosition.x, -90, 0, 0, 0, stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 14, 1));
	turnToTargetSimpleAsync(35, 127, cw, 60, 60, false, 0);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(skills, 14, 2));
	//return;
	moveToTargetSimpleAsync(35, 127, gPosition.y, gPosition.x, 127, 24, 30, 12, stopNone, true);
	driveTimeout = nPgmTime + 1800;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 14, 3));
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1000;
	timeoutWhileFalse((bool *) &gSensor[limMobile].value, driveTimeout, TID2(skills, 14, 4));
	setDrive(0, 0);
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 14, 5));

	// 15
	turnToTargetSimpleAsync(109, 104, ch, 40, 40, true, PI);
	driveTimeout = nPgmTime + 1000;
	autoSimpleTimeoutWhile(turnToTargetSimpleState, driveTimeout, TID2(skills, 15, 1));
	moveToTargetSimpleAsync(109, 104, gPosition.y, gPosition.x, -127, 30, -30, 4, stopHarsh, true);
	driveTimeout = nPgmTime + 2500;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 15, 2));
	turnToAngleSimpleAsync(45, ccw, 60, 60, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToAngleSimpleState, driveTimeout, TID2(skills, 15, 3));

	// 16
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 226.5);
	moveToTargetDisSimpleAsync(0.25 * PI, 9, _y, _x, 60, 0, 0, 0, stopNone, false);
	driveTimeout = nPgmTime + 2000;
	while (autoSimpleState == moveToTargetDisSimpleState && nPgmTime < driveTimeout) sleep(10);
	//autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 16, 1));
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	while (gVelocity.x > 0.05 && nPgmTime < driveTimeout) sleep(10);
	//timeoutWhileGreaterThanF(&gVelocity.x, 0.05, driveTimeout, TID2(skills, 16, 2));

	setDrive(7, 7);
	sleep(300);
	//resetSonarFull(300, 0.25 * PI, true);
	//return;
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(skills, 5, 3));
	mobileSet(mobileIdle);
	sleep(300);

	// 17
	resetPositionFull(gPosition, gPosition.y, gPosition.x, 45);
	moveToTargetDisSimpleAsync(0.25 * PI, -3, _y, _x, -60, 0, 0, 0, stopNone, false);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 17, 1));
	turnToAngleSimpleAsync(-6, ccw, 127, 127, false);
	driveTimeout = nPgmTime + 2000;
	mobileSet(mobileBottom, 0);
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	autoSimpleTimeoutWhile(turnToAngleSimpleState, driveTimeout, TID2(skills, 17, 2));
	moveToTargetDisSimpleAsync(gPosition.a, -72, gPosition.y, gPosition.x, -127, 24, -127, 0, stopNone, true);
	driveTimeout = nPgmTime + 2500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 17, 3));
	playSound(soundDownwardTones);

	/*
	// 1
	moveToTargetSimpleAsync(71, 47, 40, 16, 127, 24, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, -10);
	coneTimeout = nPgmTime + 1000;
	timeoutWhileLessThanL(&gSensor[liftPoti].value, LIFT_MOBILE_THRESHOLD, coneTimeout, TID2(skills, 1, 2));
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 1000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 1, 3));
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 1, 4));
	setDrive(0, 0);
	moveToTargetSimpleAsync(71, 47, gPosition.y, gPosition.x, 127, 4, stopSoft, true);
	driveTimeout = nPgmTime + 1000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 1, 5));
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 1, 6));

	// 2
	turnToTargetNewAsync(39, 14, ch, 0);
	driveTimeout = nPgmTime + 5000;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	coneTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, TID2(skills, 2, 1));
	moveToTargetSimpleAsync(39, 14, gPosition.y, gPosition.x, 80, 2, stopSoft, true);
	driveTimeout = nPgmTime + 2000;
	liftTimeoutWhile(liftLowerSimpleState, coneTimeout, TID2(skills, 2, 2));
	armLowerSimpleAsync(ARM_PRESTACK - 100, -127, 20);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 2, 2));
	//coneAsync = clearArmAsync();
	//coneTimeout = nPgmTime + 2000;
	//await(coneAsync, coneTimeout, "skills 2-3");
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD + 10, 127, 0);
	coneTimeout = nPgmTime + 1500;
	liftTimeoutWhile(liftRaiseSimpleState, coneTimeout, TID2(skills, 2, 3));
	mobileSet(mobileBottom, 0);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 2, 4));

	// 3
	moveToTargetDisSimpleAsync(gPosition.a + PI, 8, gPosition.y, gPosition.x, -80, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 3, 1));
	turnToTargetNewAsync(16, 60, cw, PI);
	driveTimeout = nPgmTime + 3000;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	armRaiseSimpleAsync(ARM_TOP, 127, 0);
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, TID2(skills, 3, 2));
	moveToTargetSimpleAsync(16, 60, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 3, 3));
	turnToAngleNewAsync(0, cw);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToAngleNewState, driveTimeout, TID2(skills, 3, 4));
	resetBlueRightAsync();
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(resetBlueRightState, driveTimeout, TID2(skills, 3, 5));

	// 4
	moveToTargetDisSimpleAsync(0, 19, 0, gPosition.x, 80, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 4, 1));
	turnToTargetNewAsync(13, 107, cw, 0);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, TID2(skills, 4, 2));
	moveToTargetSimpleAsync(13, 107, gPosition.y, gPosition.x, 80, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 4, 3));
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 4, 4));

	// 5
	turnToTargetNewAsync(30, 40, ch, PI);
	driveTimeout = nPgmTime + 3000;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, TID2(skills, 5, 1));
	moveToTargetSimpleAsync(30, 40, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 5, 2));
	turnToAngleNewAsync(-135, cw);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToAngleNewState, driveTimeout, TID2(skills, 5, 3));
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 10.5, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 5, 4));
	//moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 15, _y, _x, 30, 0, stopNone, false);
	//driveTimeout = nPgmTime + 1500;
	//await(driveAsync, driveTimeout, "skills 5-5");
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gVelocity.y, -0.05, driveTimeout, TID2(skills, 5, 5));
	setDrive(7, 7);
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(skills, 5, 6));
	sleep(300);
	moveToTargetDisSimpleAsync(PI / 4, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	setMobile(-7);
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 5, 7));
	mobileSet(mobileManaged, 0);

	// 6
	turnToTargetNewAsync(16, 60, cw, PI);
	driveTimeout = nPgmTime + 3000;
	mobileSet(mobileBottom, 0);
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, TID2(skills, 6, 1));
	moveToTargetSimpleAsync(16, 60, gPosition.y, gPosition.x, -127, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 6, 2));
	turnToAngleNewAsync(0, cw);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToAngleNewState, driveTimeout, TID2(skills, 6, 3));
	resetBlueRightAsync();
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(resetBlueRightState, driveTimeout, TID2(skills, 6, 4));

	// 7
	moveToTargetDisSimpleAsync(0, 18, 0, gPosition.x, 80, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 7, 1));
	turnToTargetStupidAsync(47, 71, ch, 0);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToTargetStupidState, driveTimeout, TID2(skills, 7, 2));
	moveToTargetSimpleAsync(47, 71, gPosition.y, gPosition.x, 80, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 7, 3));
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 7, 4));

	// 8
	turnToTargetNewAsync(14, 39, ccw, 0);
	driveTimeout = nPgmTime + 3000;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, TID2(skills, 8, 1));
	moveToTargetSimpleAsync(14, 39, gPosition.y, gPosition.x, 80, 2, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 8, 2));
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(skills, 8, 3));

	// 9
	moveToTargetDisSimpleAsync(gPosition.a + PI, 7, gPosition.y, gPosition.x, -80, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 9, 1));
	turnToTargetNewAsync(52, 18, ccw, PI);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, TID2(skills, 9, 2));
	moveToTargetSimpleAsync(52, 18, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 9, 3));
	turnToAngleNewAsync(90, ccw);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToAngleNewState, driveTimeout, TID2(skills, 9, 4));
	setDrive(-30, -30);
	driveTimeout = nPgmTime + 1500;
	sleep(500);
	timeoutWhileLessThanF(&gVelocity.x, -0.1, driveTimeout, TID2(skills, 9, 5));
	setDrive(-7, -7);
	sleep(500);
	resetPositionFull(gPosition, gPosition.y, 8.25, 90);
	//return;
	skip:

	// 10
	moveToTargetDisSimpleAsync(PI / 2, 31, gPosition.y, 0, 50, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	//mobileSet(mobileBottom, -1);
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 10, 1));
	//return;
	turnToTargetCustomAsync(95, 71, ccw, 0, 25, 0.05);
	driveTimeout = nPgmTime + 8000;
	//configure(liftConfig, LIFT_BOTTOM, -127, 0);
	//liftSet(liftLowerSimpleState, &liftConfig);
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(skills, 10, 2));
	//return;
	moveToTargetSimpleAsync(94, 72, gPosition.y, gPosition.x, 80, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 10, 3));
	//return;
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(skills, 10, 4));

	// 11
	turnToTargetStupidAsync(112, 95, ch, 0);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToTargetStupidState, driveTimeout, TID2(skills, 11, 1));
	moveToTargetSimpleAsync(112, 95, gPosition.y, gPosition.x, 80, 2, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 11, 2));
	turnToTargetCustomAsync(110, 110, cw, 0, 45, 0.3);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(skills, 11, 3));
	moveToTargetSimpleAsync(110, 110, gPosition.y, gPosition.x, 80, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 11, 4));
	turnToAngleCustomAsync(45, ccw, 70, 0.2);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToAngleCustomState, driveTimeout, TID2(skills, 11, 5));
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 228);
	moveToTargetDisSimpleAsync(PI / 4, 12, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 11, 6));
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileGreaterThanF(&gVelocity.y, 0.05, driveTimeout, TID2(skills, 11, 7));
	setDrive(7, 7);
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(skills, 11, 8));
	sleep(300);
	moveToTargetDisSimpleAsync(-0.75 * PI, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(skills, 11, 9));
	mobileSet(mobileTop, -1);
	return;

	// 12
	turnToTargetNewAsync(23, 106, ccw, PI);
	driveTimeout = nPgmTime + 3000;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, TID2(skills, 12, 1));
	moveToTargetSimpleAsync(23, 106, gPosition.y, gPosition.x, 127, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(skills, 12, 2));
	*/
}
/*
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
	float _x;
	float _y;

	gMobileCheckLift = true;

	trackPositionTaskKill();
	resetPositionFull(gPosition, 40, 16, 45);
	resetVelocity(gVelocity, gPosition);
	trackPositionTaskAsync();

	coneTimeout = nPgmTime + 1400;

	// 1
	moveToTargetDisSimpleAsync(PI / 4, 10, gPosition.y, gPosition.x, 60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(rl20, 1, 1));
	turnToTargetCustomAsync(107, 13, ccw, 0, 40, 0.2);
	driveTimeout = nPgmTime + 3000;
	liftRaiseSimpleAsync(gLiftRaiseTarget[0], 70, -15);
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(rl20, 1, 3));
	moveToTargetSimpleAsync(107, 13, gPosition.y, gPosition.x, 127, 4, stopSoft, false);
	driveTimeout = nPgmTime + 3000;
	mobileSet(mobileBottom, -1);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rl20, 1, 4));
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(rl20, 1, 5));

	// 2
	turnToTargetStupidAsync(45, 25, ch, PI);
	driveTimeout = nPgmTime + 3000;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	coneTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(turnToTargetStupidState, driveTimeout, TID2(rl20, 2, 1));
	moveToTargetSimpleAsync(45, 25, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftLowerSimpleState, coneTimeout, TID2(rl20, 2, 2));
	armLowerSimpleAsync(ARM_PRESTACK - 100, -127, 20);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rl20, 2, 3));
	turnToAngleCustomAsync(-0.75 * PI, ccw, 70, 0.3);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToAngleCustomState, driveTimeout, TID2(rl20, 2, 4));
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 10.5, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(rl20, 2, 5));
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gVelocity.y, -0.05, driveTimeout, TID2(rl20, 2, 6));
	setDrive(7, 7);
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD + 10, 127, 0);
	coneTimeout = nPgmTime + 1500;
	liftTimeoutWhile(liftRaiseSimpleState, coneTimeout, TID2(rl20, 2, 7));
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(rl20, 2, 8));
	sleep(300);
	moveToTargetDisSimpleAsync(PI / 4, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	setMobile(-7);
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(rl20, 2, 9));
	mobileSet(mobileManaged, 0);
}

void auto20RedRight()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	float _x;
	float _y;

	gMobileCheckLift = true;

	trackPositionTaskKill();
	resetPositionFull(gPosition, 16, 40, 45);
	resetVelocity(gVelocity, gPosition);
	trackPositionTaskAsync();

	coneTimeout = nPgmTime + 1400;

	// 1
	moveToTargetDisSimpleAsync(PI / 4, 6, gPosition.y, gPosition.x, 60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(rr20, 1, 1));
	turnToTargetCustomAsync(14, 107, cw, 0, 40, 0.2);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(rr20, 1, 2));
	moveToTargetSimpleAsync(14, 107, gPosition.y, gPosition.x, 70, 4, stopSoft, false);
	driveTimeout = nPgmTime + 3000;
	liftRaiseSimpleAsync(gLiftRaiseTarget[0], 127, -15);
	mobileSet(mobileBottom, -1);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rr20, 1, 4));
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(rr20, 1, 5));

	// 2
	turnToTargetStupidAsync(25, 45, ch, PI);
	driveTimeout = nPgmTime + 3000;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	coneTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(turnToTargetStupidState, driveTimeout, TID2(rr20, 2, 1));
	moveToTargetSimpleAsync(25, 45, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftLowerSimpleState, coneTimeout, TID2(rr20, 2, 2));
	armLowerSimpleAsync(ARM_PRESTACK - 100, -127, 20);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rr20, 2, 3));
	turnToAngleCustomAsync(-0.75 * PI, cw, 70, 0.3);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToAngleCustomState, driveTimeout, TID2(rr20, 2, 4));
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 10.5, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(rr20, 2, 5));
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gVelocity.y, -0.05, driveTimeout, TID2(rr20, 2, 6));
	setDrive(7, 7);
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD + 10, 127, 0);
	coneTimeout = nPgmTime + 1500;
	liftTimeoutWhile(liftRaiseSimpleState, coneTimeout, TID2(rr20, 2, 7));
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(rr20, 2, 8));
	sleep(300);
	moveToTargetDisSimpleAsync(PI / 4, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	setMobile(-7);
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(rr20, 2, 9));
	mobileSet(mobileManaged, 0);
}

void auto20BlueRight()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	float _x;
	float _y;

	gMobileCheckLift = true;

	trackPositionTaskKill();
	resetPositionFull(gPosition, 16, 40, 45);
	resetVelocity(gVelocity, gPosition);
	trackPositionTaskAsync();

	coneTimeout = nPgmTime + 1400;

	// 1
	moveToTargetDisSimpleAsync(PI / 4, 10, gPosition.y, gPosition.x, 60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(br20, 1, 1));
	turnToTargetCustomAsync(14, 107, cw, 0, 40, 0.2);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(br20, 1, 2));
	moveToTargetSimpleAsync(14, 107, gPosition.y, gPosition.x, 70, 4, stopSoft, false);
	driveTimeout = nPgmTime + 3000;
	liftRaiseSimpleAsync(gLiftRaiseTarget[0], 127, -15);
	mobileSet(mobileBottom, -1);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(br20, 1, 4));
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(br20, 1, 5));

	// 2
	turnToTargetNewAsync(25, 45, ch, PI);
	driveTimeout = nPgmTime + 3000;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	coneTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, TID2(br20, 2, 1));
	moveToTargetSimpleAsync(25, 45, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftLowerSimpleState, coneTimeout, TID2(br20, 2, 2));
	armLowerSimpleAsync(ARM_PRESTACK - 100, -127, 20);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(br20, 2, 3));
	turnToAngleCustomAsync(-0.75 * PI, cw, 70, 0.3);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToAngleCustomState, driveTimeout, TID2(br20, 2, 4));
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 10.5, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(br20, 2, 5));
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gVelocity.y, -0.05, driveTimeout, TID2(br20, 2, 6));
	setDrive(7, 7);
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD + 10, 127, 0);
	coneTimeout = nPgmTime + 1500;
	liftTimeoutWhile(liftRaiseSimpleState, coneTimeout, TID2(br20, 2, 7));
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(br20, 2, 8));
	sleep(300);
	moveToTargetDisSimpleAsync(PI / 4, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	setMobile(-7);
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(br20, 2, 9));
	mobileSet(mobileManaged, 0);
}

void auto20BlueLeft()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	float _x;
	float _y;

	gMobileCheckLift = true;

	trackPositionTaskKill();
	resetPositionFull(gPosition, 40, 16, 45);
	resetVelocity(gVelocity, gPosition);
	trackPositionTaskAsync();

	coneTimeout = nPgmTime + 1400;

	// 1
	moveToTargetDisSimpleAsync(PI / 4, 6, gPosition.y, gPosition.x, 60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(bl20, 1, 1));
	turnToTargetCustomAsync(107, 14, ccw, 0, 40, 0.2);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(bl20, 1, 2));
	moveToTargetSimpleAsync(107, 14, gPosition.y, gPosition.x, 70, 4, stopSoft, false);
	driveTimeout = nPgmTime + 3000;
	liftRaiseSimpleAsync(gLiftRaiseTarget[0], 127, -15);
	mobileSet(mobileBottom, -1);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(bl20, 1, 4));
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(bl20, 1, 5));

	// 2
	turnToTargetStupidAsync(45, 25, ch, PI);
	driveTimeout = nPgmTime + 3000;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	coneTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(turnToTargetStupidState, driveTimeout, TID2(rl20, 2, 1));
	moveToTargetSimpleAsync(45, 25, gPosition.y, gPosition.x, -127, 4, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	liftTimeoutWhile(liftLowerSimpleState, coneTimeout, TID2(rl20, 2, 2));
	armLowerSimpleAsync(ARM_PRESTACK - 100, -127, 20);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rl20, 2, 3));
	turnToAngleCustomAsync(-0.75 * PI, ccw, 70, 0.3);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToAngleCustomState, driveTimeout, TID2(rl20, 2, 4));
	_x = gPosition.x;
	_y = gPosition.y;
	normalize(_x, _y, -1, 56);
	moveToTargetDisSimpleAsync(-3.0 / 4 * PI, 10.5, _y, _x, 60, 0, stopNone, false);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(rl20, 2, 5));
	setDrive(30, 30);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gVelocity.y, -0.05, driveTimeout, TID2(rl20, 2, 6));
	setDrive(7, 7);
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD + 10, 127, 0);
	coneTimeout = nPgmTime + 1500;
	liftTimeoutWhile(liftRaiseSimpleState, coneTimeout, TID2(rl20, 2, 7));
	mobileSet(mobileDownToMiddle, -1);
	coneTimeout = nPgmTime + 1500;
	mobileTimeoutUntil(mobileMiddle, coneTimeout, TID2(rl20, 2, 8));
	sleep(300);
	moveToTargetDisSimpleAsync(PI / 4, 17, gPosition.y, gPosition.x, -60, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	setMobile(-7);
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(rl20, 2, 9));
	mobileSet(mobileManaged, 0);
}

void stationaryLeftCore(bool goToWall = true)
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	float _x;
	float _y;

	gMobileCheckLift = true;

	trackPositionTaskKill();
	resetPositionFull(gPosition, 40, 16, 45);
	resetVelocity(gVelocity, gPosition);
	trackPositionTaskAsync();

	coneTimeout = nPgmTime + 1400;

	// 1
	moveToTargetSimpleAsync(47, 23, gPosition.y, gPosition.x, 70, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(ls, 1, 1));
	turnToTargetCustomAsync(47, 47, cw, 0, 60, 0.3);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(ls, 1, 2));
	setDrive(40, 40);
	driveTimeout = nPgmTime + 2000;
	liftRaiseSimpleAsync(83, 127, -15);
	armLowerSimpleAsync(2600, -127, 25);
	sleep(driveTimeout - 1500 - nPgmTime);
	timeoutWhileGreaterThanF(&gVelocity.x, 0.1, driveTimeout, TID2(ls, 1, 4));
	setDrive(15, 15);
	liftLowerSimpleAsync(47, -127, 25);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[liftPoti].value, 47, coneTimeout, TID2(ls, 1, 5));
	armLowerSimpleAsync(ARM_HORIZONTAL, -127, 25);
	coneTimeout = nPgmTime + 1500;
	timeoutWhileGreaterThanL(&gSensor[armPoti].value, ARM_HORIZONTAL + 200, coneTimeout, TID2(ls, 1, 6));

	// 2
	liftRaiseSimpleAsync(82, 127, -15);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[liftPoti].value, 75, coneTimeout, TID2(ls, 2, 1));
	armRaiseSimpleAsync(ARM_TOP, 127, -15);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[armPoti].value, ARM_TOP - 200, coneTimeout, TID2(ls, 2, 2));
	if (goToWall)
	{
		turnToTargetCustomAsync(52, 18, cw, PI, 45, 0.05);
		driveTimeout = nPgmTime + 3000;
		autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(ls, 2, 3));
		moveToTargetSimpleAsync(52, 18, gPosition.y, gPosition.x, -70, 0, stopSoft | stopHarsh, true);
		driveTimeout = nPgmTime + 2000;
		coneTimeout = nPgmTime + 1500;
		timeoutWhileLessThanF(&gPosition.y, 50, coneTimeout, TID2(ls, 2, 4));
		liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
		coneTimeout = nPgmTime + 2000;
		autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(ls, 2, 5));
		turnToAngleStupidAsync(PI / 2, ch);
		driveTimeout = nPgmTime + 3000;
		autoSimpleTimeoutWhile(turnToAngleStupidState, driveTimeout, TID2(ls, 2, 6));
		resetBlueLeftAsync();
		driveTimeout = nPgmTime + 3000;
		autoSimpleTimeoutWhile(resetBlueLeftState, driveTimeout, TID2(ls, 2, 7));
	}
}

void stationaryRightCore(bool goToWall = true)
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	float _x;
	float _y;

	gMobileCheckLift = true;

	trackPositionTaskKill();
	resetPositionFull(gPosition, 16, 40, 45);
	resetVelocity(gVelocity, gPosition);
	trackPositionTaskAsync();

	coneTimeout = nPgmTime + 1400;

	// 1
	moveToTargetSimpleAsync(23, 47, gPosition.y, gPosition.x, 70, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rs, 1, 1));
	turnToTargetCustomAsync(47, 47, ccw, 0, 60, 0.3);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(rs, 1, 2));
	setDrive(40, 40);
	driveTimeout = nPgmTime + 2000;
	liftRaiseSimpleAsync(83, 127, -15);
	armLowerSimpleAsync(2600, -127, 25);
	sleep(driveTimeout - 1500 - nPgmTime);
	timeoutWhileGreaterThanF(&gVelocity.y, 0.1, driveTimeout, TID2(rs, 1, 4));
	setDrive(15, 15);
	liftLowerSimpleAsync(52, -127, 25);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[liftPoti].value, 52, coneTimeout, TID2(rs, 1, 5));
	armLowerSimpleAsync(ARM_HORIZONTAL, -127, 25);
	coneTimeout = nPgmTime + 1500;
	timeoutWhileGreaterThanL(&gSensor[armPoti].value, ARM_HORIZONTAL + 200, coneTimeout, TID2(rs, 1, 6));

	// 2
	liftRaiseSimpleAsync(82, 127, -15);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[liftPoti].value, 75, coneTimeout, TID2(rs, 1, 7));
	armRaiseSimpleAsync(ARM_TOP, 127, -15);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[armPoti].value, ARM_TOP - 200, coneTimeout, TID2(rs, 1, 8));
	if (goToWall)
	{
		turnToTargetCustomAsync(18, 52, ccw, PI, 45, 0.05);
		driveTimeout = nPgmTime + 3000;
		autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(rs, 2, 3));
		moveToTargetSimpleAsync(18, 52, gPosition.y, gPosition.x, -70, 0, stopSoft | stopHarsh, true);
		driveTimeout = nPgmTime + 2000;
		coneTimeout = nPgmTime + 1500;
		timeoutWhileLessThanF(&gPosition.x, 50, coneTimeout, TID2(rs, 2, 4));
		liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
		coneTimeout = nPgmTime + 2000;
		autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rs, 2, 5));
		turnToAngleStupidAsync(0, ch);
		driveTimeout = nPgmTime + 3000;
		autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rs, 2, 6));
		resetBlueRightAsync();
		driveTimeout = nPgmTime + 3000;
		autoSimpleTimeoutWhile(resetBlueRightState, driveTimeout, TID2(rs, 2, 7));
	}
}

void autoStationaryLeftBlock()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	float _x;
	float _y;

	stationaryLeftCore(false);

	// 2
	turnToTargetCustomAsync(71, 23, ch, PI, 70, 0.2);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(ls+b, 2, 1));
	moveToTargetSimpleAsync(71, 23, gPosition.y, gPosition.x, -70, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gPosition.y, 50, driveTimeout, TID2(ls+b, 2, 2));
	liftLowerSimpleAsync(LIFT_BOTTOM , -127, 0);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(ls+b, 2, 3));
	turnToTargetCustomAsync(119, 71, ch, PI, 60, 0.3);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(ls+b, 2, 4));
	moveToTargetSimpleAsync(119, 71, gPosition.y, gPosition.x, -127, 0, stopSoft, false);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(ls+b, 2, 5));
}

void autoStationaryRightBlock()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	float _x;
	float _y;

	stationaryRightCore(false);

	// 2
	turnToTargetCustomAsync(23, 71, ch, PI, 70, 0.2);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(rs+b, 2, 1));
	moveToTargetSimpleAsync(23, 71, gPosition.y, gPosition.x, -70, 6, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 1500;
	timeoutWhileLessThanF(&gPosition.x, 50, driveTimeout, TID2(rs+b, 2, 2));
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rs+b, 2, 3));
	turnToTargetCustomAsync(71, 119, ch, PI, 60, 0.3);
	driveTimeout = nPgmTime + 1500;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(rs+b, 2, 4));
	moveToTargetSimpleAsync(71, 119, gPosition.y, gPosition.x, -127, 0, stopSoft, false);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rs+b, 2, 5));
}

void autoStationaryLeft5()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	float _x;
	float _y;

	stationaryLeftCore();

	// 3
	moveToTargetDisSimpleAsync(PI / 2, 19, gPosition.y, 0, 50, 0, stopHarsh, false);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(ls+5, 3, 1));
	turnToTargetCustomAsync(107, 14, ccw, 0, 45, 0.15);
	driveTimeout = nPgmTime + 3000;
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, -10);
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(ls+5, 3, 2));
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 1000, coneTimeout, TID2(ls+5, 3, 3));
	moveToTargetSimpleAsync(107, 14, gPosition.y, gPosition.x, 127, 6, stopSoft, true);
	driveTimeout = nPgmTime + 3000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(ls+5, 3, 4));
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(ls+5, 3, 5));
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(ls+5, 3, 6));

	// 4
	turnToTargetCustomAsync(62, 18, ch, PI, 45, 0.05);
	driveTimeout = nPgmTime + 3000;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(ls+5, 4, 1));
	moveToTargetDisSimpleAsync(gPosition.a, -36, gPosition.y, gPosition.x, -127, 0, stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(ls+5, 4, 2));
	turnToAngleCustomAsync(PI / -2, ccw, 127, 0.2);
	driveTimeout = nPgmTime + 3000;
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, -10);
	autoSimpleTimeoutWhile(turnToAngleCustomState, driveTimeout, TID2(ls+5, 4, 3));
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(ls+5, 4, 4));
	moveToTargetDisSimpleAsync(gPosition.a, -8, gPosition.y, gPosition.x, -60, 0, stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(ls+5, 4, 5));
}

void autoStationaryRight5()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	float _x;
	float _y;

	stationaryRightCore();

	// 3
	moveToTargetDisSimpleAsync(0, 19, 0, gPosition.x, 50, 0, stopHarsh, false);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(rs+5, 3, 1));
	turnToTargetNewAsync(14, 107, cw, 0);
	driveTimeout = nPgmTime + 3000;
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, -10);
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, TID2(rs+5, 3, 2));
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 1000, coneTimeout, TID2(rs+5, 3, 3));
	moveToTargetSimpleAsync(14, 107, gPosition.y, gPosition.x, 127, 6, stopSoft, true);
	driveTimeout = nPgmTime + 3000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(rs+5, 3, 4));
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rs+5, 3, 5));
	mobileSet(mobileTop, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[mobilePoti].value, MOBILE_TOP - 200, coneTimeout, TID2(rs+5, 3, 5));

	// 4
	turnToTargetCustomAsync(18, 62, ch, PI, 45, 0.05);
	driveTimeout = nPgmTime + 3000;
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(rs+5, 4, 1));
	moveToTargetDisSimpleAsync(gPosition.a, -36, gPosition.y, gPosition.x, -127, 0, stopHarsh, true);
	driveTimeout = nPgmTime + 3000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(rs+5, 4, 2));
	turnToAngleCustomAsync(-PI, cw, 127, 0.2);
	driveTimeout = nPgmTime + 3000;
	liftRaiseSimpleAsync(LIFT_MOBILE_THRESHOLD, 127, -10);
	autoSimpleTimeoutWhile(turnToAngleCustomState, driveTimeout, TID2(rs+5, 4, 3));
	mobileSet(mobileBottom, -1);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[mobilePoti].value, MOBILE_BOTTOM + 200, coneTimeout, TID2(rs+5, 4, 4));
	moveToTargetDisSimpleAsync(gPosition.a, -8, gPosition.y, gPosition.x, -60, 0, stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(rs+5, 4, 5));
}

void autoStationaryLeft2()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	float _x;
	float _y;

	stationaryLeftCore();

	// 3
	moveToTargetDisSimpleAsync(PI / 2, 14, 0, gPosition.x, 50, 0, stopHarsh, false);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(ls+2, 3, 1));
	turnToTargetCustomAsync(71, 23, ccw, 0, 45, 0.15);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(ls+2, 3, 2));
	moveToTargetSimpleAsync(71, 23, gPosition.y, gPosition.x, 40, 11, stopHarsh, false);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(ls+2, 3, 3));
	armLowerSimpleAsync(ARM_BOTTOM + 250, -127, 0);
	coneTimeout = nPgmTime + 2000;
	armTimeoutWhile(armLowerSimpleState, coneTimeout, TID2(ls+2, 3, 4));

	// 4
	turnToTargetCustomAsync(47, 47, cw, 0, 60, 0.3);
	driveTimeout = nPgmTime + 2000;
	liftRaiseSimpleAsync(90, 127, -15);
	armRaiseSimpleAsync(2400, 127, -25);
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(ls+2, 4, 1));
	setDrive(40, 40);
	driveTimeout = nPgmTime + 3000;
	sleep(500);
	timeoutWhileGreaterThanF(&gVelocity.x, 0.1, driveTimeout, TID2(ls+2, 4, 2));
	setDrive(15, 15);
	sleep(500);
	liftLowerSimpleAsync(55, -127, 25);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[liftPoti].value, 55, coneTimeout, TID2(ls+2, 4, 3));
	armLowerSimpleAsync(ARM_HORIZONTAL, -127, 25);
	coneTimeout = nPgmTime + 1500;
	timeoutWhileGreaterThanL(&gSensor[armPoti].value, ARM_HORIZONTAL + 200, coneTimeout, TID2(ls+2, 4, 4));

	// 5
	liftRaiseSimpleAsync(95, 127, -15);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[liftPoti].value, 85, coneTimeout, TID2(ls+2, 5, 1));
	armRaiseSimpleAsync(ARM_TOP, 127, -15);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[armPoti].value, ARM_TOP - 200, coneTimeout, TID2(ls+2, 5, 2));
	moveToTargetDisSimpleAsync(gPosition.a, -18, gPosition.y, gPosition.x, -40, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(ls+2, 5, 3));
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	coneTimeout = nPgmTime + 1200;
	liftTimeoutWhile(liftLowerSimpleState, coneTimeout, TID2(ls+2, 5, 4));
}

void autoStationaryRight2()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	float _x;
	float _y;

	stationaryRightCore();

	// 3
	moveToTargetDisSimpleAsync(0, 14, 0, gPosition.x, 50, 0, stopHarsh, false);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(rs+2, 3, 1));
	turnToTargetNewAsync(23, 71, cw, 0);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(turnToTargetNewState, driveTimeout, TID2(rs+2, 3, 2));
	moveToTargetSimpleAsync(23, 71, gPosition.y, gPosition.x, 40, 11, stopHarsh, false);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetSimpleState, driveTimeout, TID2(rs+2, 3, 3));
	armLowerSimpleAsync(ARM_BOTTOM + 250, -127, 0);
	coneTimeout = nPgmTime + 2000;
	armTimeoutWhile(armLowerSimpleState, coneTimeout, TID2(rs+2, 3, 4));

	// 4
	turnToTargetCustomAsync(47, 47, ccw, 0, 60, 0.3);
	driveTimeout = nPgmTime + 2000;
	liftRaiseSimpleAsync(90, 127, -15);
	armRaiseSimpleAsync(2400, 127, -25);
	autoSimpleTimeoutWhile(turnToTargetCustomState, driveTimeout, TID2(rs+2, 4, 1));
	setDrive(40, 40);
	driveTimeout = nPgmTime + 3000;
	sleep(500);
	timeoutWhileGreaterThanF(&gVelocity.y, 0.1, driveTimeout, TID2(rs+2, 4, 2));
	setDrive(15, 15);
	sleep(500);
	liftLowerSimpleAsync(55, -127, 25);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileGreaterThanL(&gSensor[liftPoti].value, 55, coneTimeout, TID2(rs+2, 4, 3));
	armLowerSimpleAsync(ARM_HORIZONTAL, -127, 25);
	coneTimeout = nPgmTime + 1500;
	timeoutWhileGreaterThanL(&gSensor[armPoti].value, ARM_HORIZONTAL + 200, coneTimeout, TID2(rs+2, 4, 4));

	// 5
	liftRaiseSimpleAsync(95, 127, -15);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[liftPoti].value, 85, coneTimeout, TID2(rs+2, 5, 1));
	armRaiseSimpleAsync(ARM_TOP, 127, -15);
	coneTimeout = nPgmTime + 2000;
	timeoutWhileLessThanL(&gSensor[armPoti].value, ARM_TOP - 200, coneTimeout, TID2(rs+2, 5, 2));
	moveToTargetDisSimpleAsync(gPosition.a, -18, gPosition.y, gPosition.x, -40, 0, stopSoft | stopHarsh, true);
	driveTimeout = nPgmTime + 2000;
	autoSimpleTimeoutWhile(moveToTargetDisSimpleState, driveTimeout, TID2(rs+2, 5, 3));
	liftLowerSimpleAsync(LIFT_BOTTOM, -127, 0);
	coneTimeout = nPgmTime + 1200;
	liftTimeoutWhile(liftLowerSimpleState, coneTimeout, TID2(rs+2, 5, 4));
}

void autoTest()
{
	unsigned long driveAsync;
	unsigned long coneAsync;
	unsigned long driveTimeout;
	unsigned long coneTimeout;
	float _x;
	float _y;

	//resetPositionFull(gPosition, 0, 0, 0);
	//moveToTargetSimple(-48, 0, -127, 0, stopSoft | stopHarsh, true);

	resetSonarYOnly(300, START_BAR_RESET_INTERCEPT, -0.75 * PI, true);
	resetSonarFull(300, -0.75 * PI, true);

	//while (true)
	//{
	//	resetPositionFull(gPosition, 0, 0, 0);
	//	moveToTargetSimple(48, 0, 127, 8, stopSoft | stopHarsh, true);
	//	sleep(1000);
	//	moveToTargetSimple(0, 0, -127, 8, stopSoft | stopHarsh, true);
	//	sleep(1000);
	//}
}
*/
