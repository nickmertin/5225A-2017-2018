/* Functions */
void setupSensors()
{
	// Clear both sensor arrays
	memset(_sensorValue, 0, sizeof(_sensorValue));
	memset(_sensorLstValue, 0, sizeof(_sensorLstValue));

	// Setup the pointers for the sensor values and set the base type
	for (ubyte i = 0; i < kNumbOfTotalSensors; ++i)
	{
		gSensor[i].cls = checkSenClass(i);
		gSensor[i].value = &_sensorValue[i];
		gSensor[i].lstValue = &_sensorLstValue[i];
		gSensor[i].port = (tSensors)i;

		gSensor[i].velocityLst = 0;
		gSensor[i].velocity = 0;
		gSensor[i].valueVelLst = 0;
		gSensor[i].timeVelCheck = 0;
	}
}

void updateSensorOutput(tSensors sen)
{
	SensorValue[sen] = *gSensor[sen].value;
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
	switch (gSensor[sen].mode)
	{
		case snmdNormal:
			*gSensor[sen].value = SensorValue[sen];
			break;
		case snmdDgtIn:
			*gSensor[sen].value = correctBtnIn(sen);
			break;
		case snmdFiltered:
			*gSensor[sen].value = round(filterSensor(sen));
			break;
	}
}

void updateSensorInputs()
{
	for (ubyte i = 0; i < kNumbOfTotalSensors; ++i)
	{
		if (gSensor[i].cls == snclsInput)
			updateSensorInput(i);
	}
}

void updateSensorLst(tSensors sen)
{
	*gSensor[sen].lstValue = *gSensor[sen].value;
}

void updateSensorsLst()
{
	memcpy(_sensorLstValue, _sensorValue, sizeof(_sensorLstValue));
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

bool correctBtnIn(tSensors sen)
{
	return SensorValue[sen] <= 150;
}

float filterSensor(tSensors sen)
{
	float values[11];
	sSensor& s = gSensor[sen];
	for (int i = 0; i < 10; ++i)
	{
		values[i] = s.rawData[i];
		if (i)
			s.rawData[i] = s.rawData[i - 1];
	}
	values[10] = (float)(s.rawData[0] = SensorValue[sen]);
	return stdDevFilteredMean(values, 11, s.filterLeniency);
}

void setupDgtIn(tSensors sen)
{
	gSensor[sen].mode = snmdDgtIn;
}

void setupFilteredSensor(tSensors sen, float leniency)
{
	gSensor[sen].mode = snmdFiltered;
	gSensor[sen].filterLeniency = leniency;

	for (int i = 0; i < 10; ++i)
		gSensor[sen].rawData[i] = 0;
}

void resetQuadratureEncoder(tSensors sen)
{
	gSensor[sen].valueVelLst -= *gSensor[sen].value;
	*gSensor[sen].value = 0;
	SensorValue[sen] = 0;
}

bool safetyCheck(tSensors sen, unsigned long failedTime, float failedVal, unsigned long safetyMovingTime)
{
	unsigned long curTime = nPgmTime;
	if (curTime - gSensor[sen].failedCheckTime == 0) return gSensor[sen].failed && nPgmTime - gSensor[sen].failedStartTime >= failedTime;
	int senVal = *gSensor[sen].value;
	float val = abs((float)(senVal - gSensor[sen].safetyVal) / (float)(curTime - gSensor[sen].failedCheckTime));
	gSensor[sen].failedCheckTime = curTime;
	if (val < failedVal && curTime - gSensor[sen].safetyStartTime > safetyMovingTime)
	{
		if (!gSensor[sen].failed)
		{
			gSensor[sen].failed = true;
			gSensor[sen].failedStartTime = curTime;
		}
	}
	else
		gSensor[sen].failed = false;
	gSensor[sen].safetyVal = senVal;
	return gSensor[sen].failed && nPgmTime - gSensor[sen].failedStartTime >= failedTime;
}

void safetyClear(tSensors sen)
{
	gSensor[sen].failed = false;
	gSensor[sen].safetyVal = *gSensor[sen].value;
	unsigned long time = nPgmTime;
	gSensor[sen].failedCheckTime = time;
	gSensor[sen].safetyStartTime = time;
}

void safetySet(tSensors sen)
{
	if (!gSensor[sen].failed)
	{
		gSensor[sen].failed = true;
		gSensor[sen].failedStartTime = nPgmTime;
	}
	gSensor[sen].failedCheckTime = nPgmTime;
}

void velocityCheck(tSensors sen)
{
	unsigned long time = nPgmTime;
	if (time - gSensor[sen].timeVelCheck >= 40)
	{
		int sensor = *gSensor[sen].value;
		gSensor[sen].velocityLst = gSensor[sen].velocity;
		gSensor[sen].velocity = (float)(sensor - gSensor[sen].valueVelLst) / (float)(time - gSensor[sen].timeVelCheck);
		gSensor[sen].valueVelLst = sensor;
		gSensor[sen].timeVelCheck = time;
	}
}

void velocityClear(tSensors sen)
{
	gSensor[sen].timeVelCheck = nPgmTime;
	gSensor[sen].valueVelLst = *gSensor[sen].value;
	gSensor[sen].velocity = gSensor[sen].velocityLst = 0;
}
