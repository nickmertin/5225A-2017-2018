void setupJoysticks()
{
	for (TVexJoysticks i = (TVexJoysticks)0; i < kNumbOfVexRFIndices; ++i)
	{
		gJoy[i].cur = gJoy[i].lst = 0;
		gJoy[i].deadzone = -1;
		gJoy[i].enabled = false;
		gJoy[i].mirror = -1;
	}
}

void updateJoysticks()
{
	for (TVexJoysticks i = (TVexJoysticks)0; i < kNumbOfVexRFIndices; ++i)
		if (gJoy[i].enabled)
			updateJoystick(i);
}

void updateJoystick(TVexJoysticks joy)
{
	gJoy[joy].lst = gJoy[joy].cur;
	short val = vexRT[joy];
	gJoy[joy].cur = abs(val) > gJoy[joy].deadzone ? val : 0;
	if (gJoy[joy].mirror != -1)
		gJoy[gJoy[joy].mirror].cur |= gJoy[joy].cur;
}

void enableJoystick(TVexJoysticks joy)
{
	gJoy[joy].enabled = true;
}

void mirrorJoystick(TVexJoysticks joy, TVexJoysticks mirror)
{
	enableJoystick(mirror);
	gJoy[mirror].mirror = joy;
}
