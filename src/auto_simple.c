/* Functions */
void moveToTargetSimple(float y, float x, float ys, float xs, byte power, float dropEarly, tStopType stopType, bool slow)
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
	initCycle(cycle, 20, "moveToTarget");

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
			pidCalculate(pidY, 0, currentPosVector.y);
			word finalPower = round(power * abs(pidY.output));
			if (abs(finalPower) < 30) finalPower = 30 * sgn(finalPower);
			setDrive(finalPower, finalPower);
		}

		endCycle(cycle);
	} while (currentPosVector.y < ((stopType & stopSoft) ? (-gVelocity.y * 0.040) : 0) - dropEarly - (gVelocity.y * 0.098));

	if (stopType & stopSoft)
	{
		setDrive(-7, -7);
		while (gVelocity.y > 15) sleep(1);
	}

	if (stopType & stopHarsh)
		applyHarshStop();
	else
		setDrive(0, 0);

	writeDebugStreamLine("Moved to %f %f from %f %f | %f %f %f", y, x, ys, xs, gPosition.y, gPosition.x, radToDeg(gPosition.a));
}

void moveToTargetSimple(float y, float x, byte power, float dropEarly, tStopType stopType, bool slow) { moveToTargetSimple(y, x, gPosition.y, gPosition.x, power, dropEarly, stopType, slow); }

void moveToTargetDisSimple(float a, float d, float ys, float xs, byte power, float dropEarly, tStopType stopType, bool slow)
{
	sPolar polar;
	sVector vector;
	polar.angle = a;
	polar.magnitude = d;
	polarToVector(polar, vector);
	moveToTargetSimple(vector.y + ys, vector.x + xs, ys, xs, power, dropEarly, stopType, slow);
}

void moveToTargetDisSimple(float a, float d, byte power, float dropEarly, tStopType stopType, bool slow) { moveToTargetDisSimple(a, d, gPosition.y, gPosition.x, power, dropEarly, stopType, slow); }

void turnToAngleRadSimple(float a, tTurnDir turnDir, byte left, byte right)
{
	writeDebugStreamLine("Turning to %f", radToDeg(a));
	const float kP = 17, kI = 0.05;
	state.target = 0.900;
	state.time = nPgmTime;
	state.lstTime = state.time;
	state.nextDebug = 0;
	state.input = gVelocity.a;
	state.power = state.error = state.integral = 0;

	left = abs(left);
	right = abs(right);

	if (turnDir == ch)
		if (a < gPosition.a) turnDir = ccw; else turnDir = cw;

	sTurnState state;
	switch (turnDir)
	{
	case cw:
		a = gPosition.a + fmod(a - gPosition.a, PI * 2);
		writeDebugStreamLine("%f", a);
		setDrive(left, -right);
		while (gPosition.a < a - gVelocity.a * 0.6) sleep(1);
		writeDebugStreamLine("%f", gVelocity.a);
		//
		while (gPosition.a < a + degToRad(-5.3 + (state.target - gVelocity.a) * 0.3))
			turnSimpleInternalCw(a, state);
		setDrive(-15, 15);
		sleep(150);
		setDrive(0, 0);
		break;
	case ccw:
		a = gPosition.a + fmod(gPosition.a - a, PI * 2);
		writeDebugStreamLine("%f", a);
		setDrive(-left, right);
		while (gPosition.a > a + gVelocity.a * 0.6) sleep(1);
		writeDebugStreamLine("%f", gVelocity.a);
		//
		while (gPosition.a > a - degToRad(-5.3 + (state.target - gVelocity.a) * 0.3))
			turnSimpleInternalCcw(a, state);
		setDrive(15, -15);
		sleep(150);
		setDrive(0, 0);
		break;
	}

	writeDebugStreamLine("Turned to %f | %f %f %f", radToDeg(a), gPosition.y, gPosition.x, radToDeg(gPosition.a));
}

void turnToAngleSimple(float a, tTurnDir turnDir, byte left, byte right)
{
	turnToAngleRadSimple(degToRad(a), turnDir, left, right);
}

void turnSimpleInternalCw(float a, sTurnState& state)
{
	unsigned long deltaTime = state.time - state.lstTime;
	float vel = gVelocity.a;

	const float kP = 17, kI = 0.05;

	if (deltaTime >= 1)
	{
		state.input = vel;

		state.lstError = state.error;
		state.error = state.target - state.input;

		state.integral += state.error * deltaTime;
		if (state.integral < -8) state.integral = 0;

		state.power = state.error * kP + state.integral * kI;

		state.power += 26;

		if (state.power < 0) state.power /= 6.0;

		if (state.power > 50) state.power = 50;
		if (state.power < -5) state.power = -5;

		setDrive(state.power, -state.power);

		if (state.time >= state.nextDebug)
		{
			writeDebugStreamLine("%f %f %f", state.power, state.error, state.integral);
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

	if (deltaTime >= 1)
	{
		state.input = vel;

		state.lstError = state.error;
		state.error = state.target - state.input;

		state.integral += state.error * deltaTime;
		if (state.integral < -8) state.integral = 0;

		state.power = state.error * kP + state.integral * kI;

		state.power += 26;

		if (state.power < 0) state.power /= 6.0;

		if (state.power > 50) state.power = 50;
		if (state.power < -5) state.power = -5;

		setDrive(-state.power, state.power);

		if (state.time >= state.nextDebug)
		{
			writeDebugStreamLine("%f %f %f", state.power, state.error, state.integral);
			state.nextDebug = state.time + 20;

		}

		state.lstTime = state.time;
	}

	sleep(1);

	state.time = nPgmTime;
}
