/* Functions */
bool correctBtnIn(tSensors sen)
{
	int value = SensorValue[sen];
	return value >= gSensor[sen].dgtMin && value <= gSensor[sen].dgtMax;
}

void updateSensorOutput(tSensors sen)
{
	SensorValue[sen] = gSensor[sen].lstValue = gSensor[sen].value;
}

void updateSensorOutputs()
{
	for (ubyte i = 0; i < kNumbOfTotalSensors; ++i)
	{
		if (gSensor[i].cls == snclsOutput)
			updateSensorOutput(i);
	}
}

void updateSensorInput(tSensors sen)
{
	gSensor[sen].lstValue = gSensor[sen].value;

	int raw = gSensor[sen].rawValue = Sensorvalue[sen];

	switch (gSensor[sen].mode)
	{
	case snmdDgtIn:
		gSensor[sen].value = correctBtnIn(sen);
		break;
	case snmdInverted:
		gSensor[sen].value = !raw;
		break;
	default:
		gSensor[sen].value = raw;
		break;
	}

#ifdef CHECK_POTI_JUMPS
	if (SensorType[sen] == sensorPotentiometer && abs(gSensor[sen].value - gSensor[sen].lstValue) > 400 && ++gSensor[sen].filterAcc < 10)
	{
		if (gSensor[sen].filterAcc == 1)
			writeDebugStreamLine("%d port %d jumped from %d to %d", nPgmTime, sen - port1 + 1, gSensor[sen].lstValue, gSensor[sen].value);
		gSensor[sen].value = gSensor[sen].lstValue;
	}
	else
		gSensor[sen].filterAcc = 0;
#endif
}

void updateSensorInputs()
{
	for (ubyte i = 0; i < kNumbOfTotalSensors; ++i)
	{
		if (gSensor[i].cls == snclsInput)
			updateSensorInput(i);
	}
}

tSensorClass checkSenClass(tSensors sen)
{
	if (SensorType[sen] == sensorNone)
		return snclsNone;
	else if (SensorType[sen] == sensorDigitalOut || SensorType[sen] == sensorLEDtoVCC)
		return snclsOutput;
	else
		return snclsInput;
}

void setupDgtIn(tSensors sen, int min, int max)
{
	gSensor[sen].mode = snmdDgtIn;
	gSensor[sen].dgtMin = min;
	gSensor[sen].dgtMax = max;
}

void setupInvertedSen(tSensors sen)
{
	gSensor[sen].mode = snmdInverted;
}

void resetQuadratureEncoder(tSensors sen)
{
	gSensor[sen].value = 0;
	SensorValue[sen] = 0;
}

void velocityCheck(tSensors sen)
{
	tHog();
	unsigned long time = nPgmTime;
	sSensor& s = gSensor[sen];
	for (ubyte i = 0; i < s.velCount; ++i)
		if (s.velData[i].timestamp >= time)
		{
			memmove(&s.velData[i], &s.velData[i + 1], s.velCount - i - 1);
			--i;
			--s.velCount;
		}
	if (!s.velCount || time - s.velData[s.velCount - 1].timestamp >= 20)
	{
		int sensor = s.value;
		if (s.velCount == SENSOR_VEL_POINT_COUNT)
		{
			s.velCount = SENSOR_VEL_POINT_COUNT - 1;
			memmove(&s.velData[0], &s.velData[1], sizeof(sSensorVelPoint) * (SENSOR_VEL_POINT_COUNT - 1));
		}
		sSensorVelPoint& data = gSensor[sen].velData[gSensor[sen].velCount++];
		data.value = sensor;
		data.timestamp = time;
		if (s.velCount > 1)
		{
			s.lstVelocity = s.velocity;
			sSensorVelPoint& old = s.velData[0];
			s.velocity = (float)(sensor - old.value) / (float)(time - old.timestamp);
			s.velGood = true;
		}
	}
	tRelease();
}

void velocityClear(tSensors sen)
{
	tHog();
	writeDebugStreamLine("%d velocityClear %d", nPgmTime, sen);
	gSensor[sen].velCount = 0;
	gSensor[sen].velocity = gSensor[sen].lstVelocity = 0;
	gSensor[sen].velGood = false;
	velocityCheck(sen);
	tRelease();
}

void startSensor(tSensors sen)
{
	if (gSensor[sen].cls == snclsInput)
		gSensor[sen].value = gSensor[sen].lstValue = SensorValue[sen];
}

void startSensors()
{
	for (ubyte i = 0; i < kNumbOfTotalSensors; ++i)
		startSensor(i);
}

void setupSensors()
{
	// Setup the pointers for the sensor values and set the base type
	for (ubyte i = 0; i < kNumbOfTotalSensors; ++i)
	{
		gSensor[i].cls = checkSenClass(i);
		gSensor[i].port = (tSensors)i;

		gSensor[i].lstVelocity = 0;
		gSensor[i].velocity = 0;
		gSensor[i].velGood = false;
		gSensor[i].velCount = 0;

#ifdef CHECK_POTI_JUMPS
		gSensor[i].filterAcc = 0;
#endif

		startSensor(i);
	}
}
