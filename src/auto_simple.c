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
	} while (currentPosVector.y < ((stopType & stopSoft) ? (-gVelocity.y * 0.240) : 0) - dropEarly - (gVelocity.y * 0.098));

	if (stopType & stopSoft)
	{
		setDrive(-5, -5);
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

	left = abs(left);
	right = abs(right);

	if (turnDir == ch)
		if (a < gPosition.a) turnDir = ccw; else turnDir = cw;

	switch (turnDir)
	{
	case cw:
		setDrive(left, -right);
		while (gPosition.a < a - gVelocity.a * 0.4) sleep(1);
		writeDebugStreamLine("%f", gVelocity.a);
		turnCw.target = 0.900;
		turnSimpleInternalCw (a);
		break;
	case ccw:
		setDrive(-left, right);
		while (gPosition.a > a + gVelocity.a * 0.4) sleep(1);
		writeDebugStreamLine("%f", gVelocity.a);
		turnCcw.target = 0.900;
		turnSimpleInternalCcw (a);
		break;
	}

	writeDebugStreamLine("Turned to %f | %f %f %f", radToDeg(a), gPosition.y, gPosition.x, radToDeg(gPosition.a));
}

void turnToAngleSimple(float a, tTurnDir turnDir, byte left, byte right)
{
	turnToAngleRadSimple(degToRad(a), turnDir, left, right);
}

void turnSimpleInternalCw(float a)
{
	while (gPosition.a < a + degToRad(-5.3 + (turnCw.target - gVelocity.a) * 0.3) || gVelocity.a > 1.2)
	{
		unsigned long deltaTime = turnCw.time - turnCw.lstTime;
		float vel = gVelocity.a;

		if (deltaTime >= 1)
		{
			turnCw.input = vel;

			turnCw.lstError = turnCw.error;
			turnCw.error = turnCw.target - turnCw.input;

			turnCw.integral += turnCw.error * deltaTime;
			if (turnCw.integral < -8) turnCw.integral = 0;

			turnCw.power = turnCw.error * kP + turnCw.integral * kI;

			turnCw.power += 26;

			if (turnCw.power < 0) turnCw.power /= 6.0;

			if (turnCw.power > 50) turnCw.power = 50;
			if (turnCw.power < -5) turnCw.power = -5;

			setDrive(turnCw.power, -turnCw.power);

			if (turnCw.time >= turnCw.nextDebug)
			{
				writeDebugStreamLine("%f %f %f", turnCw.power, turnCw.error, turnCw.integral);
				turnCw.nextDebug = turnCw.time + 20;

			}

			turnCw.lstTime = turnCw.time;
		}

		sleep(1);

		turnCw.time = nPgmTime;
	}
	setDrive(-15, 15);
	sleep(150);
	setDrive(0, 0);
}

void turnSimpleInternalCcw(float a)
{
	while (gPosition.a > a - degToRad(-5.3 + (turnCcw.target - gVelocity.a) * 0.3) || gVelocity.a < -1.2)
	{
		unsigned long deltaTime = turnCcw.time - turnCcw.lstTime;
		float vel = gVelocity.a;

		if (deltaTime >= 1)
		{
			turnCcw.input = vel;

			turnCcw.lstError = turnCcw.error;
			turnCcw.error = turnCcw.target - turnCcw.input;

			turnCcw.integral += turnCcw.error * deltaTime;
			if (turnCcw.integral < -8) turnCcw.integral = 0;

			turnCcw.power = turnCcw.error * kP + turnCcw.integral * kI;

			turnCcw.power += 26;

			if (turnCcw.power < 0) turnCcw.power /= 6.0;

			if (turnCcw.power > 50) turnCcw.power = 50;
			if (turnCcw.power < -5) turnCcw.power = -5;

			setDrive(-turnCcw.power, turnCcw.power);

			if (turnCcw.time >= turnCcw.nextDebug)
			{
				writeDebugStreamLine("%f %f %f", turnCcw.power, turnCcw.error, turnCcw.integral);
				turnCcw.nextDebug = turnCcw.time + 20;

			}

			turnCcw.lstTime = turnCcw.time;
		}

		sleep(1);

		turnCcw.time = nPgmTime;
	}
	setDrive(15, -15);
	sleep(150);
	setDrive(0, 0);
}
