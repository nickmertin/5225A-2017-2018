void setLift(word power)
{
	gMotor[liftL].power = gMotor[liftR].power = power;
}

void handleLift()
{
	if (!liftData.stateDisabled)
	{
		switch (liftData.state)
		{
			case liftLowering:
				if (*gSensor[limBottom].value) ENABLE_TO_STATE(lift, liftHolding);
				break;
			case liftRaising:
				if (*gSensor[limTop].value) ENABLE_TO_STATE(lift, liftRaising);
				break;
		}
		HANDLE_STATE_REQUEST(lift, , )
	}
}
