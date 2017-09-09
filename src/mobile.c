void setMobile(sbyte value)
{
	gMotor[mobileL].power = gMotor[mobileR].power = value;
}

void handleMobile()
{
	switch (mobileData.state)
	{
		case mobileLowering:
			if (*gSensor[mobilePoti].value <= MOBILE_DOWN_POSITION) CHANGE_STATE(mobile, mobileDown);
			break;
		case mobileRaising:
			if (*gSensor[mobilePoti].value >= MOBILE_UP_POSITION) CHANGE_STATE(mobile, mobileUp);
			break;
		case mobileToHalfway:
			return;
	}
	HANDLE_STATE_REQUEST(mobile, , );
}

void holdMobileToTarget(int target)
{
	sPID pid;
	pidInit(pid, 0.1, 0.00002, 0, 100, 4000, -1, 127);

	sCycleData cycle;
	initCycle(cycle, 20);
	while (true)
	{
		pidCalculate(pid, (float) target, (float) *gSensor[mobilePoti].value);
		setMobile((sbyte) pid.output);

		S_LOG "PID: %.2f\t%.2f", pid.error, pid.output E_LOG_DATA

		HANDLE_STATE_REQUEST(mobile, ;, return;);

		endCycle(cycle);
	}
	setMobile(0);
}
