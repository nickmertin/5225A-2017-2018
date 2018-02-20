void handleLcd()
{
	tLcdButtons buttons = nLCDButtons;

	if (LCD_RISING(btnLeft))
	{
		if (gLcdScreen == kNumbLcdScreens)
			gLcdScreen = 0;
		else
			++gLcdScreen;
	}

	if (LCD_RISING(btnRight))
	{
		if (gLcdScreen)
			--gLcdScreen;
		else
			gLcdScreen = kNumbLcdScreens - 1;
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
	}

	gLastLcdButtons = buttons;
}
