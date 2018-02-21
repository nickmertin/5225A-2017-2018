void handleLcd()
{
	tLcdButtons buttons = nLCDButtons;

	if (LCD_RISING(btnLeft))
	{
		if (gLcdScreen)
			--gLcdScreen;
		else
			gLcdScreen = kNumbLcdScreens - 1;
	}

	if (LCD_RISING(btnRight))
	{
		if (gLcdScreen == kNumbLcdScreens - 1)
			gLcdScreen = 0;
		else
			++gLcdScreen;
	}

	string line;

	switch (gLcdScreen)
	{
	case lcdMain:
		sprintf(line, "%4d %4d %2d", gSensor[armPoti].value, gSensor[liftPoti].value, gNumCones);
		clearLCDLine(0);
		displayLCDString(0, 0, line);

		velocityCheck(trackL);
		velocityCheck(trackR);

		sprintf(line, "%2.1f %2.1f %s%c", gSensor[trackL].velocity, gSensor[trackR].velocity, gAlliance == allianceRed ? "Red  " : "Blue ", '0' + gCurAuto);
		clearLCDLine(1);
		displayLCDString(1, 0, line);
		break;
	case lcdTracking:
		if (LCD_RISING(btnCenter))
			resetPositionFull(gPosition, 0, 0, 0);

		sprintf(line, "%3.2f %3.2f", gPosition.y, gPosition.x);
		clearLCDLine(0);
		displayLCDString(0, 0, line);

		sprintf(line, "%3.2f", radToDeg(gPosition.a));
		clearLCDLine(1);
		displayLCDString(1, 0, line);
		break;
	case lcdLiftTest:
		if (LCD_RISING(btnCenter))
			testLiftAsync();
		clearLCDLine(1);
		displayLCDCenteredString(0, "Lift Test");
		break;
	}

	gLastLcdButtons = buttons;
}

void testLift()
{
	for (tMotor mtr = port1; mtr <= port10; ++mtr)
		gMotor[mtr].power = 0;
	updateMotors();

	displayLCDCenteredString(0, "8R:Test 8U:Exit");

	unsigned long time = 0;

	bool up = false;

	while (true)
	{
		updateSensorInput(liftPoti);
		updateJoystick(Btn8R);
		updateJoystick(Btn8U);

		clearLCDLine(1);
		displayLCDNumber(1, 0, gSensor[liftPoti].value);
		displayLCDNumber(1, 8, time);

		if (RISING(Btn8R))
		{
			time = nPgmTime;

			if (up)
			{
				setLift(-127);
				updateMotors();
				while (gSensor[liftPoti].value > LIFT_BOTTOM) sleep(10);
				setLift(-15);
				updateMotors();
			}
			else
			{
				setLift(127);
				updateMotors();
				while (gSensor[liftPoti].value < LIFT_TOP) sleep(10);
				setLift(15);
				updateMotors();
			}

			time = nPgmTime - time;
			up = !up;
		}

		if (RISING(Btn8U))
		{
			if (DISABLED)
				disabledAsync();
			else
				usercontrolAsync();
			return;
		}
	}
}
