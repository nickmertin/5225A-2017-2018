void calculatePosition(sArmPos& position, float a, float b)
{
	float cb = cosDegrees(b);
	float Scb = BEAM_S * cb;
	float D = sqrt((BEAM_R * BEAM_R) + (BEAM_S * BEAM_S) - (2 * BEAM_R * Scb));
	float g = acos((BEAM_R - Scb) / D) * 180 / PI;
	position.x = D * sinDegrees(a - g);
	position.y = D * -cosDegrees(a - g);
}


float calculateTargetA(float x, float y)
{
	float Dsq = (x * x) + (y * y);
	float D = sqrt(Dsq);
	return (acos(((BEAM_R * BEAM_R) + Dsq - (BEAM_S * BEAM_S)) / (2 * BEAM_R * D)) * 180 / PI) + 90 + (atan2(y, x) * 180 / PI);
}

float calculateTargetB(float x, float y)
{
	return acos(((BEAM_R * BEAM_R) + (BEAM_S * BEAM_S) - (x * x) - (y * y)) / (2 * BEAM_R * BEAM_S)) * 180 / PI;
}

float potiToDeg(short poti, short cal)
{
	return (poti + (CAL_BASE * DEG_PER_POT) - cal) / (float)DEG_PER_POT;
}

float degToPoti(float deg, short cal)
{
	return (deg * DEG_PER_POT) - (CAL_BASE * DEG_PER_POT) + cal;
}

void setArm(sbyte powerA, sbyte powerB)
{
	gMotor[armAR].power = gMotor[armAL].power = powerA;
	gMotor[armB].power = powerB;
}

void setArmF(float powerA, float powerB)
{
	setArm((sbyte) powerA, (sbyte) powerB);
}

bool armToTarget(sArmPos& target, bool safety, bool hold)
{
	sPID pidA, pidB;

	pidInit(pidA, 3, 0, 0, 0, 0, 0, 127);
	pidInit(pidB, 2.5, 0, 0, 0, 0, 0, 127);

	while(true) {
		float degA = potiToDeg(SensorValue[armPotiA], A_VERTICAL);
		float degB = potiToDeg(SensorValue[armPotiB], B_VERTICAL);

		float targetA = calculateTargetA(target.x, target.y);
		float targetB = calculateTargetB(target.x, target.y);

		pidCalculate(pidA, targetA, degA);
		pidCalculate(pidB, targetB, degB);

		setArmF(pidA.output, pidB.output);

		sleep(10);
	}
}
