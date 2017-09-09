void setIntake(sbyte power)
{
	gMotor[intake].power = power;
}

void handleIntake()
{
	HANDLE_STATE_REQUEST(intake, , )
}
