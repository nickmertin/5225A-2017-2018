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

#if SKILLS_ROUTE < 0
		sprintf(line, "%c %c %c AUTO TEST ", gSensor[limArm].value ? 'A' : ' ', gSensor[limLift].value ? 'L' : ' ', gSensor[jmpSkills].value ? 'S' : ' ');
#elif SKILLS_ROUTE == 0
		if (LCD_RISING(btnCenter))
			gAutoLocked = !gAutoLocked;
		sprintf(line, "%c %c %c %s%02d  ", gSensor[limArm].value ? 'A' : ' ', gSensor[limLift].value ? 'L' : ' ', gSensor[jmpSkills].value ? 'S' : ' ', gAlliance == allianceRed ? gAutoLocked ? "LOCK R" : "Red   " : gAutoLocked ? "LOCK B" : "Blue  ", gCurAuto);
#elif SKILLS_ROUTE == 1
		sprintf(line, "%c %c %c - S1 (%d)", gSensor[limArm].value ? 'A' : ' ', gSensor[limLift].value ? 'L' : ' ', gSensor[jmpSkills].value ? 'S' : ' ', gAlliance == allianceBlue ? 110 : 108);
#else
		sprintf(line, "%c %c %c - Skills %d", gSensor[limArm].value ? 'A' : ' ', gSensor[limLift].value ? 'L' : ' ', gSensor[jmpSkills].value ? 'S' : ' ', SKILLS_ROUTE);
#endif
		clearLCDLine(1);
		displayLCDString(1, 0, line);
		break;
	case lcdAutoCones:
		if (LCD_RISING(btnCenter))
		{
			if (gAutoCones == 4)
				gAutoCones = 0;
			else
				++gAutoCones;
		}
		displayLCDCenteredString(0, "AUTO CONES");
		sprintf(line, "%d", gAutoCones);
		displayLCDCenteredString(1, line);
		break;
	case lcdAutoStart:
		if (LCD_RISING(btnCenter))
			gAutoStartSkills = !gAutoStartSkills;
		displayLCDCenteredString(0, "AUTO START");
		displayLCDCenteredString(1, gAutoStartSkills ? "SKILLS" : "GAME");
		break;
	case lcdBattery:
		sprintf(line, "Main:       %1.2f V", (float)nImmediateBatteryLevel / 1000.0);
		displayLCDString(0, 0, line);
		sprintf(line, "Backup:     %1.2f V", (float)BackupBatteryLevel / 1000.0);
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
	case lcdJumpCount:
		string armJumps;
		string liftJumps;
		clearLCDLine(0);
		clearLCDLine(1);
		sprintf(armJumps, "Arm jumps: %d", gSensor[liftPoti].jumpCount);
		sprintf(liftJumps, "Lift jumps: %d", gSensor[armPoti].jumpCount);

		displayLCDString(0, 0, armJumps);
		displayLCDString(1, 0, liftJumps);
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

				if (LCD_RISING(btnLeft) && gTurnCurvature > TURN_CURVE_MIN)
					--gTurnCurvature;
				if (LCD_RISING(btnRight) && gTurnCurvature < TURN_CURVE_MAX)
					++gTurnCurvature;

				sprintf(line, "%d", gTurnCurvature);
				displayLCDCenteredString(0, line);

				endCycle(gMainCycle);
			} while (!LCD_RISING(btnCenter));

			updateTurnLookup();
		}
		displayLCDCenteredString(0, "Turn Curve");
		sprintf(line, "%d", gTurnCurvature);
		displayLCDCenteredString(1, line);
		break;
	case lcdTurnCurveLim:
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

			gTurnIgnoreJumper = true;
			updateTurnLookup();
		}
		displayLCDCenteredString(0, "TurnCurvLimit");
		sprintf(line, "%d", gTurnCurveLim);
		displayLCDCenteredString(1, line);
		break;
	case lcdDriveAlg:
		if (LCD_RISING(btnCenter))
		{
			gDriveAlg = gDriveAlg == driveRed ? driveBlue : driveRed;
			updateDriveLookup();
		}
		displayLCDCenteredString(0, "Drive Algorithm");
		displayLCDCenteredString(1, gDriveAlg == driveRed ? "RED" : "BLUE");
		break;
	case lcdDriveCurve:
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

				if (LCD_RISING(btnLeft) && gDriveCurvature > DRIVE_CURVE_MIN)
					--gDriveCurvature;
				if (LCD_RISING(btnRight) && gDriveCurvature < DRIVE_CURVE_MAX)
					++gDriveCurvature;

				sprintf(line, "%d", gDriveCurvature);
				displayLCDCenteredString(0, line);

				endCycle(gMainCycle);
			} while (!LCD_RISING(btnCenter));

			gDriveIgnoreJumper = true;
			updateDriveLookup();
		}
		displayLCDCenteredString(0, "Drive Curve");
		sprintf(line, "%d", gDriveCurvature);
		displayLCDCenteredString(1, line);
		break;
	}

	gLastLcdButtons = buttons;
}

void testLift()
{
	for (tMotor mtr = port1; mtr <= port10; ++mtr)
		gMotor[mtr].power = 0;
	updateMotors();

	displayLCDCenteredString(1, "Test ---- Exit");

	unsigned long time = 0;

	bool up = false;

	while (true)
	{
		sleep(10);

		updateSensorInput(liftPoti);
		updateSensorInput(limLift);
		tLcdButtons buttons = nLCDButtons;

		clearLCDLine(0);
		displayLCDNumber(0, 0, gSensor[liftPoti].value);
		displayLCDNumber(0, 8, time);

		if (LCD_RISING(btnLeft))
		{
			time = nPgmTime;

			unsigned long timeout = nPgmTime + 1100;

			if (up)
			{
				setLift(-127);
				updateMotors();
				while (!gSensor[limLift].value)
				{
					if (TimedOut(timeout, TID0(testLift), false, VEL_NONE, 0, 0))
					{
						playSound(soundException);
						setLift(0);
						updateMotors();

					}

					updateSensorInput(liftPoti);
					updateSensorInput(limLift);
					if (DATALOG_TEST != -1)
					{
						tHog();
						datalogDataGroupStart();
						datalogAddValue(DATALOG_TEST, gSensor[liftPoti].rawValue);
						datalogDataGroupEnd();
						tRelease();
					}
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
					if (TimedOut(timeout, TID0(testLift), false, VEL_NONE, 0, 0))
					{
						playSound(soundException);
						setLift(0);
						updateMotors();

					}

					updateSensorInput(liftPoti);
					if (DATALOG_TEST != -1)
					{
						tHog();
						datalogDataGroupStart();
						datalogAddValue(DATALOG_TEST, gSensor[liftPoti].rawValue);
						datalogDataGroupEnd();
						tRelease();
					}
					sleep(10);
				}
				setLift(15);
				updateMotors();
			}

			time = nPgmTime - time;
			up = !up;
		}

		if (LCD_RISING(btnRight))
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

			tStart(autoMotorSensorUpdateTask);
			tStart(trackPositionTask);

			autoSkills(index);

			tStop(autoMotorSensorUpdateTask);
			tStart(trackPositionTask);

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
