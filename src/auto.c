/* Functions */
void trackPosition(int left, int right, int back, sPos& position)
{
	float L = (left - position.leftLst) * SPIN_TO_IN_LR; // The amount the left side of the robot moved
	float R = (right - position.rightLst) * SPIN_TO_IN_LR; // The amount the right side of the robot moved
	float S = (back - position.backLst) * SPIN_TO_IN_S; // The amount the back side of the robot moved

	// Update the last values
	position.leftLst = left;
	position.rightLst = right;
	position.backLst = back;

	float h; // The hypotenuse of the triangle formed by the middle of the robot on the starting position and ending position and the middle of the circle it travels around
	float i; // Half on the angle that I've traveled
	float h2; // The same as h but using the back instead of the side wheels
	float a = (L - R) / (L_DISTANCE_IN + R_DISTANCE_IN); // The angle that I've traveled
	if (a)
	{
		float r = R / a; // The radius of the circle the robot travel's around with the right side of the robot
		i = a / 2.0;
		float sinI = sin(i);
		h = ((r + R_DISTANCE_IN) * sinI) * 2.0;

		float r2 = S / a; // The radius of the circle the robot travel's around with the back of the robot
		h2 = ((r2 + S_DISTANCE_IN) * sinI) * 2.0;
	}
	else
	{
		h = R;
		i = 0;

		h2 = S;
	}
	float p = i + position.a; // The global ending angle of the robot
	float cosP = cos(p);
	float sinP = sin(p);

	// Update the global position
	position.y += h * cosP;
	position.x += h * sinP;

	position.y += h2 * -sinP; // -sin(x) = sin(-x)
	position.x += h2 * cosP; // cos(x) = cos(-x)

	position.a += a;
}

void resetPosition(sPos& position)
{
	position.leftLst = position.rightLst = position.backLst = 0;
	position.y = position.x = position.a = 0;
}

void resetVelocity(sVel& velocity, sPos& position)
{
	velocity.a = velocity.y = velocity.x = 0;

	velocity.lstPosA = position.a;
	velocity.lstPosY = position.y;
	velocity.lstPosX = position.x;

	velocity.lstChecked = nPgmTime;
}

void trackVelocity(sPos position, sVel& velocity)
{
	unsigned long curTime = nPgmTime;
	long passed = curTime - velocity.lstChecked;
	if (passed > 40)
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
		updateSensorInput(trackL);
		updateSensorInput(trackR);
		updateSensorInput(trackB);
		trackPosition(gSensor[trackL].value, gSensor[trackR].value, gSensor[trackB].value, gPosition);
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

		if (DATALOG_BATTERY != -1)
		{
			tHog();
			datalogDataGroupStart();
			datalogAddValue(DATALOG_BATTERY + 0, nImmediateBatteryLevel);
			datalogAddValue(DATALOG_BATTERY + 2, BackupBatteryLevel);
			datalogDataGroupEnd();
			tRelease();
		}

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
	aPow *= -6.3;

	word left = yPow + aPow;
	word right = yPow - aPow;

	left = sgn(left) * MAX(fabs(left), 7);
	right = sgn(right) * MAX(fabs(right), 7);

	LIM_TO_VAL_SET(left, 30);
	LIM_TO_VAL_SET(right, 30);

	writeDebugStreamLine("Applying harsh stop: %d %d", left, right);
	setDrive(left, right);
	updateMotors();
	sleep(150);
	setDrive(0, 0);
	updateMotors();
}

void resetPositionFull(sPos& position, float y, float x, float a)
{
	tStop(trackPositionTask);
	writeDebugStreamLine("Resetting position %f %f %f | %f %f %f", position.y, position.x, radToDeg(fmod(gPosition.a, PI * 2)), y, x, radToDeg(fmod(a, PI * 2)));
	resetPosition(position);

	resetQuadratureEncoder(trackL);
	resetQuadratureEncoder(trackR);
	resetQuadratureEncoder(trackB);

	position.y = y;
	position.x = x;
	position.a = a;
	tStart(trackPositionTask);
}
