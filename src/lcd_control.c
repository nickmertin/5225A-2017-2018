task runLCDTask()
{
	while (true)
	{
		runLCD();
		sleep(50);
	}
}

void runLCD()
{
	handleLCD();

	// If a new error has appeared, play a sound
	if (gError != gErrorLst && gError != "  ")
		playSound(soundException);
	gErrorLst = gError;

	// If the centre button is pressed
	if (LCD_RISING(kButtonCenter))
	{
		switch (gLCDCurPosition)
		{
			case 0:
				gError = "  ";
				break;

			case 1:
				if (joystickData.state < DRIVER_END - 1)
					CHANGE_STATE(joystick, joystickData.state + 1)
				else
					CHANGE_STATE(joystick, 0)
				break;
		}
	}

	// If one of the side buttons is pressed
	if (LCD_RISING(kButtonLeft) || LCD_RISING(kButtonRight))
	{
		switch (gLCDCurPosition)
		{
			case 0:
				clearLCD();
				break;

			case 1:
				clearLCDLine(0);
				displayLCDCenteredString(0, "Driver");
				drawDriver();
				break;
		}
	}

	switch (gLCDCurPosition)
	{
		case 0:
			string lcdOut, cAutoPoti;
			sprintf(lcdOut, "%s %s %s%s",
				"    ",
				"    ",
				"    ",
				gError);
			displayLCDString(0, 0, lcdOut);
			extendNumber(*gSensor[autoPoti].value, 4, cAutoPoti);
			sprintf(lcdOut, "%s %s %d %.1f",
				cAutoPoti,
				gAlliance == allianceBlue ? "Blue" : "Red ",
				gCurAuto,
				gAutoTime / 1000.0);
			displayLCDString(1, 0, lcdOut);
			break;

		case 1:
			drawDriver();
			break;
	}
}

// Draw the driver screen
void drawDriver()
{
	string driver = joystickData.state < DRIVER_END ? gDriverNames[joystickData.state] : "ERR BAD DRIVER";
	clearLCDLine(1);
	displayLCDCenteredString(1, driver);
}
