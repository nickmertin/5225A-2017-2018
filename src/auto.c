//void driveStraight(float &distance, sbyte power, bool safety, bool harshStop)
//{
//	S_LOG "Driving straight %f\"", distance E_LOG_INFO

//	DISABLE_TO_STATE(drive, driveAutomated)

//	resetQuadratureEncoder(leftEnc);
//	resetQuadratureEncoder(rightEnc);

//	sPID pid;
//	pidInit(pid, 100.0, 0.002, 0.01, 0.05, 2.0, -1, -1);

//	float target = distance;

//	unsigned long time = nPgmTime;

//	bool autoSafetyOld = gAutoSafety;

//	if (safety)
//		gAutoSafety = false;

//	sbyte maxPower = abs(power);

//	float left, right;

//	do
//	{
//		if (safety && checkDriveSafety(DRIVE_AUTO_VELOCITY_FAILED, DRIVE_AUTO_FAILED_TIME))
//		{
//			gAutoSafety = autoSafetyOld;
//			stopDrive();
//			return;
//		}

//		left = (float)*gSensor[leftEnc].value * IN_PER_TICK;
//		right = (float)*gSensor[rightEnc].value * IN_PER_TICK;

//		pidCalculate(pid, 0.0, left - right);

//		distance = abs(left + right) / 2.0;

//		float pLeft = power + pid.output;
//		float pRight = power - pid.output;

//		scalePower(pLeft, pRight, maxPower);

//		tankDrive((sbyte)pLeft, (sbyte)pRight);

//		sleep(2);
//	} while (!target || distance < target || sgn(left + right) != sgn(power));

//	if (safety)
//		gAutoSafety = autoSafetyOld;

//	if (harshStop)
//		applyHarshStop();

//	stopDrive();

//	ENABLE_TO_STATE(drive, driveUser);
//}

//void driveTurn(float leftDelta, float rightDelta, float range, sbyte limit, bool safety, bool harshStop)
//{
//	S_LOG "Turning, moving %f\" on the left and %f\" on the right", leftDelta, rightDelta E_LOG_DATA

//	DISABLE_TO_STATE(drive, driveAutomated)

//	resetQuadratureEncoder(leftEnc);
//	resetQuadratureEncoder(rightEnc);

//	sPID leftPid, rightPid;
//	pidInit(leftPid, 30.0, 0.01, 0.1, 0.05, 20.0, -1, -1);
//	pidInit(rightPid, 30.0, 0.01, 0.1, 0.05, 20.0, -1, -1);

//	unsigned long time = nPgmTime;

//	bool autoSafetyOld = gAutoSafety;

//	if (safety)
//		gAutoSafety = false;

//	sbyte maxPower = abs(limit);

//	float left = 0.0;
//	float right = 0.0;

//	do
//	{
//		if (safety && checkDriveSafety(DRIVE_AUTO_VELOCITY_FAILED, DRIVE_AUTO_FAILED_TIME))
//		{
//			gAutoSafety = autoSafetyOld;
//			stopDrive();
//			return;
//		}

//		left = (float)*gSensor[leftEnc].value * IN_PER_TICK;
//		right = (float)*gSensor[rightEnc].value * IN_PER_TICK;

//		pidCalculate(leftPid, leftDelta, left);
//		pidCalculate(rightPid, rightDelta, right);

//		float lPow = leftPid.output;
//		float rPow = rightPid.output;

//		scalePower(lPow, rPow, maxPower);

//		tankDrive((sbyte)(LIM_TO_VAL(lPow, 127)), (sbyte)(LIM_TO_VAL(rPow, 127)));

//		sleep(2);
//	} while (abs(left - leftDelta) > range || abs(right - rightDelta) > range);

//	if (safety)
//		gAutoSafety = autoSafetyOld;

//	if (harshStop)
//		applyHarshStop();

//	stopDrive();

//	ENABLE_TO_STATE(drive, driveUser)
//}

task autoMotorSensorUpdateTask()
{
	while (true)
	{
		updateMotors();
		updateSensorsLst();
		updateSensorInputs();
		updateSensorOutputs();
		sleep(CYCLE_MAX_TIME);
	}
}

task autoSafetyTask()
{
	unsigned long time = nPgmTime;
	while (true)
	{
		//if (gAutoSafety && checkDriveSafety(DRIVE_AUTO_VELOCITY_FAILED, DRIVE_AUTO_FAILED_TIME))
		//{
		//	stopAllButCurrentTasks();
		//	stopAllMotors();
		//	S_LOG "Auto safety triggered" E_LOG_EROR
		//}
		//unsigned long delta = nPgmTime - time;
		//time += CYCLE_MAX_TIME;
		//if (delta >= CYCLE_MAX_TIME)
		//	S_LOG "Auto safetly cycle took %d ms; should be less than %d", delta, CYCLE_MAX_TIME E_LOG_WARN
		//else
		//	sleep(CYCLE_MAX_TIME - delta);
	}
}

//void applyHarshStop()
//{
//	int left = *gSensor[leftEnc].value;
//	int right = *gSensor[rightEnc].value;
//	sleep(50);
//	updateSensorInputs();
//	left -= *gSensor[leftEnc].value;
//	right -= *gSensor[rightEnc].value;
//	S_LOG "Applying harsh stop: %d %d", left, right E_LOG_DATA
//	tankDrive(sgn(left) * AUTO_BREAK_POWER, sgn(right) * AUTO_BREAK_POWER);
//	updateMotors();
//	sleep(250);
//	stopDrive();
//	updateMotors();
//}

void scalePower(float &leftPow, float &rightPow, sbyte max)
{
	float _max = (float)abs(max);
	if (fabs(leftPow) > _max || fabs(rightPow) > _max)
	{
		float powScaler = _max / MAX(fabs(leftPow), fabs(rightPow));
		leftPow *= powScaler;
		rightPow *= powScaler;

		if (fabs(leftPow) > _max) leftPow = _max * sgn(leftPow);
		if (fabs(rightPow) > _max) rightPow = _max * sgn(rightPow);
	}
}

//void waitForAccelStable()
//{
//	int count = 0;
//	unsigned long time = nPgmTime;
//	do
//	{
//		velocityCheck(accelH);
//		velocityCheck(accelV);
//		if (abs(*gSensor[accelV].value - 1000) <= 10 && abs(*gSensor[accelH].value) <= 10 && abs(gSensor[accelV].velocity) <= 0.1 && abs(gSensor[accelH].velocity) <= 0.1)
//			++count;
//		else
//			count = 0;

//		unsigned long timeDelta = nPgmTime - time;
//		if (timeDelta > CYCLE_MAX_TIME)
//		{
//			S_LOG "waitForAccelStable cycle took %d ms; max is %d", timeDelta, CYCLE_MAX_TIME E_LOG_WARN
//			time = nPgmTime;
//		}
//		else
//		{
//			sleep(CYCLE_MAX_TIME - timeDelta);
//			time += CYCLE_MAX_TIME;
//		}
//	} while (count < 10);
//}
