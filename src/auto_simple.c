/* Functions */
#define NOT_SAFETY(power, message) !TimedOut(npgmTime + 100, TID1(message, 0), true, VEL_LOCAL_Y, sgn(power)*0.5, nPgmTime-timeStart, &velSafetyCounter)
void moveToTargetSimple(float y, float x, float ys, float xs, byte power, byte startPower, float maxErrX, float decelEarly, byte decelPower, float dropEarly, tStopType stopType, tMttMode mode, bool velSafety)
{
	int velSafetyCounter = 0;
	if (LOGS) writeDebugStreamLine("Moving to %f %f from %f %f at %d", y, x, ys, xs, power);

	gTargetLast.y = y;
	gTargetLast.x = x;

	// Create the line to follow
	sLine followLine;

	// Start points
	followLine.p1.y = ys;
	followLine.p1.x = xs;

	// End points
	followLine.p2.y = y;
	followLine.p2.x = x;

	float lineLength = getLengthOfLine(followLine);
	if (LOGS) writeDebugStreamLine("Line length: %.2f", lineLength);
	float lineAngle = getAngleOfLine(followLine); // Get the angle of the line that we're following relative to the vertical
	float pidAngle = nearAngle(lineAngle - (power < 0 ? PI : 0), gPosition.a);
	if (LOGS) writeDebugStreamLine("Line | Pid angle: %f | %f", radToDeg(lineAngle), radToDeg(pidAngle));

	// Current position relative to the ending point
	sVector currentPosVector;
	sPolar currentPosPolar;

	sCycleData cycle;
	initCycle(cycle, 10, "moveToTarget");

	float vel;
	float _sin = sin(lineAngle);
	float _cos = cos(lineAngle);

	word last = startPower;
	float correction = 0;

	if (mode == mttSimple)
		setDrive(power, power);

	word finalPower = power;

	unsigned long timeStart = nPgmTime;
	do
	{
		currentPosVector.x = gPosition.x - x;
		currentPosVector.y = gPosition.y - y;
		vectorToPolar(currentPosVector, currentPosPolar);
		currentPosPolar.angle += lineAngle;
		polarToVector(currentPosPolar, currentPosVector);

		if (maxErrX)
		{
			float errA = gPosition.a - pidAngle;
			float errX = currentPosVector.x + currentPosVector.y * sin(errA) / cos(errA);
			float correctA = atan2(x - gPosition.x, y - gPosition.y);
			if (power < 0)
				correctA += PI;
			correction = fabs(errX) > maxErrX ? 8.0 * (nearAngle(correctA, gPosition.a) - gPosition.a) * sgn(power) : 0;
		}

		if (mode != mttSimple)
		{
			switch (mode)
			{
			case mttProportional:
				finalPower = round(-127.0 / 40.0 * currentPosVector.y) * sgn(power);
				break;
			case mttCascading:
#if SKILLS_ROUTE == 0
				const float kB = 2.8;
				const float kP = 2.0;
#else
				float kB, kP;
				if (nPgmTime - gAutoTime > 40000)
				{
					kB = 5.0;
					kP = 3.2;
				}
				else
				{
					kB = 4.5;
					kP = 2.5;
				}
#endif
				float vTarget = 45 * (1 - exp(0.07 * (currentPosVector.y + dropEarly)));
				finalPower = round(kB * vTarget + kP * (vTarget - vel)) * sgn(power);
				break;
			}
			LIM_TO_VAL_SET(finalPower, abs(power));
			if (finalPower * sgn(power) < 30)
				finalPower = 30 * sgn(power);
			word delta = finalPower - last;
			LIM_TO_VAL_SET(delta, 5);
			finalPower = last += delta;
		}

		switch (sgn(correction))
		{
		case 0:
			setDrive(finalPower, finalPower);
			break;
		case 1:
			setDrive(finalPower, finalPower * exp(-correction));
			break;
		case -1:
			setDrive(finalPower * exp(correction), finalPower);
			break;
		}

		vel = _sin * gVelocity.x + _cos * gVelocity.y;

		endCycle(cycle);
	} while (currentPosVector.y < -dropEarly - MAX((vel * ((stopType & stopSoft) ? 0.175 : 0.098)), decelEarly) && (velSafety? NOT_SAFETY(power, moveToTargetSimple) : 1 ) );

	if (LOGS) writeDebugStreamLine("%f %f", currentPosVector.y, vel);

	setDrive(decelPower, decelPower);

	do
	{
		currentPosVector.x = gPosition.x - x;
		currentPosVector.y = gPosition.y - y;
		vectorToPolar(currentPosVector, currentPosPolar);
		currentPosPolar.angle += lineAngle;
		polarToVector(currentPosPolar, currentPosVector);

		vel = _sin * gVelocity.x + _cos * gVelocity.y;

		endCycle(cycle);
	} while (currentPosVector.y < -dropEarly - (vel * ((stopType & stopSoft) ? 0.175 : 0.098)));

	if (stopType & stopSoft)
	{
		setDrive(-6 * sgn(power), -6 * sgn(power));
		do
		{
			currentPosVector.x = gPosition.x - x;
			currentPosVector.y = gPosition.y - y;
			vectorToPolar(currentPosVector, currentPosPolar);
			currentPosPolar.angle += lineAngle;
			polarToVector(currentPosPolar, currentPosVector);

			vel = _sin * gVelocity.x + _cos * gVelocity.y;

			endCycle(cycle);
		} while (vel > 7 && currentPosVector.y < 0);
	}

	if (stopType & stopHarsh)
		applyHarshStop();
	else
		setDrive(0, 0);

	if (LOGS) writeDebugStreamLine("Moved to %f %f from %f %f | %f %f %f", y, x, ys, xs, gPosition.y, gPosition.x, radToDeg(gPosition.a));
}

void moveToTargetDisSimple(float a, float d, float ys, float xs, byte power, byte startPower, float maxErrX, float decelEarly, byte decelPower, float dropEarly, tStopType stopType, tMttMode mode, bool velSafety)
{
	moveToTargetSimple(ys + d * cos(a), xs + d * sin(a), ys, xs, power, startPower, maxErrX, decelEarly, decelPower, dropEarly, stopType, mode, false);
}

void turnToAngleNewAlg(float a, tTurnDir turnDir, float fullRatio, byte coastPower, float stopOffsetDeg, bool mogo, bool harshStop, bool velSafety)
{
	if (LOGS) writeDebugStreamLine("Turning to %f", radToDeg(a));

	int velSafetyCounter = 0;

	if (turnDir == ch)
		if (fmod(a - gPosition.a, PI * 2) > PI) turnDir = ccw; else turnDir = cw;

	float endFull;

	unsigned long timeStart = nPgmTime;

	switch (turnDir)
	{
	case cw:
		a = gPosition.a + fmod(a - gPosition.a, PI * 2);
		endFull = gPosition.a * (1 - fullRatio) + a * fullRatio;
		setDrive(127, -127);
		while (gPosition.a < endFull /*&& (velSafety? NOT_SAFETY(power, turnToAngleNewAlg) : 1 )*/)
		{
			if (DATALOG_TURN != -1)
			{
				tHog();
				datalogDataGroupStart();
				datalogAddValue(DATALOG_TURN + 0, radToDeg(gPosition.a));
				datalogAddValue(DATALOG_TURN + 1, 127);
				datalogDataGroupEnd();
				tRelease();
			}
			sleep(10);
		}
		setDrive(coastPower, -coastPower);
		timeStart = nPgmTime;
		while (gPosition.a < a - degToRad(stopOffsetDeg) /* && (velSafety? NOT_SAFETY(power, turnToAngleNewAlg) : 1 ) */)
		{
			if (DATALOG_TURN != -1)
			{
				tHog();
				datalogDataGroupStart();
				datalogAddValue(DATALOG_TURN + 0, radToDeg(gPosition.a));
				datalogAddValue(DATALOG_TURN + 1, 127);
				datalogDataGroupEnd();
				tRelease();
			}
			sleep(10);
		}
		if (LOGS) writeDebugStreamLine("Turn done: %d",  gPosition.a);
		if (harshStop)
		{
			setDrive(-20, 20);
			sleep(150);
			if (LOGS) writeDebugStreamLine("Break done: %d",  gPosition.a);
		}
		setDrive(0, 0);
		break;
	case ccw:
		a = gPosition.a - fmod(gPosition.a - a, PI * 2);
		endFull = gPosition.a * (1 - fullRatio) + a * fullRatio;
		setDrive(-127, 127);
		while (gPosition.a > endFull /* && (velSafety? NOT_SAFETY(power, turnToAngleNewAlg) : 1 )*/)
		{
			if (DATALOG_TURN != -1)
			{
				tHog();
				datalogDataGroupStart();
				datalogAddValue(DATALOG_TURN + 0, radToDeg(gPosition.a));
				datalogAddValue(DATALOG_TURN + 1, 127);
				datalogDataGroupEnd();
				tRelease();
			}
			sleep(10);
		}
		setDrive(-coastPower, coastPower);
		timeStart = npgmTime;
		while (gPosition.a > a + degToRad(stopOffsetDeg)/* && (velSafety? NOT_SAFETY(power, turnToAngleNewAlg) : 1)*/)
		{
			if (DATALOG_TURN != -1)
			{
				tHog();
				datalogDataGroupStart();
				datalogAddValue(DATALOG_TURN + 0, radToDeg(gPosition.a));
				datalogAddValue(DATALOG_TURN + 1, 127);
				datalogDataGroupEnd();
				tRelease();
			}
			sleep(10);
		}
		if (LOGS) writeDebugStreamLine("Turn done: %d",  gPosition.a);
		if (harshStop)
		{
			setDrive(20, -20);
			sleep(150);
			if (LOGS) writeDebugStreamLine("Break done: %d",  gPosition.a);
		}
		setDrive(0, 0);
		break;
	}
	if (LOGS) writeDebugStreamLine("Turned to %f | %f %f %f", radToDeg(a), gPosition.y, gPosition.x, radToDeg(gPosition.a));
}

void turnToTargetNewAlg(float y, float x, tTurnDir turnDir, float fullRatio, byte coastPower, float stopOffsetDeg, bool mogo, bool harshStop, float offset, bool velSafety)
{
	if (LOGS) writeDebugStreamLine("Turning to %f %f", y, x);

	if (turnDir == ch)
		if (fmod(atan2(x - gPosition.x, y - gPosition.y) + offset - gPosition.a, PI * 2) > PI) turnDir = ccw; else turnDir = cw;

	float endFull, target;

	int velSafetyCounter = 0;
	unsigned long timeStart =  nPgmTime;

	switch (turnDir)
	{
	case cw:
		target = gPosition.a + fmod(atan2(x - gPosition.x, y - gPosition.y) + offset - gPosition.a, PI * 2);
		endFull = gPosition.a * (1 - fullRatio) + target * fullRatio;
		if (LOGS) writeDebugStreamLine("%f %f", radToDeg(target), radToDeg(endFull));
		setDrive(127, -127);
		while (gPosition.a < endFull /*&& (velSafety? NOT_SAFETY(power, turnToTargetNewAlg) : 1 )*/)
		{
			if (DATALOG_TURN != -1)
			{
				tHog();
				datalogDataGroupStart();
				datalogAddValue(DATALOG_TURN + 0, radToDeg(gPosition.a));
				datalogAddValue(DATALOG_TURN + 1, 127);
				datalogDataGroupEnd();
				tRelease();
			}
			sleep(10);
		}
		setDrive(coastPower, -coastPower);
		timeStart = npgmTime;
		while (gPosition.a < nearAngle(atan2(x - gPosition.x, y - gPosition.y) + offset, target) - degToRad(stopOffsetDeg) /*&& (velSafety? NOT_SAFETY(power, turnToTargetNewAlg) : 1 )*/)
		{
			if (DATALOG_TURN != -1)
			{
				tHog();
				datalogDataGroupStart();
				datalogAddValue(DATALOG_TURN + 0, radToDeg(gPosition.a));
				datalogAddValue(DATALOG_TURN + 1, 127);
				datalogDataGroupEnd();
				tRelease();
			}
			sleep(10);
		}
		if (LOGS) writeDebugStreamLine("Turn done: %d",  gPosition.a);
		if (harshStop)
		{
			setDrive(-20, 20);
			sleep(150);
			if (LOGS) writeDebugStreamLine("Break done: %d",  gPosition.a);
		}
		setDrive(0, 0);
		break;
	case ccw:
		target = gPosition.a - fmod(gPosition.a - atan2(x - gPosition.x, y - gPosition.y) - offset, PI * 2);
		endFull = gPosition.a * (1 - fullRatio) + (target) * fullRatio;
		if (LOGS) writeDebugStreamLine("%f %f", radToDeg(target), radToDeg(endFull));
		setDrive(-127, 127);
		while (gPosition.a > endFull /*&& (velSafety? NOT_SAFETY(power, turnToTargetNewAlg) : 1 )*/)
		{
			if (DATALOG_TURN != -1)
			{
				tHog();
				datalogDataGroupStart();
				datalogAddValue(DATALOG_TURN + 0, radToDeg(gPosition.a));
				datalogAddValue(DATALOG_TURN + 1, 127);
				datalogDataGroupEnd();
				tRelease();
			}
			sleep(10);
		}
		setDrive(-coastPower, coastPower);
		timeStart = nPgmTime;
		while (gPosition.a > nearAngle(atan2(x - gPosition.x, y - gPosition.y) + offset, target) + degToRad(stopOffsetDeg) /*&& (velSafety? NOT_SAFETY(power, turnToTargetNewAlg) : 1 )*/)
		{
			if (DATALOG_TURN != -1)
			{
				tHog();
				datalogDataGroupStart();
				datalogAddValue(DATALOG_TURN + 0, radToDeg(gPosition.a));
				datalogAddValue(DATALOG_TURN + 1, 127);
				datalogDataGroupEnd();
				tRelease();
			}
			sleep(10);
		}
		if (LOGS) writeDebugStreamLine("Turn done: %d",  gPosition.a);
		if (harshStop)
		{
			setDrive(20, -20);
			sleep(150);
			if (LOGS) writeDebugStreamLine("Break done: %d",  gPosition.a);
		}
		setDrive(0, 0);
		break;
	}
	if (LOGS) writeDebugStreamLine("Turned to %f %f | %f %f %f", y, x, gPosition.y, gPosition.x, radToDeg(gPosition.a));
}

void sweepTurnToTarget(float y, float x, float a, float r, tTurnDir turnDir, byte power, bool slow, bool velSafety)
{
	sVector vector;
	sPolar polar;
	int velSafetyCounter = 0;

	if (turnDir == ch)
	{
		vector.x = gPosition.x - x;
		vector.y = gPosition.y - y;
		vectorToPolar(vector, polar);
		polar.angle += a;
		polarToVector(polar, vector);

		turnDir = vector.x > 0 ? cw : ccw;
	}

	float yOrigin, xOrigin;
	float linearV, angularV, angularVLast = 0;
	float localR, localA;

	const float kR = 15.0;
	const float kA = 5.0;
	const float kB = 60.0;
	const float kP = 30.0;
	const float kD = 2000.0;

	sCycleData cycle;
	initCycle(cycle, 40, "sweepTurnToTarget");

	unsigned long timeStart = nPgmTime;
	switch (turnDir)
	{
	case cw:
		vector.y = 0;
		vector.x = r;
		vectorToPolar(vector, polar);
		polar.angle -= a;
		polarToVector(polar, vector);
		yOrigin = y + vector.y;
		xOrigin = x + vector.x;

		localA = atan2(gPosition.x - xOrigin, gPosition.y - yOrigin);

		a = nearAngle(a, power > 0 ? gPosition.a : (gPosition.a + PI));

		if (LOGS) writeDebugStreamLine("%d Sweep to %f around %f %f", nPgmTime, radToDeg(a), yOrigin, xOrigin);

		do
		{
			float aGlobal = gPosition.a;
			if (power < 0)
				aGlobal += PI;
			angularV = gVelocity.a;
			float _y = gPosition.y - yOrigin;
			float _x = gPosition.x - xOrigin;
			localR = sqrt(_y * _y + _x * _x);
			localA = nearAngle(atan2(_x, _y), localA);
			linearV = gVelocity.x * sin(localA + PI / 2) + gVelocity.y * cos(localA + PI / 2);

			float target = MAX(linearV, 15) / localR + kR * log(localR / r) + kA * (nearAngle(localA + PI / 2, aGlobal) - aGlobal);
			word turn = round(kB * target + kP * (target - angularV) + kD * (angularVLast - angularV) / 40);
			angularVLast = angularV;

			if (turn < 0)
				turn = 0;
			else if (turn > 150)
				turn = 150;

			if (power > 0)
				setDrive(power, power - turn);
			else
				setDrive(power + turn, power);

			if (DATALOG_SWEEP != -1)
			{
				tHog();
				datalogDataGroupStart();
				datalogAddValue(DATALOG_SWEEP + 0, localR * 100);
				datalogAddValue(DATALOG_SWEEP + 1, radToDeg(localA) * 10);
				datalogAddValue(DATALOG_SWEEP + 2, radToDeg(target) * 10);
				datalogAddValue(DATALOG_SWEEP + 3, turn * 10);
				datalogAddValue(DATALOG_SWEEP + 4, linearV * 10);
				datalogAddValue(DATALOG_SWEEP + 5, radToDeg(angularV) * 10);
				datalogDataGroupEnd();
				tRelease();
			}

			endCycle(cycle);
		} while ((power > 0 ? gPosition.a : (gPosition.a + PI)) - a < (slow ? -0.1 : -0.15) && (velSafety? NOT_SAFETY(power, sweepTurnToTarget) : 1 ));
		break;
	case ccw:
		vector.y = 0;
		vector.x = r;
		vectorToPolar(vector, polar);
		polar.angle += a;
		polarToVector(polar, vector);
		yOrigin = y + vector.y;
		xOrigin = x + vector.x;

		localA = atan2(gPosition.x - xOrigin, gPosition.y - yOrigin);

		a = nearAngle(a, power > 0 ? gPosition.a : (gPosition.a + PI));

		if (LOGS) writeDebugStreamLine("%d Sweep to %f around %f %f", nPgmTime, radToDeg(a), yOrigin, xOrigin);

		do
		{
			float aGlobal = gPosition.a;
			if (power < 0)
				aGlobal += PI;
			angularV = gVelocity.a;
			float _y = gPosition.y - yOrigin;
			float _x = gPosition.x - xOrigin;
			localR = sqrt(_y * _y + _x * _x);
			localA = nearAngle(atan2(_x, _y), localA);
			linearV = gVelocity.x * sin(localA - PI / 2) + gVelocity.y * cos(localA - PI / 2);

			float target = -MAX(linearV, 15) / localR + kR * log(r / localR) + kA * (nearAngle(localA - PI / 2, aGlobal) - aGlobal);
			word turn = round(kB * target + kP * (target - angularV) + kD * (angularVLast - angularV) / 40);
			angularVLast = angularV;

			if (turn > 0)
				turn = 0;
			else if (turn < -150)
				turn = -150;

			if (power > 0)
				setDrive(power + turn, power);
			else
				setDrive(power, power - turn);

			if (DATALOG_SWEEP != -1)
			{
				tHog();
				datalogDataGroupStart();
				datalogAddValue(DATALOG_SWEEP + 0, localR * 100);
				datalogAddValue(DATALOG_SWEEP + 1, radToDeg(localA) * 10);
				datalogAddValue(DATALOG_SWEEP + 2, radToDeg(target) * 10);
				datalogAddValue(DATALOG_SWEEP + 3, turn * 10);
				datalogAddValue(DATALOG_SWEEP + 4, linearV * 10);
				datalogAddValue(DATALOG_SWEEP + 5, radToDeg(angularV) * 10);
				datalogDataGroupEnd();
				tRelease();
			}

			endCycle(cycle);
		} while ((power > 0 ? gPosition.a : (gPosition.a + PI)) - a > (slow ? 0.1 : 0.15) && (velSafety? NOT_SAFETY(power, moveToTargetSimple) : 1));
		break;
	}
	setDrive(0, 0);
	if (LOGS) writeDebugStreamLine("%d Done sweep turn", nPgmTime);
}
