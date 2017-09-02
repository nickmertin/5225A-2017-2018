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
	}
	HANDLE_STATE_REQUEST(mobile, ;, ;);
}
