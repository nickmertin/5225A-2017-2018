void setClaw(sbyte target)
{
	gMotor[clawServo].power = target;
}

void clawToTarget()
{
	sCycleData cycle;
	initCycle(cycle, 10);
	while (true)
	{
		setClaw(gClawTarget);

		HANDLE_STATE_REQUEST(claw, ;, break;)

		endCycle(cycle);
	}
}
