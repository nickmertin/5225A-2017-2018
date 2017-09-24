/* Functions */
void startCycle()
{
	gCycleStartTime = nPgmTime;
}

void endCycle()
{
	++gCycleCount; // Increment the cycle count

	// Make sure the cycle hasn't taken more than the max time and sleep so it takes exactly that time
	gCycleTime = nPgmTime - gCycleStartTime;
	if (gCycleTime > CYCLE_MAX_TIME)
		writeDebugStreamLine("Cycle took %dms, max should be %d", gCycleTime, CYCLE_MAX_TIME);
	else
		sleep(CYCLE_MAX_TIME - gCycleTime);
}
