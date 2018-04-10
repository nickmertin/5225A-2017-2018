//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                       VEX Competition Control Include File
//
// This file provides control over a VEX Competition Match. It should be included in the user's
// program with the following line located near the start of the user's program
//        #include "VEX_Competition_Includes_Custom.h"
// The above statement will cause this program to be included in the user's program. There's no
// need to modify this program.
//
// The program displays status information on the new VEX LCD about the competition state. You don't
// need the LCD, the program will work fine whether or not the LCD is actually provisioned.
//
// The status information is still useful without the LCD. The ROBOTC IDE debugger has a "remote screen"
// that contains a copy of the status information on the LCD. You can use this to get a view of the
// status of your program. The remote screen is shown with the menu command
//   "Robot -> Debugger Windows -> VEX Remote Screen"
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(FORCE_AUTO)
#define AUTONOMOUS true
#elif defined(IGNORE_AUTO)
#define AUTONOMOUS false
#else
#define AUTONOMOUS bIfiAutonomousMode
#endif

#ifdef IGNORE_DISABLE
#define DISABLED false
#else
#define DISABLED bIfiRobotDisabled
#endif

void allMotorsOff();
void allTasksStop();
void disabled();
void startup();
void autonomous();
void usercontrol();
void testLift();
void testSkills();

MAKE_ASYNC_ONLY_MACHINE_5(competition, ;,
0, (disabled), ;,
0, (autonomous), ;,
0, (usercontrol), ;,
0, (testLift), ;,
0, (testSkills), ;
)

bool bStopTasksBetweenModes = true;

task main()
{
	// Master CPU will not let competition start until powered on for at least 2-seconds
	clearLCDLine(0);
	clearLCDLine(1);
	displayLCDString(0, 0, "Startup");
	sleep(2000);

	startup();

	while (true)
	{
		disabledAsync();
		while (DISABLED) sleep(10);

		if (AUTONOMOUS)
		{
			autonomousAsync();

			// Waiting for autonomous phase to end
			while (AUTONOMOUS && !DISABLED)
			{
				if (!bVEXNETActive)
				{
					if (nVexRCReceiveState == vrNoXmiters) // the transmitters are powered off!!
						allMotorsOff();
				}
				wait1Msec(25); // Waiting for autonomous phase to end
			}
			allMotorsOff();
			if(bStopTasksBetweenModes)
			{
				allTasksStop();
			}
		}

		else
		{
			usercontrolAsync();

			// Here we repeat loop waiting for user control to end and (optionally) start
			// of a new competition run
			while (!AUTONOMOUS && !DISABLED)
			{
				if (nVexRCReceiveState == vrNoXmiters) // the transmitters are powered off!!
					allMotorsOff();
				wait1Msec(25);
			}
			allMotorsOff();
			if(bStopTasksBetweenModes)
			{
				allTasksStop();
			}
		}
	}
}

void allMotorsOff()
{
	motor[port1] = 0;
	motor[port2] = 0;
	motor[port3] = 0;
	motor[port4] = 0;
	motor[port5] = 0;
	motor[port6] = 0;
	motor[port7] = 0;
	motor[port8] = 0;
#if defined(VEX2)
	motor[port9] = 0;
	motor[port10] = 0;
#endif
}

void allTasksStop()
{
	tStop(1);
	tStop(2);
	tStop(3);
	tStop(4);
#if defined(VEX2)
	tStop(5);
	tStop(6);
	tStop(7);
	tStop(8);
	tStop(9);
	tStop(10);
	tStop(11);
	tStop(12);
	tStop(13);
	tStop(14);
	tStop(15);
	tStop(16);
	tStop(17);
	tStop(18);
	tStop(19);
#endif
}
