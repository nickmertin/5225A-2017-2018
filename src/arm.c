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

bool armFollowPath(sArmPath& path, tArmStates nextState)
{
	sArmPos oldTarget;
	oldTarget.x = gArmTarget.x;
	oldTarget.y = gArmTarget.y;
	gArmTarget = path.points[0].pos;
	armToTarget(false, false);

	sPID pidDist, pidPosOffset, pidVelOffset;

	pidInit(pidDist, 1, 0, 0, 0, 0, 0, 0);
	pidInit(pidPosOffset, 5, 0, 0, 0, 0, 0, 5);
	pidInit(pidVelOffset, 5, 0.01, 0, -1, -1, 0, 5);

	unsigned long startTime = nPgmTime;

	sCycleData cycle;
	initCycle(cycle, 50);

	for (int i = 0; i < path.pointsCount - 1;)
	{
		float degA = potiToDeg(*gSensor[armPotiA].value, A_VERTICAL);
		float degB = potiToDeg(*gSensor[armPotiB].value, B_VERTICAL);

		velocityCheck(armPotiA);
		velocityCheck(armPotiB);

		sArmPos pos;
		calculatePosition(pos, degA, degB);

		float _x = path.points[i].pos.x;
		float _y = path.points[i].pos.y;
		float dx = _x - pos.x;
		float dy = _y - pos.y;
		float distSq = dx * dx + dy * dy;

		while (i < path.pointsCount - 1)
		{
			_x = path.points[i + 1].pos.x;
			_y = path.points[i + 1].pos.y;
			float dxN = _x - pos.x;
			float dyN = _y - pos.y;
			float distNSq = dxN * dxN + dyN * dyN;

			if (distNSq < distSq)
			{
				i++;
				dx = dxN;
				dy = dyN;
				distSq = distNSq;
			}
			else break;
		}

		pidCalculate(pidPosOffset, 0, sqrt(distSq) * sin(atan2(dy, dx) - degToRad(path.points[i].direction)));

		float targetX = pidPosOffset.output;
		float targetY = 2.0;

		rotateDegrees(targetX, targetY, path.points[i].direction);

		targetX += pos.x;
		targetY += pos.y;

		S_LOG "[%.2f,%.2f] (%.2f,%.2f) -> (%.2f,%.2f)", _x, _y, pos.x, pos.y, targetX, targetY E_LOG_DATA

		float targetA = calculateTargetA(targetX, targetY);
		float targetB = calculateTargetB(targetX, targetY);

		float dA = targetA - degA;
		float dB = targetB - degB;

		float outA, outB;

		if (dA == 0)
		{
			outA = 0;
			outB = 127.0;
		}
		else if (dB == 0)
		{
			outA = 127.0;
			outB = 0;
		}
		else
		{
			float targetRatio = fabs(dA / dB);
			float trLog = log(targetRatio);

			float realRatio = (gSensor[armPotiA].velocity == 0 || gSensor[armPotiB].velocity == 0) ? targetRatio : fabs(gSensor[armPotiA].velocity / gSensor[armPotiB].velocity);
			float rrLog = log(realRatio);

			pidCalculate(pidVelOffset, trLog, rrLog);

			float outputLog = pidVelOffset.output + 0.4;// + trLog;

			S_LOG "%f | %f | %f", trLog, rrLog, outputLog E_LOG_DATA

			switch (sgn(outputLog))
			{
				case -1:
					outA = 127.0 * exp(outputLog);
					outB = 127.0;
					break;
				case 1:
					outA = 127.0;
					outB = 127.0 * exp(-outputLog);
					break;
				case 0:
					outA = outB = 127.0;
					break;
			}
		}

		S_LOG "OUT: %d %d", (int)outA, (int)outB E_LOG_DATA

		setArmF(outA * sgn(dA), outB * sgn(dB));

		skip:

		HANDLE_STATE_REQUEST(arm, setArm(0, 0);, return false;);

		endCycle(cycle);
	}

	gArmTarget.x = path.points[path.pointsCount - 1].pos.x;
	gArmTarget.y = path.points[path.pointsCount - 1].pos.y;
	DISABLE_TO_STATE(arm, armDoFollowPath);
	bool result = armToTarget(false, false);
	gArmTarget.x = oldTarget.x;
	gArmTarget.y = oldTarget.y;

	setArm(0, 0);
	ENABLE_TO_STATE(arm, nextState);
	return result;
}
