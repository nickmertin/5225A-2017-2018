void setDrive(sbyte left, sbyte right)
{
	gMotor[driveL1].power = gMotor[driveL2].power = gMotor[driveL3].power = left;
	gMotor[driveR1].power = gMotor[driveR2].power = gMotor[driveR3].power = right;
}

void handleDrive()
{
	switch (driveData.state)
	{
		case driveManual:
			setDrive(gJoyThrottle + gJoyTurn, gJoyThrottle - gJoyTurn);
			break;
	}
	HANDLE_STATE_REQUEST(drive, ;, ;);
}
