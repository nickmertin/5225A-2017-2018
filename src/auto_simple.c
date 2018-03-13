/* Functions */
void moveToTargetSimple(float y, float x, float ys, float xs, byte power, float maxErrX, float decelEarly, byte decelPower, float dropEarly, tStopType stopType, bool slow)
{
	writeDebugStreamLine("Moving to %f %f from %f %f at %d", y, x, ys, xs, power);

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
	float correction = 0;

	if (!slow)
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
			correction = fabs(errX) > maxErrX ? -5.0 * errA : 0;
		}

		if (slow)
		{
			finalPower = round(-127.0 / 48.0 * sgn(power) * currentPosVector.y);
			LIM_TO_VAL_SET(finalPower, abs(power));
			if (finalPower * sgn(power) < 30)
				finalPower = 30 * sgn(power);
			word delta = finalPower - last;
			LIM_TO_VAL_SET(delta, 5);
			finalPower = last += delta;
			writeDebugStreamLine("%d | %.2f %d", nPgmTime, currentPosVector.y, finalPower);
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

void moveToTargetDisSimple(float a, float d, float ys, float xs, byte power, float maxErrX, float decelEarly, byte decelPower, float dropEarly, tStopType stopType, bool slow)
{
	moveToTargetSimple(ys + d * cos(a), xs + d * sin(a), ys, xs, power, maxErrX, decelEarly, decelPower, dropEarly, stopType, slow);
}

void moveToLineSimple(float a, float yInt, float ys, float xs, byte power, float decelEarly, byte decelPower, float dropEarly, tStopType stopType, bool slow)
{
	float sinA = sin(a);
	float cosA = cos(a);

	writeDebugStreamLine("Moving to y=%fx+%f from %f %f at %d", cosA / sinA, yInt, ys, xs, power);

	float y, x;

	sCycleData cycle;
	initCycle(cycle, 10, "moveToTarget");

	float vel, l;

	float _sin, _cos;

	word last = 0;

	if (!slow)
		setDrive(power, power);

	unsigned long timeStart = nPgmTime;
	do
	{
		if (nearAngle(gPosition.a, a) == a || nearAngle(gPosition.a + PI, a) == a)
			break;

		_sin = sin(gPosition.a);
		_cos = cos(gPosition.a);

		l = ((gPosition.y - yInt) * sinA - gPosition.x * cosA) / sin(gPosition.a - a);

		gTargetLast.x = gPosition.x + l * _sin;
		gTargetLast.y = gPosition.y + l * _cos;

		if (slow)
		{
			word finalPower = round(127.0 / 48.0 * sgn(power) * l);
			LIM_TO_VAL_SET(finalPower, abs(power));
			if (finalPower * sgn(power) < 30)
				finalPower = 30 * sgn(power);
			word delta = finalPower - last;
			LIM_TO_VAL_SET(delta, 5);
			finalPower = last += delta;
			setDrive(finalPower, finalPower);
			writeDebugStreamLine("%d | %.2f %d", nPgmTime, l, finalPower);
		}

		vel = _sin * gVelocity.x + _cos * gVelocity.y;

		endCycle(cycle);
	} while (l > dropEarly + MAX((vel * ((stopType & stopSoft) ? 0.175 : 0.098)), decelEarly));

	writeDebugStreamLine("%f %f", l, vel);

	setDrive(decelPower, decelPower);

	do
	{
		if (nearAngle(gPosition.a, a) == a || nearAngle(gPosition.a + PI, a) == a)
			break;

		_sin = sin(gPosition.a);
		_cos = cos(gPosition.a);

		l = ((gPosition.y - yInt) * sinA - gPosition.x * cosA) / sin(gPosition.a - a);

		gTargetLast.x = gPosition.x + l * _sin;
		gTargetLast.y = gPosition.y + l * _cos;

		vel = _sin * gVelocity.x + _cos * gVelocity.y;

		endCycle(cycle);
	} while (l > dropEarly + (vel * ((stopType & stopSoft) ? 0.175 : 0.098)));

	if (stopType & stopSoft)
	{
		setDrive(-6 * sgn(power), -6 * sgn(power));
		do
		{
			l = ((gPosition.y - yInt) * sinA - gPosition.x * cosA) / sin(gPosition.a - a);

			gTargetLast.x = gPosition.x + l * _sin;
			gTargetLast.y = gPosition.y + l * _cos;

			vel = _sin * gVelocity.x + _cos * gVelocity.y;

			endCycle(cycle);
		} while (vel > 7 && l > 0);
	}

	if (stopType & stopHarsh)
		applyHarshStop();
	else
		setDrive(0, 0);

	writeDebugStreamLine("Moved to %f %f from %f %f | %f %f %f", y, x, ys, xs, gPosition.y, gPosition.x, radToDeg(gPosition.a));
}

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

void turnToAngleRadNewAlg (float a, tTurnDir turnDir, bool mogo)
{
	writeDebugStreamLine("Turning to %f", radToDeg(a));

	unsigned long time = nPgmTime;
	unsigned long lstTime = time;
	unsigned long nextDebug = 0;
	float input = gVelocity.a;
	float power = 0;
	float velTarget = 0;
	unsigned long deltaTime;
	float vel;
	float error;

	const float kB = 30.0;
	const float kP = 18;
	float vTarget;

	if (turnDir == ch)
		if (a < gPosition.a) turnDir = ccw; else turnDir = cw;

	switch (turnDir)
	{
	case cw:
		a = gPosition.a + fmod(a - gPosition.a, PI * 2);
		writeDebugStreamLine("%f", a);
		writeDebugStreamLine("%f", gVelocity.a);
		//
		//velTarget = 0.900;
		//while (gPosition.a < a - degToRad(mogo ? 2.2 : 3.5))

		while (gPosition.a < a)
		{
			deltaTime = time - lstTime;
			vel = gVelocity.a;

			if (deltaTime >= 1)
			{
				input = gPosition.a;

				//lstError = error;
				error = a - input;
				//vTarget =  0.9 rad/ms
				vTarget = sgn(error) * (1.0 - exp(-0.4 * abs(error)));

				power = kB * vTarget + kP * (vTarget - gVelocity.a);

				//if (power < 5 && error > 0.8)
				//	power = 5;
				if (power > 127)
					power = 127;

				if (DATALOG_TURN != -1)
				{
					datalogDataGroupStart();
					datalogAddValue(DATALOG_TURN + 0, error);
					datalogAddValue(DATALOG_TURN + 1, a);
					datalogAddValue(DATALOG_TURN + 2, input);
					datalogAddValue(DATALOG_TURN + 3, vTarget * 1000);
					datalogAddValue(DATALOG_TURN + 4, gVelocity.a * 1000);
					datalogAddValue(DATALOG_TURN + 5, power);
				}

				setDrive(power, -power);

				lstTime = time;
			}

			sleep(1);

			time = nPgmTime;
		}

		setDrive(-30, 30);
		sleep(50);
		setDrive(0, 0);
		break;
	case ccw:
		a = gPosition.a - fmod(gPosition.a - a, PI * 2);
		velTarget = -0.900;
		while (gPosition.a > a + degToRad(mogo ? 2.2 : 3.5))
			//turnSimpleInternalCcw(a, state);
		setDrive(30, -30);
		sleep(50);
		setDrive(0, 0);
		break;
	}
	writeDebugStreamLine("Turned to %f | %f %f %f", radToDeg(a), gPosition.y, gPosition.x, radToDeg(gPosition.a));
}

void sweepTurnToTarget(float y, float x, float a, float r, tTurnDir turnDir, bool slow)
{
	sVector vector;
	sPolar polar;

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
	float linearV, angularV;
	float localR, localA;

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

		a += 2 * PI * floor((a - gPosition.a) / (2 * PI));

		writeDebugStreamLine("Sweep to %f", a);

		localA = atan2(gPosition.x - xOrigin, gPosition.y - yOrigin);

		const float kR = 15.0;
		const float kA = 3.0;
		const float kB = 60.0;
		const float kP = 15.0;

		do
		{
			float aGlobal = gPosition.a;
			linearV = gVelocity.x * sin(aGlobal) + gVelocity.y * cos(aGlobal);
			angularV = gVelocity.a;

			float _y = gPosition.y - yOrigin;
			float _x = gPosition.x - xOrigin;
			localR = sqrt(_y * _y + _x * _x);
			localA = nearAngle(atan2(_x, _y), localA);

			float target = MAX(linearV, 15) / localR + kR * log(localR / r) + kA * (localA + PI / 2 - aGlobal);
			word turn = round(kB * target + kP * (target - angularV));

			//LIM_TO_VAL_SET(turn, 80);
			if (turn < 0)
				turn = 0;
			else if (turn > 150)
				turn = 150;
			setDrive(127, 127 - turn);

			if (DATALOG_SWEEP != -1)
			{
				tHog();
				datalogDataGroupStart();
				datalogAddValue(DATALOG_SWEEP + 0, localR);
				datalogAddValue(DATALOG_SWEEP + 1, radToDeg(localA));
				datalogAddValue(DATALOG_SWEEP + 2, radToDeg(target) * 1000);
				datalogAddValue(DATALOG_SWEEP + 3, turn * 100);
				datalogAddValue(DATALOG_SWEEP + 4, linearV * 10);
				datalogAddValue(DATALOG_SWEEP + 5, radToDeg(angularV) * 1000);
				datalogDataGroupEnd();
				tRelease();
			}

			sleep(10);
		} while (gPosition.a - a < (slow ? -0.01 : -0.03));

		break;
	}
}
