/* Functions */
void trackPosition(int left, int right, sPos& position)
{
	float L = (left - position.leftLst) * SPIN_TO_IN; // The amount the left side of the robot moved
	float R = (right - position.rightLst) * SPIN_TO_IN; // The amount the right side of the robot moved

	// Update the last values
	position.leftLst = left;
	position.rightLst = right;

	float h; // The hypotenuse of the triangle formed by the middle of the robot on the starting position and ending position and the middle of the circle it travels around
	float i; // Half on the angle that I've traveled
	float a = (L - R) / (DRIVE_WIDTH_IN); // The angle that I've traveled
	if (a)
	{
		float r = R / a; // The radius of the circle the robot travel's around with the right side of the robot
		i = a / 2.0;
		float sinI = sin(i);
		h = ((r + DRIVE_WIDTH_IN / 2) * sinI) * 2.0;
	}
	else
	{
		h = R;
		i = 0;
	}
	float p = i + position.a; // The global ending angle of the robot
	float cosP = cos(p);
	float sinP = sin(p);

	// Update the global position
	position.y += h * cosP;
	position.x += h * sinP;

	position.a += a;
}

void resetPosition(sPos& position)
{
	position.leftLst = position.rightLst = 0;
	position.y = position.x = position.a = 0;
}

void trackVelocity(sPos position, sVel& velocity)
{
	unsigned long curTime = nPgmTime;
	long passed = curTime - velocity.lstChecked;
	if (passed > 50)
	{
		float posA = position.a;
		float posY = position.y;
		float posX = position.x;
		velocity.a = ((posA - velocity.lstPosA) * 1000.0) / passed;
		velocity.y = ((posY - velocity.lstPosY) * 1000.0) / passed;
		velocity.x = ((posX - velocity.lstPosX) * 1000.0) / passed;

		velocity.lstPosA = posA;
		velocity.lstPosY = posY;
		velocity.lstPosX = posX;
		velocity.lstChecked = curTime;
	}
}

void vectorToPolar(sVector& vector, sPolar& polar)
{
	if (vector.x || vector.y)
	{
		polar.magnitude = sqrt(vector.x * vector.x + vector.y * vector.y);
		polar.angle = atan2(vector.y, vector.x);
	}
	else
		polar.magnitude = polar.angle = 0;
}

void polarToVector(sPolar& polar, sVector& vector)
{
	if (polar.magnitude)
	{
		vector.x = polar.magnitude * cos(polar.angle);
		vector.y = polar.magnitude * sin(polar.angle);
	}
	else
		vector.x = vector.y = 0;
}

void transformVelocityToLocal(sVel& global, sVector& local, float angle)
{
	sVector vel;
	vel.x = global.x;
	vel.y = global.y;
	sPolar polar;
	vectorToPolar(vel, polar);
	polar.angle += angle;
	polarToVector(polar, local);
}

void transformVelocityToGlobal(sVel& local, sVector& global, float angle)
{
	sVector vel;
	vel.x = global.x;
	vel.y = global.y;
	sPolar polar;
	vectorToPolar(vel, polar);
	polar.angle -= angle;
	polarToVector(polar, global);
}

float getAngleOfLine(sLine line)
{
	return atan2(line.p2.x - line.p1.x, line.p2.y - line.p1.y);
}

float getLengthOfLine(sLine line)
{
	float x = line.p2.x - line.p1.x;
	float y = line.p2.y - line.p1.y;
	return sqrt(x * x + y * y);
}

task trackPositionTask()
{
	while (true)
	{
		updateSensorInput(leftEnc);
		updateSensorInput(rightEnc);
		trackPosition(gSensor[leftEnc].value, gSensor[rightEnc].value, gPosition);
		trackVelocity(gPosition, gVelocity);
		sleep(1);
	}
}

task autoMotorSensorUpdateTask()
{
	sCycleData cycle;
	initCycle(cycle, 10, "auto motor/sensor");
	while (true)
	{
		updateMotors();
		updateSensorInputs();
		updateSensorOutputs();
		endCycle(cycle);
	}
}

void applyHarshStop()
{
	sVector vel;
	vel.x = gVelocity.x;
	vel.y = gVelocity.y;
	sPolar polarVel;
	vectorToPolar(vel, polarVel);
	polarVel.angle += gPosition.a;
	polarToVector(polarVel, vel);
	float yPow = vel.y, aPow = gVelocity.a;

	writeDebugStreamLine("Vel y | a: %f | %f", yPow, aPow);

	yPow *= -0.7;
	aPow *= -3;

	word left = yPow + aPow;
	word right = yPow - aPow;

	left = sgn(left) * MAX(fabs(left), 10);
	right = sgn(right) * MAX(fabs(right), 10);

	LIM_TO_VAL_SET(left, 50);
	LIM_TO_VAL_SET(right, 50);

	writeDebugStreamLine("Applying harsh stop: %f %f", left, right);
	setDrive(left, right);
	updateMotors();
	sleep(150);
	setDrive(0, 0);
	updateMotors();
}

void resetPositionFull(sPos& position, float y, float x, float a) { resetPositionFullRad(position, y, x, degToRad(a)); }

void resetPositionFullRad(sPos& position, float y, float x, float a)
{
	writeDebugStreamLine("Resetting position (%f %f) %f degrees)", position.x, position.y, degToRad(position.a));
	hogCPU();
	resetPosition(position);

	resetQuadratureEncoder(leftEnc);
	resetQuadratureEncoder(rightEnc);

	position.y = y;
	position.x = x;
	position.a = a;
	releaseCPU();
}

float kP = 2.0, kI = 0.0, kD = 0.0, kIInner = PI / 6, kIOuter = PI;

void moveToTarget(float y, float x, float ys, float xs, byte power, float delta, float lineEpsilon, float targetEpsilon, bool harshStop, bool slow)
{
	writeDebugStreamLine("Moving to %f %f from %f %f", y, x, ys, xs);
	sPID pidA, pidY;
	pidInit(pidA, kP, kI, kD, kIInner, kIOuter, -1, -1);
	pidInit(pidY, 0.2, 0.01, 0.0, 0.5, 1.5, -1, 1.0);

	lineEpsilon = MIN(lineEpsilon, targetEpsilon);

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

	// The position relative to the line
	sVector localPos;
	sPolar polar;

	targetEpsilon *= targetEpsilon;

	sCycleData cycle;
	initCycle(cycle, 50, "moveToTarget");
	do
	{
		// Setup our current displacement
		localPos.x = gPosition.x - xs;
		localPos.y = gPosition.y - ys;

		vectorToPolar(localPos, polar);
		polar.angle += lineAngle;
		polarToVector(polar, localPos);

		EndTimeSlice();

		bool closeToLine = fabs(localPos.x) < lineEpsilon || localPos.y > lineLength - MAX(lineEpsilon, delta);

		float currentDelta;
		if (localPos.y < -delta) currentDelta = -localPos.y;
		else if (localPos.y > lineLength - delta) currentDelta = lineLength - localPos.y;
		else currentDelta = delta;
		float target = closeToLine ? pidAngle : pidAngle - atan2(localPos.x, currentDelta);

		EndTimeSlice();

		//pidCalculate(pidA, target, nearAngle((gVelocity.x * gVelocity.x + gVelocity.y * gVelocity.y > 0.1 && gVelocity.a < 0.5) ? atan2(gVelocity.x, gVelocity.y) : power > 0 ? gPosition.a : gPosition.a + PI, target));
		pidCalculate(pidA, target, nearAngle(gPosition.a, target));

		EndTimeSlice();

		float basePower;
		if (slow)
		{
			pidCalculate(pidY, lineLength, localPos.y);
			basePower = fabs(pidY.output * power);
		}
		else
			basePower = fabs((float) power);

		EndTimeSlice();

		float weight = pidA.output;//sgn(pidA.output) * pow(fabs(pidA.output), 0.2);
		//if (closeToLine) weight *= 0.5;

		float scalar = basePower / (1 + fabs(weight));

		word left = (word)(scalar * (sgn(power) + weight));
		word right = (word)(scalar * (sgn(power) - weight));

		EndTimeSlice();

		writeDebugStreamLine("Global %.2f %.2f %.2f | %.2f %.2f Local %.2f %.2f | %.2f %.2f %.2f | %d %d", gPosition.y, gPosition.x, radToDeg(gPosition.a), gVelocity.y, gVelocity.x, localPos.y, localPos.x, radToDeg(target - pidA.error), radToDeg(target), weight, left, right);

		if (cycle.count) setDrive(left, right);

		// Get distance from target position

		localPos.y -= lineLength;

		endCycle(cycle);
	} while (localPos.y < 0 && localPos.x * localPos.x + localPos.y * localPos.y > targetEpsilon);

	if (harshStop)
		applyHarshStop();
	else
		setDrive(0, 0);

	writeDebugStreamLine("Moved to %f %f from %f %f | %f %f %f", y, x, ys, xs, gPosition.y, gPosition.x, radToDeg(gPosition.a));
}

void moveToTarget(float y, float x, byte power, float delta, float lineEpsilon, float targetEpsilon, bool harshStop, bool slow) { moveToTarget(y, x, gPosition.y, gPosition.x, power, delta, lineEpsilon, targetEpsilon, harshStop, slow); }

void turnToAngleRad(float a, tTurnDir turnDir, byte left, byte right, bool harshStop, bool slow)
{
	writeDebugStreamLine("Turning to %f", radToDeg(a));
	sPID pidA;
	pidInit(pidA, 70.0, 0.0, 0.0, -1, -1, 46, -1);
	left = abs(left);
	right = abs(right);

	if (turnDir == ch)
		if (a < gPosition.a) turnDir = ccw; else turnDir = cw;

	if (slow)
	{
		switch (turnDir)
		{
			case cw:
				while (gPosition.a < a - (gVelocity.a * 0.090))
				{
					pidCalculate(pidA, a, gPosition.a);
					int power = round(abs(pidA.output));
					LIM_TO_VAL_SET(power, 127);
					setDrive(power > left ? left : power, power > right ? -right : -power);
					sleep(1);
				}
				break;
			case ccw:
				while (gPosition.a > a - (gVelocity.a * 0.090))
				{
					pidCalculate(pidA, a, gPosition.a);
					int power = round(abs(pidA.output));
					LIM_TO_VAL_SET(power, 127);
					setDrive(power > left ? -left : -power, power > right ? right : power);
					sleep(1);
				}
				break;
		}
	}
	else
	{
		switch (turnDir)
		{
			case cw:
				setDrive(left, -right);
				while (gPosition.a < a - (gVelocity.a * 0.13)) sleep(1);
				break;
			case ccw:
				setDrive(-left, right);
				while (gPosition.a > a - (gVelocity.a * 0.13)) sleep(1);
				break;
		}
	}

	if (harshStop)
		applyHarshStop();

	setDrive(0, 0);

	writeDebugStreamLine("Turned to %f | %f %f %f", radToDeg(a), gPosition.y, gPosition.x, radToDeg(gPosition.a));
}

void turnToAngle(float a, tTurnDir turnDir, byte left, byte right, bool harshStop, bool slow)
{
	turnToAngleRad(degToRad(a), turnDir, left, right, harshStop, slow);
}

void turnToTarget(float y, float x, tTurnDir turnDir, byte left, byte right, bool harshStop, bool slow, float offset)
{
	turnToTarget(y, x, gPosition.y, gPosition.x, turnDir, left, right, harshStop, slow, offset);
}

void turnToTarget(float y, float x, float ys, float xs, tTurnDir turnDir, byte left, byte right, bool harshStop, bool slow, float offset)
{
	writeDebugStreamLine("Turning to %f %f from %f %f", y, x, ys, xs);

	offset = degToRad(offset);

	sPID pidA;
	pidInit(pidA, 70.0, 0.0, 0.0, -1, -1, 46, -1);
	left = abs(left);
	right = abs(right);

	float aOffset = 0;
	float orA = gPosition.a;
	float a = getTargetAngle(y, x, ys, xs) + offset;
	a = round((orA - a) / (2 * PI)) * (2 * PI) + a;
	if (turnDir == cw)
		while (a + aOffset < orA) aOffset += PI * 2;
	else if (turnDir == ccw)
		while (a + aOffset > orA) aOffset -= PI * 2;
	else
		if (a < orA) turnDir = ccw; else turnDir = cw;
	a += aOffset;

	writeDebugStreamLine("Turn angle | Angle: %f | %f", radToDeg(a), radToDeg(orA));

	if (slow)
	{
		sCycleData cycle;
		initCycle(cycle, 10, "moveToTarget");
		switch (turnDir)
		{
			case cw:
				while (gPosition.a < a - (gVelocity.a * 0.090))
				{
					a = getTargetAngle(y, x, ys, xs) + offset;
					a = round((orA - a) / (2 * PI)) * (2 * PI) + a;
					a += aOffset;
					pidCalculate(pidA, a, gPosition.a);
					int power = round(abs(pidA.output));
					LIM_TO_VAL_SET(power, 127);
					setDrive(power > left ? left : power, power > right ? -right : -power);
					endCycle(cycle);
				}
				break;
			case ccw:
				while (gPosition.a > a - (gVelocity.a * 0.090))
				{
					a = getTargetAngle(y, x, ys, xs) + offset;
					a = round((orA - a) / (2 * PI)) * (2 * PI) + a;
					a += aOffset;
					pidCalculate(pidA, a, gPosition.a);
					int power = round(abs(pidA.output));
					LIM_TO_VAL_SET(power, 127);
					setDrive(power > left ? -left : -power, power > right ? right : power);
					endCycle(cycle);
				}
				break;
		}
	}
	else
	{
		switch (turnDir)
		{
			case cw:
				setDrive(left, -right);
				while (gPosition.a < a - (gVelocity.a * 0.13)) { a = getTargetAngle(y, x, ys, xs) + aOffset; sleep(10); }
				break;
			case ccw:
				setDrive(-left, right);
				while (gPosition.a > a - (gVelocity.a * 0.13)) { a = getTargetAngle(y, x, ys, xs) + aOffset; sleep(10); }
				break;
		}
	}

	if (harshStop)
		applyHarshStop();

	setDrive(0, 0);

	writeDebugStreamLine("Turned to %f %f from %f %f | %f %f %f", y, x, ys, xs, gPosition.y, gPosition.x, radToDeg(gPosition.a));
}

float getTargetAngle(float y, float x, float ys, float xs)
{
	sLine line;
	line.p1.y = ys;
	line.p1.x = xs;
	line.p2.y = y;
	line.p2.x = x;
	return getAngleOfLine(line);
}

float getDistanceFromPoint(sVector point)
{
	return sqrt(sq(gPosition.x - point.x) + sq(gPosition.y - point.y));
}
