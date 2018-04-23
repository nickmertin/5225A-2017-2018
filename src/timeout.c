void timeoutWhileEqual(void *val1, void *val2, unsigned int size, unsigned long timeout, unsigned char *routine, unsigned short id, bool kill)
{
	while (!memcmp(val1, val2, size) && !TimedOut(timeout, routine, id, kill)) sleep(10);
	writeDebugStreamLine("Time left %d [%s - %d]", timeout - nPgmTime, routine, id);
}

void timeoutWhileNotEqual(void *val1, void *val2, unsigned int size, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill)
{
	while (memcmp(val1, val2, size) && !TimedOut(timeout, routine, id, kill)) sleep(10);
	writeDebugStreamLine("Time left %d [%s - %d]", timeout - nPgmTime, routine, id);
}

void timeoutWhileLessThanL(tTimeoutVelSourceType velSourceType, unsigned long velSourceData, float vel, long *val, long threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill, bool correctSign)
{
	int velSafetyCounter = 0;
	if (correctSign)
		vel = abs(vel);
	if (velSourceType == velSensor)
		velocityClear(velSourceData);
	unsigned long startTime = nPgmTime;
	while (*val < threshold && !TimedOut(timeout, routine, id, kill, velSourceType, velSourceData, vel, nPgmTime-startTime, &velSafetyCounter)) sleep(10);
	writeDebugStreamLine("Time left %d [%s - %d]", timeout - nPgmTime, routine, id);
}

void timeoutWhileGreaterThanL(tTimeoutVelSourceType velSourceType, unsigned long velSourceData, float vel, long *val, long threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill, bool correctSign)
{
	int velSafetyCounter = 0;
	if (correctSign)
		vel = -abs(vel);
	if (velSourceType == velSensor)
		velocityClear(velSourceData);
	unsigned long startTime = nPgmTime;
	while (*val > threshold && !TimedOut(timeout, routine, id, kill, velSourceType, velSourceData, vel, nPgmTime-startTime, &velSafetyCounter)) sleep(10);
	writeDebugStreamLine("Time left %d [%s - %d]", timeout - nPgmTime, routine, id);
}

void timeoutWhileLessThanF(tTimeoutVelSourceType velSourceType, unsigned long velSourceData, float vel, float *val, float threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill, bool correctSign)
{
	int velSafetyCounter = 0;
	if (correctSign)
		vel = abs(vel);
	if (velSourceType == velSensor)
		velocityClear(velSourceData);
	unsigned long startTime = nPgmTime;
	while (*val < threshold && !TimedOut(timeout, routine, id, kill, velSourceType, velSourceData, vel, nPgmTime-startTime, &velSafetyCounter)) sleep(10);
	writeDebugStreamLine("Time left %d [%s - %d]", timeout - nPgmTime, routine, id);
}

void timeoutWhileGreaterThanF(tTimeoutVelSourceType velSourceType, unsigned long velSourceData, float vel, float *val, float threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill, bool correctSign)
{
	int velSafetyCounter = 0;
	if (correctSign)
		vel = -abs(vel);
	if (velSourceType == velSensor)
		velocityClear(velSourceData);
	unsigned long startTime = nPgmTime;
	while (*val > threshold && !TimedOut(timeout, routine, id, kill, velSourceType, velSourceData, vel, nPgmTime-startTime, &velSafetyCounter)) sleep(10);
	writeDebugStreamLine("Time left %d [%s - %d]", timeout - nPgmTime, routine, id);
}

void timeoutWhileTrue(bool *val, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill)
{
	while (*val && !TimedOut(timeout, routine, id, kill)) sleep(10);
	writeDebugStreamLine("Time left %d [%s - %d]", timeout - nPgmTime, routine, id);
}

void timeoutWhileFalse(bool *val, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill)
{
	while (!*val && !TimedOut(timeout, routine, id, kill)) sleep(10);
	writeDebugStreamLine("Time left %d [%s - %d]", timeout - nPgmTime, routine, id);
}
