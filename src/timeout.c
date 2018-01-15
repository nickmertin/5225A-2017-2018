void timeoutWhileEqual(void *val1, void *val2, unsigned int size, unsigned long timeout)
{
	string description;
	sprintf(description, "*%08x == *%08x", val1, val2);
	while (!memcmp(val1, val2, size) && !TimedOut(timeout, description)) sleep(10);
}

void timeoutWhileNotEqual(void *val1, void *val2, unsigned int size, unsigned long timeout)
{
	string description;
	sprintf(description, "*%08x != *%08x", val1, val2);
	while (memcmp(val1, val2, size) && !TimedOut(timeout, description)) sleep(10);
}

void timeoutWhileLessThanS(short *val, short threshold, unsigned long timeout)
{
	string description;
	sprintf(description, "*%08x < %d", val, threshold);
	while (*val < threshold && !TimedOut(timeout, description)) sleep(10);
}

void timeoutWhileGreaterThanS(short *val, short threshold, unsigned long timeout)
{
	string description;
	sprintf(description, "*%08x > %d", val, threshold);
	while (*val > threshold && !TimedOut(timeout, description)) sleep(10);
}

void timeoutWhileLessThanL(long *val, long threshold, unsigned long timeout)
{
	string description;
	sprintf(description, "*%08x < %d", val, threshold);
	while (*val < threshold && !TimedOut(timeout, description)) sleep(10);
}

void timeoutWhileGreaterThanL(long *val, long threshold, unsigned long timeout)
{
	string description;
	sprintf(description, "*%08x > %d", val, threshold);
	while (*val > threshold && !TimedOut(timeout, description)) sleep(10);
}

void timeoutWhileLessThanF(float *val, float threshold, unsigned long timeout)
{
	string description;
	sprintf(description, "*%08x < %f", val, threshold);
	while (*val < threshold && !TimedOut(timeout, description)) sleep(10);
}

void timeoutWhileGreaterThanF(float *val, float threshold, unsigned long timeout)
{
	string description;
	sprintf(description, "*%08x > %f", val, threshold);
	while (*val > threshold && !TimedOut(timeout, description)) sleep(10);
}

void timeoutWhileTrue(bool *val, unsigned long timeout)
{
	string description;
	sprintf(description, "*%08x", val);
	while (*val && !TimedOut(timeout, description)) sleep(10);
}

void timeoutWhileFalse(bool *val, unsigned long timeout)
{
	string description;
	sprintf(description, "!*%08x", val);
	while (!*val && !TimedOut(timeout, description)) sleep(10);
}
