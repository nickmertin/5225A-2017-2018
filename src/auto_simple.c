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
	} while (currentPosVector.y < (stopType & stopSoft ? -0.5 : 0) - dropEarly);

	if (stopType & stopSoft)
	{
		setDrive(-6, -6);
		sleep(20);
		setDrive(-5, -5);
		sleep(150);
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
