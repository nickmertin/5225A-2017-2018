/* Functions */
void initCycle(sCycleData& data, unsigned long period)
{
	data.period = period;
	data.startTime = nPgmTime;
	data.count = 0;
	data.time = 0;
}

void endCycle(sCycleData& data)
{
	++data.count; // Increment the cycle count

	// Make sure the cycle hasn't taken more than the max time and sleep so it takes exactly that time
	unsigned long now = nPgmTime;
	data.time = now - data.startTime;
	if (data.time > data.period)
		writeDebugStreamLine("Cycle took %dms, max should be %d", data.time, data.period);
	else
		sleep(data.period - data.time);
	data.startTime = nPgmTime;
}
