void timeoutWhileEqual(void *val1, void *val2, unsigned int size, unsigned long timeout, const string description)
{
	while (!memcmp(val1, val2, size) && !TimedOut(timeout, description)) sleep(10);
}

void timeoutWhileNotEqual(void *val1, void *val2, unsigned int size, unsigned long timeout, const string description)
{
	while (memcmp(val1, val2, size) && !TimedOut(timeout, description)) sleep(10);
}

void timeoutWhileLessThanS(short *val, short threshold, unsigned long timeout, const string description)
{
	while (*val < threshold && !TimedOut(timeout, description)) sleep(10);
}

void timeoutWhileGreaterThanS(short *val, short threshold, unsigned long timeout, const string description)
{
	while (*val > threshold && !TimedOut(timeout, description)) sleep(10);
}

void timeoutWhileLessThanL(long *val, long threshold, unsigned long timeout, const string description)
{
	while (*val < threshold && !TimedOut(timeout, description)) sleep(10);
}

void timeoutWhileGreaterThanL(long *val, long threshold, unsigned long timeout, const string description)
{
	while (*val > threshold && !TimedOut(timeout, description)) sleep(10);
}

void timeoutWhileLessThanF(float *val, float threshold, unsigned long timeout, const string description)
{
	while (*val < threshold && !TimedOut(timeout, description)) sleep(10);
}

void timeoutWhileGreaterThanF(float *val, float threshold, unsigned long timeout, const string description)
{
	while (*val > threshold && !TimedOut(timeout, description)) sleep(10);
}

void timeoutWhileTrue(bool *val, unsigned long timeout, const string description)
{
	while (*val && !TimedOut(timeout, description)) sleep(10);
}

void timeoutWhileFalse(bool *val, unsigned long timeout, const string description)
{
	while (!*val && !TimedOut(timeout, description)) sleep(10);
}
