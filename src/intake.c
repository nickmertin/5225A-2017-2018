void setIntake(word power)
{
	gMotor[intake].power = power;
}

void handleIntake()
{
	HANDLE_STATE_REQUEST(intake, , )
}
