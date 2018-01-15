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
