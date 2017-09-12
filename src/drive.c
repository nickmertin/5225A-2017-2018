void setDrive(word left, word right)
{
	gMotor[driveL1].power = gMotor[driveL2].power = left;
	gMotor[driveR1].power = gMotor[driveR2].power = right;
}

void handleDrive()
{
	switch (driveData.state)
	{
		case driveManual:
			setDrive(gJoyThrottle + gJoyTurn, gJoyThrottle - gJoyTurn);
			break;
	}
	HANDLE_STATE_REQUEST(drive, , );
}
