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

void timeoutWhileLessThanS(short *val, short threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill)
{
	while (*val < threshold && !TimedOut(timeout, routine, id, kill)) sleep(10);
	writeDebugStreamLine("Time left %d [%s - %d]", timeout - nPgmTime, routine, id);
}

void timeoutWhileGreaterThanS(tSensors sen, short threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill)
{
	velocityClear(sen);
	while (gSensor[sen].value > threshold && !TimedOut(timeout, routine, id, kill, sen)) sleep(10);
	writeDebugStreamLine("Time left %d [%s - %d]", timeout - nPgmTime, routine, id);
}

void timeoutWhileLessThanL(tSensors sen, long threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill)
{
	velocityClear(sen);
	while (gSensor[sen].value < threshold && !TimedOut(timeout, routine, id, kill, sen)) sleep(10);
	writeDebugStreamLine("Time left %d [%s - %d]", timeout - nPgmTime, routine, id);
}

void timeoutWhileGreaterThanL(tSensors sen, long threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill)
{
	velocityClear(sen);
	while (gSensor[sen].value > threshold && !TimedOut(timeout, routine, id, kill, sen)) sleep(10);
	writeDebugStreamLine("Time left %d [%s - %d]", timeout - nPgmTime, routine, id);
}

void timeoutWhileLessThanF(float *val, float threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill)
{
	velocityClear(trackL);
	while (*val < threshold && !TimedOut(timeout, routine, id, kill, trackL)) sleep(10);
	writeDebugStreamLine("Time left %d [%s - %d]", timeout - nPgmTime, routine, id);
}

void timeoutWhileGreaterThanF(float *val, float threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill)
{
	while (*val > threshold && !TimedOut(timeout, routine, id, kill)) sleep(10);
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
