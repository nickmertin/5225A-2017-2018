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
	tHog();
	gSensor[sen].lstValue = gSensor[sen].value;
	int raw = gSensor[sen].rawValue = SensorValue[sen];

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

	//Update buffer holding sensor values for Potis and Encoders - buffer used for vel calc
	if (SensorType[sen] == sensorPotentiometer || SensorType[sen] == sensorQuadEncoder)
	{
		sSensor& s = gSensor[sen];
		unsigned long t = nPgmTime;
		if (t > s.dataPointArr[s.arrHead].timestamp)
		{
			if (s.dataPointArr <= SENSOR_DATA_POINT_COUNT+1)
				s.dataCount++;

			s.arrHead = ( (s.arrTail+1) % SENSOR_DATA_POINT_COUNT );
			s.dataPointArr[s.arrHead].value = s.value;
			s.dataPointArr[s.arrHead].timestamp = t;

			if (s.dataCount > SENSOR_DATA_POINT_COUNT)
				s.arrTail = ( (s.arrTail+1) % SENSOR_DATA_POINT_COUNT );
		}
	}
	tRelease();

#ifdef MOTOR_SENSOR_LOGS
	if (_sensorDoDatalog)
	{
		if (gSensor[sen].rawDatalog != -1)
			datalogAddValue(gSensor[sen].rawDatalog, gSensor[sen].rawValue);
		if (gSensor[sen].valueDatalog != -1)
			datalogAddValue(gSensor[sen].valueDatalog, gSensor[sen].value);
		if (gSensor[sen].velDatalog != -1)
		{
			velocityCheck(sen);
			if (gSensor[sen].velGood)
				datalogAddValue(gSensor[sen].velDatalog, gSensor[sen].velocity * 1000);
		}
	}
#endif

#ifdef CHECK_POTI_JUMPS
	if (SensorType[sen] == sensorPotentiometer && ++gSensor[sen].filterAcc < 10 && ( (abs(gSensor[sen].value - gSensor[sen].lstValue) > 400)/* || (gSensor[sen].velGood && gSensor[sen].potiCheckVel && sgn(gSensor[sen].velocity) == -sgn(gSensor[sen].lstVelocity))*/ ) )
	{
		if (gSensor[sen].filterAcc == 1)
		{
			writeDebugStreamLine("%d port %d jumped from %d to %d", nPgmTime, sen - port1 + 1, gSensor[sen].lstValue, gSensor[sen].value);
			gSensor[sen].jumpCount ++;
		}
		gSensor[sen].value = gSensor[sen].lstValue;
	}
	else
		gSensor[sen].filterAcc = 0;
#endif
}

void updateSensorInputs()
{
#ifdef MOTOR_SENSOR_LOGS
	tHog();
	datalogDataGroupStart();
	_sensorDoDatalog = true;
#endif
	for (ubyte i = 0; i < kNumbOfTotalSensors; ++i)
	{
		if (gSensor[i].cls == snclsInput)
			updateSensorInput(i);
	}
#ifdef MOTOR_SENSOR_LOGS
	_sensorDoDatalog = false;
	datalogDataGroupEnd();
	tRelease();
#endif
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
	unsigned long t = nPgmTime;
	sSensor& s = gSensor[sen];

	if (SensorType[sen] == sensorPotentiometer || SensorType[sen] == sensorQuadEncoder)
	{
		if(s.dataCount > 1)
		{
			if(s.dataPointArr[s.arrHead].timestamp == s.dataPointArr[s.arrTail].timestamp)
			{
				s.velGood = false;
				writeDebugStreamLine("%d SENSOR %d VEL ERROR - SAME TIMESTAMP - head:%d, tail:%d", nPgmTime, sen, s.arrHead, s.arrTail);
			}
			else
			{
				s.lstVelocity = s.velocity;
				s.velocity = (float)(s.dataPointArr[s.arrHead].value - s.dataPointArr[s.arrTail].value) / (float)(s.dataPointArr[s.arrHead].timestamp - s.dataPointArr[s.arrTail].timestamp)
				s.velGood = true;
			}
		}
	}

	tRelease();
}

/*
void velocityClear(tSensors sen)
{
	tHog();
	writeDebugStreamLine("%d velocityClear %d", nPgmTime, sen);
	gSensor[sen].velocity = gSensor[sen].lstVelocity = 0;
	gSensor[sen].velGood = false;
	velocityCheck(sen);
	tRelease();
}
*/

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
		gSensor[i].mode = snmdNormal;

		gSensor[i].lstVelocity = 0;
		gSensor[i].velocity = 0;
		gSensor[i].velGood = false;

		for (ubyte j = 0; j < SENSOR_DATA_POINT_COUNT; ++j)
		{
			gSensor[i].dataPointArr[j].timestamp = 0;
			gSensor[i].dataPointArr[j].value = 0;
		}
		gSensor[i].arrHead = 0;
		gSensor[i].arrTail = 0;
		gSensor[i].dataCount = 0;

#ifdef MOTOR_SENSOR_LOGS
		gSensor[i].rawDatalog = -1;
		gSensor[i].valueDatalog = -1;
		gSensor[i].velDatalog = -1;
#endif

#ifdef CHECK_POTI_JUMPS
		gSensor[i].filterAcc = 0;
		gSensor[i].jumpCount = 0;
		gSensor[i].potiCheckVel = false;
#endif

		startSensor(i);
	}

#ifdef MOTOR_SENSOR_LOGS
	_sensorDoDatalog = false;
#endif
}
