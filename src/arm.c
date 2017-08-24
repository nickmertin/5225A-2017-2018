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
	return (Dsq > BEAM_MAX_SQ) ? atan2(y, x) * 180 / PI + 90 : ((acos(((BEAM_R * BEAM_R) + Dsq - (BEAM_S * BEAM_S)) / (2 * BEAM_R * sqrt(Dsq))) * 180 / PI) + 90 + (atan2(y, x) * 180 / PI));
}

float calculateTargetB(float x, float y)
{
	float Dsq = (x * x) + (y * y);
	return (Dsq > BEAM_MAX_SQ) ? 180 : (acos(((BEAM_R * BEAM_R) + (BEAM_S * BEAM_S) - Dsq) / (2 * BEAM_R * BEAM_S)) * 180 / PI);
}

float potiToDeg(short poti, short cal)
{
	return (float) (poti + (CAL_BASE * DEG_PER_POT) - cal) * POT_PER_DEG;
}

float degToPoti(float deg, short cal)
{
	return (deg * DEG_PER_POT) - (CAL_BASE * DEG_PER_POT) + cal;
}

void currentArmPosition(sArmPos& position)
{
	calculatePosition(position, potiToDeg(*gSensor[armPotiA].value, A_VERTICAL), potiToDeg(*gSensor[armPotiB].value, B_VERTICAL));
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

bool armToTarget(bool safety, bool hold)
{
	sPID pidA, pidB;

	pidInit(pidA, 5, 0/*.01*/, 0/*.02*/, 2, 35, 0, 127);
	pidInit(pidB, 4, 0/*.01*/, 0/*.02*/, 2, 35, 0, 127);

	while (true)
	{
		float degA = potiToDeg(*gSensor[armPotiA].value, A_VERTICAL);
		float degB = potiToDeg(*gSensor[armPotiB].value, B_VERTICAL);

		sArmPos cur;
		calculatePosition(cur, degA, degB);

		float targetA = calculateTargetA(gArmTarget.x, gArmTarget.y);
		float targetB = calculateTargetB(gArmTarget.x, gArmTarget.y);

		pidCalculate(pidA, targetA, degA);
		pidCalculate(pidB, targetB, degB);

		setArmF(pidA.output, pidB.output);

		if (!hold && fabs(pidA.error) < 0.5)
		{
			return true;
		}

		HANDLE_STATE_REQUEST(arm, setArm(0, 0);, return fabs(pidA.error) < 0.5;)

		sleep(10);
	}
	return false;
}

bool armFollowParabola(float a, float b, float c, float targetX, tArmStates nextState)
{
	sPID pidDist, pidVelA, pidVelB;

	pidInit(pidDist, 1, 0, 0, 0, 0, 0, 0);
	pidInit(pidVelA, 10_000, 0, 0, 0, 0, 0, 0);
	pidInit(pidVelA, 10_000, 0, 0, 0, 0, 0, 0);

	float targetY = targetX * (a * targetX + b) + c;

	float a_3 = a / 3.0;
	float ac_3 = a_3 * c;
	float b2_12 = b * b / 12.0;
	float a2 = a * a;
	float a3 = a2 * a;
	float _1_a6 = 1.0 / (a3 * a3);
	float _1_4a2 = 0.25 / a2;
	float b_8a3 = b / (8.0 * a3);
	float b_2a = b / (2.0 * a);
	float psi = ac_3 - b2_12 + 1.0 / 6.0;

	sArmPos lastPos;
	currentArmPosition(lastPos);

	velocityClear(armPotiA);
	velocityClear(armPotiB);

	unsigned long startTime = nPgmTime;

	sCycleData cycle;
	initCycle(cycle, 50);

	while (true)
	{
		float degA = potiToDeg(*gSensor[armPotiA].value, A_VERTICAL);
		float degB = potiToDeg(*gSensor[armPotiB].value, B_VERTICAL);

		velocityCheck(armPotiA);
		velocityCheck(armPotiB);

		sArmPos pos;
		calculatePosition(pos, degA, degB);

		float phi = pos.x * _1_4a2 + b_8a3;
		float rho = psi - a_3 * pos.y;
		float eta = sqrt(rho * rho * rho * _1_a6 + phi * phi);
		float closestX = pow(phi + eta, 1.0 / 3.0) + pow(phi - eta, 1.0 / 3.0) - b_2a;
		float closestY = closestX * (a * closestX + b) + c;
		float dist = sqrt(closestX * closestX + closestY * closestY);

		float deltaX = targetX - pos.x;
		float deltaY = targetY - pos.y;
		float delta = sqrt(deltaX * deltaX + deltaY * deltaY);

		if (delta < 0.5)
			break;

		pidCalculate(pidDist, 0, pos.y > closestY ? dist : -dist);

		float m = 2 * a * closestX + b;
		float m2 = m * m;

		float v1x = sqrt(fabs(pidDist.output) / (1.0 / m2 + 1)) * sgn(pidDist.output);
		float v1y = -v1x / m;

		float v2_mag = fabs(sin(atan(m) - atan2(deltaY, deltaX))) * delta;
		v2_mag = MIN(v2_mag, 1.0);

		float v2x = sqrt(v2_mag / (m2 + 1));
		if (sgn(m) == SGN_OF_DIFF(targetX, closestX))
			v2x = -v2x;

		float v2y = m * v2x;

		float vx = v1x + v2x;
		float vy = v1y + v2y;

		float degAB = degA + degB;
		float sinA = sinDegrees(degA);
		float cosA = cosDegrees(degA);
		float sinAB = sinDegrees(degAB);
		float cosAB = cosDegrees(degAB);

		float den = sinA * cosAB - cosA * sinAB;
		float D = sqrt((BEAM_R * BEAM_R) + (BEAM_S * BEAM_S) - (2 * BEAM_R * BEAM_S * cosDegrees(degB)));

		float wA = (vy * cosAB - vx * sinAB) / (D * den);
		float wB = (vx * sinA - vy * cosA) / (BEAM_S * den);

		pidCalculate(pidVelA, wA, gSensor[armPotiA].velocity * DEG_PER_POT * PI / 180.0);
		pidCalculate(pidVelA, wB, gSensor[armPotiB].velocity * DEG_PER_POT * PI / 180.0);

		float target = 127 * v2_mag;
		float outA, outB;

		if (fabs(pidVelA.output) > fabs(pidVelB.output))
		{
			outB = target / fabs(pidVelA.output) * pidVelB.output;
			outA = pidVelA.output > 0 ? target : -target;
		}
		else
		{
			outA = target / fabs(pidVelB.output) * pidVelA.output;
			outB = pidVelB.output > 0 ? target : -target;
		}

		setArmF(outA, outB);

		HANDLE_STATE_REQUEST(arm, setArm(0, 0);, return false);

		endCycle(cycle);
	}

	S_LOG "armFollowParabola cycles: %d average cycle time: %d ms", nPgmTime - startTime, (nPgmTime - startTime) / cycle.count E_LOG_INFO

	setArm(0, 0);
	return true;
}
