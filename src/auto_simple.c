/* Functions */
void moveToTargetSimple(float y, float x, float ys, float xs, byte power, float decelEarly, byte decelPower, float dropEarly, tStopType stopType, bool slow)
{
	writeDebugStreamLine("Moving to %f %f from %f %f at %d", y, x, ys, xs, power);

	sPID pidY;
	pidInit(pidY, 0.12, 0.005, 0.0, 0.5, 1.5, -1, 1.0);

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
	writeDebugStreamLine("Line length: %.2f", lineLength);
	float lineAngle = getAngleOfLine(followLine); // Get the angle of the line that we're following relative to the vertical
	float pidAngle = nearAngle(lineAngle - (power < 0 ? PI : 0), gPosition.a);
	writeDebugStreamLine("Line | Pid angle: %f | %f", radToDeg(lineAngle), radToDeg(pidAngle));

	// Current position relative to the ending point
	sVector currentPosVector;
	sPolar currentPosPolar;

	sCycleData cycle;
	initCycle(cycle, 10, "moveToTarget");

	float vel;
	float _sin = sin(lineAngle);
	float _cos = cos(lineAngle);

	word last = 0;

	if (!slow)
		setDrive(power, power);

	unsigned long timeStart = nPgmTime;
	do
	{
		currentPosVector.x = gPosition.x - x;
		currentPosVector.y = gPosition.y - y;
		vectorToPolar(currentPosVector, currentPosPolar);
		currentPosPolar.angle += lineAngle;
		polarToVector(currentPosPolar, currentPosVector);

		if (slow)
		{
			word finalPower = round(-127.0 / 48.0 * sgn(power) * currentPosVector.y);
			LIM_TO_VAL_SET(finalPower, abs(power));
			if (finalPower * sgn(power) < 30)
				finalPower = 30 * sgn(power);
			word delta = finalPower - last;
			LIM_TO_VAL_SET(delta, 5);
			finalPower = last += delta;
			setDrive(finalPower, finalPower);
			writeDebugStreamLine("%d | %.2f %d", nPgmTime, currentPosVector.y, finalPower);
		}

		vel = _sin * gVelocity.x + _cos * gVelocity.y;

		endCycle(cycle);
	} while (currentPosVector.y < -dropEarly - MAX((vel * ((stopType & stopSoft) ? 0.175 : 0.098)), decelEarly));

	writeDebugStreamLine("%f %f", currentPosVector.y, vel);

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

	writeDebugStreamLine("Moved to %f %f from %f %f | %f %f %f", y, x, ys, xs, gPosition.y, gPosition.x, radToDeg(gPosition.a));
}

void moveToTargetSimple(float y, float x, byte power, float decelEarly, byte decelPower, float dropEarly, tStopType stopType, bool slow) { moveToTargetSimple(y, x, gPosition.y, gPosition.x, power, decelEarly, decelPower, dropEarly, stopType, slow); }

void moveToTargetDisSimple(float a, float d, float ys, float xs, byte power, float decelEarly, byte decelPower, float dropEarly, tStopType stopType, bool slow)
{
	moveToTargetSimple(ys + d * cos(a), xs + d * sin(a), ys, xs, power, decelEarly, decelPower, dropEarly, stopType, slow);
}

void moveToTargetDisSimple(float a, float d, byte power, float decelEarly, byte decelPower, float dropEarly, tStopType stopType, bool slow) { moveToTargetDisSimple(a, d, gPosition.y, gPosition.x, power, decelEarly, decelPower, dropEarly, stopType, slow); }

void turnToAngleRadSimple(float a, tTurnDir turnDir, byte left, byte right, bool mogo)
{
	writeDebugStreamLine("Turning to %f", radToDeg(a));
	sTurnState state;
	state.time = nPgmTime;
	state.lstTime = state.time;
	state.nextDebug = 0;
	state.input = gVelocity.a;
	state.power = state.error;
	state.mogo = mogo;
	state.left = left;
	state.right = right;

	left = abs(left);
	right = abs(right);

	if (turnDir == ch)
		if (a < gPosition.a) turnDir = ccw; else turnDir = cw;

	switch (turnDir)
	{
	case cw:
		a = gPosition.a + fmod(a - gPosition.a, PI * 2);
		writeDebugStreamLine("%f", a);
		setDrive(left, -right);
		while (gPosition.a < a - gVelocity.a * (mogo ? 0.31 : 0.34)) sleep(1);
		writeDebugStreamLine("%f", gVelocity.a);
		//
		state.target = 0.900;
		while (gPosition.a < a - degToRad(mogo ? 2.2 : 3.5))
			turnSimpleInternalCw(a, state);
		setDrive(-30 * sgn(left), 30 * sgn(right));
		sleep(50);
		setDrive(0, 0);
		break;
	case ccw:
		a = gPosition.a - fmod(gPosition.a - a, PI * 2);
		setDrive(-left, right);
		while (gPosition.a > a - gVelocity.a * (mogo ? 0.31 : 0.34)) sleep(1);
		writeDebugStreamLine("%f", gVelocity.a);
		//
		state.target = -0.900;
		while (gPosition.a > a + degToRad(mogo ? 2.2 : 3.5))
			turnSimpleInternalCcw(a, state);
		setDrive(30 * sgn(left), -30 * sgn(right));
		sleep(50);
		setDrive(0, 0);
		break;
	}

	writeDebugStreamLine("Turned to %f | %f %f %f", radToDeg(a), gPosition.y, gPosition.x, radToDeg(gPosition.a));
}

void turnToAngleSimple(float a, tTurnDir turnDir, byte left, byte right, bool mogo)
{
	turnToAngleRadSimple(degToRad(a), turnDir, left, right, mogo);
}

void turnToTargetSimple(float y, float x, tTurnDir turnDir, byte left, byte right, bool mogo, float offset)
{
	writeDebugStreamLine("Turning to %f %f", y, x);
	sTurnState state;
	state.time = nPgmTime;
	state.lstTime = state.time;
	state.nextDebug = 0;
	state.input = gVelocity.a;
	state.power = state.error = 0;
	state.mogo = mogo;
	state.left = left;
	state.right = right;

	left = abs(left);
	right = abs(right);

	float a = atan2(x - gPosition.x, y - gPosition.y) + offset;

	if (turnDir == ch)
		if (fmod(a - gPosition.a, PI * 2) > PI) turnDir = ccw; else turnDir = cw;

	switch (turnDir)
	{
	case cw:
		a = gPosition.a + fmod(a - gPosition.a, PI * 2);
		writeDebugStreamLine("%f", a);
		setDrive(left, -right);
		while (gPosition.a < a - gVelocity.a * (mogo ? 0.31 : 0.34))
		{
			a = nearAngle(gPosition.a + fmod(atan2(x - gPosition.x, y - gPosition.y) + offset - gPosition.a, PI * 2), a);
			sleep(1);
		}
		writeDebugStreamLine("%f", gVelocity.a);
		//
		state.target = 0.900;
		while (gPosition.a < a - degToRad(mogo ? 2.2 : 3.5))
		{
			a = gPosition.a + fmod(atan2(x - gPosition.x, y - gPosition.y) + offset - gPosition.a, PI * 2);
			turnSimpleInternalCw(a, state);
		}
		setDrive(-30 * sgn(left), 30 * sgn(right));
		sleep(50);
		setDrive(0, 0);
		break;
	case ccw:
		a = gPosition.a - fmod(gPosition.a - a, PI * 2);
		writeDebugStreamLine("%f", a);
		setDrive(-left, right);
		while (gPosition.a > a - gVelocity.a * (mogo ? 0.31 : 0.34))
		{
			a = nearAngle(gPosition.a - fmod(gPosition.a - atan2(x - gPosition.x, y - gPosition.y) - offset, PI * 2), a);
			sleep(1);
		}
		writeDebugStreamLine("%f", gVelocity.a);
		//
		state.target = -0.900;
		while (gPosition.a > a + degToRad(mogo ? 2.2 : 3.5))
		{
			a = gPosition.a - fmod(gPosition.a - atan2(x - gPosition.x, y - gPosition.y) - offset, PI * 2);
			turnSimpleInternalCcw(a, state);
		}
		setDrive(30 * sgn(left), -30 * sgn(right));
		sleep(50);
		setDrive(0, 0);
		break;
	}

	writeDebugStreamLine("Turned to %f %f %f | %f %f %f", y, x, radToDeg(a), gPosition.y, gPosition.x, radToDeg(gPosition.a));
}

void turnSimpleInternalCw(float a, sTurnState& state)
{
	unsigned long deltaTime = state.time - state.lstTime;
	float vel = gVelocity.a;

	const float kP = 18;

	if (deltaTime >= 1)
	{
		state.input = vel;

		state.lstError = state.error;
		state.error = state.target - state.input;

		if (state.error < -0.8)
			state.power = -5;
		else
		{
			state.power = state.error * kP;

			state.power += 26;

			if (state.power > 32) state.power = 32;
			if (state.power < 24) state.power = 24;
		}

		setDrive(LIM_TO_VAL(state.power, state.left), -LIM_TO_VAL(state.power, state.right));

		if (state.time >= state.nextDebug)
		{
			writeDebugStreamLine("%f %f", state.power, state.error);
			state.nextDebug = state.time + 20;
		}

		state.lstTime = state.time;
	}

	sleep(1);

	state.time = nPgmTime;
}

void turnSimpleInternalCcw(float a, sTurnState& state)
{
	unsigned long deltaTime = state.time - state.lstTime;
	float vel = gVelocity.a;

	const float kP = 18;

	if (deltaTime >= 1)
	{
		state.input = vel;

		state.lstError = state.error;
		state.error = state.target - state.input;

		if (state.error > 0.8)
			state.power = -5;
		else
		{
			state.power = state.error * kP;

			state.power -= 26;

			if (state.power < -32) state.power = -32;
			if (state.power > -24) state.power = -24;
		}

		setDrive(LIM_TO_VAL(state.power, state.left), -LIM_TO_VAL(state.power, state.right));

		if (state.time >= state.nextDebug)
		{
			writeDebugStreamLine("%f %f", state.power, state.error);
			state.nextDebug = state.time + 20;
		}

		state.lstTime = state.time;
	}

	sleep(1);

	state.time = nPgmTime;
}

void turnToAngleCustom(float a, tTurnDir turnDir, byte power, float epsilon)
{
	writeDebugStreamLine("Turning to %f", radToDeg(a));
	if (turnDir == ch)
		if (fmod(a - gPosition.a, PI * 2) > PI) turnDir = ccw; else turnDir = cw;

	switch (turnDir)
	{
	case cw:
		a = gPosition.a + fmod(a - gPosition.a, PI * 2);
		setDrive(power, -power);
		while (gPosition.a < a - epsilon) sleep(1);
		break;
	case ccw:
		a = gPosition.a - fmod(gPosition.a - a, PI * 2);
		setDrive(-power, power);
		while (gPosition.a > a + epsilon) sleep(1);
		break;
	}
	applyHarshStop();
	writeDebugStreamLine("Turned to %f | %f %f %f", radToDeg(a), gPosition.y, gPosition.x, radToDeg(gPosition.a));
}

void turnToTargetCustom(float y, float x, tTurnDir turnDir, float offset, byte power, float epsilon)
{
	writeDebugStreamLine("Turning to %f %f + %f", y, x, radToDeg(offset));
	float a;
	if (turnDir == ch)
		if (fmod(atan2(x - gPosition.x, y - gPosition.y) + offset - gPosition.a, PI * 2) > PI) turnDir = ccw; else turnDir = cw;

	switch (turnDir)
	{
	case cw:
		writeDebugStreamLine("CW %d %d | %d", y, x, a);
		setDrive(power, -power);
		do
		{
			a = gPosition.a + fmod(atan2(x - gPosition.x, y - gPosition.y) - gPosition.a + offset, PI * 2);
			sleep(1);
		} while (gPosition.a < a - epsilon);
		break;
	case ccw:
		writeDebugStreamLine("CCW %d %d | %d", y, x, a);
		setDrive(-power, power);
		do
		{
			a = gPosition.a - fmod(gPosition.a - atan2(x - gPosition.x, y - gPosition.y) - offset, PI * 2);
			sleep(1);
		} while (gPosition.a > a + epsilon);
		break;
	}
	applyHarshStop();
	writeDebugStreamLine("Turned to %f %f | %f | %f %f %f", y, x, radToDeg(a), gPosition.y, gPosition.x, radToDeg(gPosition.a));
}

//void resetBlueLeft()
//{
//	setDrive(-30, -30);
//	unsigned long timeout = nPgmTime + 1500;
//	sleep(500);
//	timeoutWhileLessThanF(&gVelocity.x, -0.1, timeout, TID0(rbl));
//	setDrive(-7, -7);
//	sleep(500);
//	resetPositionFull(gPosition, gPosition.y, 8.25, 90);
//}

//void resetBlueRight()
//{
//	setDrive(-30, -30);
//	unsigned long timeout = nPgmTime + 1500;
//	sleep(500);
//	timeoutWhileLessThanF(&gVelocity.y, -0.1, timeout, TID0(rbr));
//	setDrive(-7, -7);
//	sleep(500);
//	resetPositionFull(gPosition, 8.25, gPosition.x, 0);
//}
