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
	case lcdBattery:
		sprintf(line, "%1.2f V  %1.2f V  ", (float)nImmediateBatteryLevel / 1000.0, getExpanderVoltage(expander, true));
		displayLCDString(0, 0, line);
		sprintf(line, "%1.2f V  Batt V. ", (float)BackupBatteryLevel / 1000.0);
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
	case lcdLightSensors:
		displayLCDCenteredString(0, "Light Sensors");
		sprintf(line, "%04d    %04d    ", SensorValue[lsBarL], SensorValue[lsBarR]);
		clearLCDLine(1);
		displayLCDString(1, 0, line);
		break;
	case lcdLiftTest:
		if (LCD_RISING(btnCenter))
			testLiftAsync();
		clearLCDLine(1);
		displayLCDCenteredString(0, "Lift Test");
		break;
	case lcdSkillsTest:
		if (LCD_RISING(btnCenter))
			testSkillsAsync();
		clearLCDLine(1);
		displayLCDCenteredString(0, "Skills Test");
		break;
	case lcdTurnAlg:
		if (LCD_RISING(btnCenter))
		{
			gTurnAlg = gTurnAlg == turnRed ? turnBlue : turnRed;
			updateTurnLookup();
		}
		displayLCDCenteredString(0, "Turn Algorithm");
		displayLCDCenteredString(1, gTurnAlg == turnRed ? "RED" : "BLUE");
		break;
	case lcdTurnCurve:
		if (LCD_RISING(btnCenter))
		{
			setDrive(0, 0);
			if (stackState != stackNotRunning)
				stackSet(stackNotRunning);

			displayLCDString(1, 0, "DEC   SAVE   INC");

			do
			{
				gLastLcdButtons = buttons;
				buttons = nLCDButtons;

				if (LCD_RISING(btnLeft) && gTurnCurveLim > 0)
					--gTurnCurveLim;
				if (LCD_RISING(btnRight) && gTurnCurveLim < 100)
					++gTurnCurveLim;

				sprintf(line, "%d", gTurnCurveLim);
				displayLCDCenteredString(0, line);

				endCycle(gMainCycle);
			} while (!LCD_RISING(btnCenter));

			updateTurnLookup();
		}
		displayLCDCenteredString(0, "Turn Curvature Limit");
		sprintf(line, "%d", gTurnCurveLim);
		displayLCDCenteredString(1, line);
		break;
	}

	gLastLcdButtons = buttons;
}

float getExpanderVoltage(tSensors sen, bool isRevA2)
{
	updateSensorInput(sen);
	return (float)gSensor[sen].value / (isRevA2 ? 280 : 182.4);
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
		sleep(10);

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
				while (gSensor[liftPoti].value > LIFT_BOTTOM)
				{
					updateSensorInput(liftPoti);
					sleep(10);
				}
				setLift(-15);
				updateMotors();
			}
			else
			{
				setLift(127);
				updateMotors();
				while (gSensor[liftPoti].value < LIFT_TOP)
				{
					updateSensorInput(liftPoti);
					sleep(10);
				}
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

void testSkills()
{
	for (tMotor mtr = port1; mtr <= port10; ++mtr)
		gMotor[mtr].power = 0;
	updateMotors();

	const string descriptions[5] =
	{
		"Start->Reset 1",
		"Reset 1->Reset 2",
		"Reset 2->Reset 3",
		"Reset 3->Reset 4",
		"Reset 4->End"
	};

	displayLCDCenteredString(0, "8R:Test 8U:Exit");

	int index = 0;

	while (true)
	{
		sleep(10);

		displayLCDCenteredString(1, descriptions[index]);

		tLcdButtons buttons = nLCDButtons;

		if (LCD_RISING(btnLeft))
		{
			if (index)
				--index;
			else
				index = 4;
		}

		if (LCD_RISING(btnRight))
		{
			if (index == 4)
				index = 0;
			else
				++index;
		}

		gLastLcdButtons = buttons;

		updateJoystick(Btn8R);
		updateJoystick(Btn8U);

		if (RISING(Btn8R))
		{
			gAutoTime = nPgmTime;
			writeDebugStreamLine("Auto start %d", gAutoTime);

			startSensors();

			stackReset();
			liftReset();
			armReset();
			mobileReset();
			autoSimpleReset();

			gKillDriveOnTimeout = true;
			gSetTimedOut = true;
			gTimedOut = false;

			autoMotorSensorUpdateTaskAsync();
			trackPositionTaskAsync();

			autoSkills(index);

			autoMotorSensorUpdateTaskKill();
			trackPositionTaskKill();

			writeDebugStreamLine("Auto: %d ms", nPgmTime - gAutoTime);

			setDrive(0, 0);
			stackReset();
			liftReset();
			armReset();
			mobileReset();
			autoSimpleReset();
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
