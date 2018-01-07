#pragma config(Sensor, in1,    autoPoti,       sensorPotentiometer)
#pragma config(Sensor, in2,    mobilePoti,     sensorPotentiometer)
#pragma config(Sensor, in3,    brakesPoti,     sensorPotentiometer)
#pragma config(Sensor, in4,    liftPoti,       sensorPotentiometer)
#pragma config(Sensor, in5,    armPoti,        sensorPotentiometer)
#pragma config(Sensor, dgtl1,  trackL,         sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  trackR,         sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  trackB,         sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  jmpSkills,      sensorDigitalIn)
#pragma config(Sensor, dgtl8,  limMobile,      sensorTouch)
#pragma config(Motor,  port2,           liftL,         tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           driveL1,       tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           driveL2,       tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           arm,           tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port6,           mobile,        tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           driveR2,       tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           driveR1,       tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           liftR,         tmotorVex393HighSpeed_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//#define CHECK_POTI_JUMPS

// Necessary definitions

bool TimedOut(unsigned long timeOut, const string description);

#define TASK_POOL_SIZE 19

// Year-independent libraries

#include "notify.h"
#include "task.h"
#include "async.h"
#include "motors.h"
#include "sensors.h"
#include "joysticks.h"
#include "cycle.h"
#include "utilities.h"
#include "pid.h"
#include "state.h"

#include "notify.c"
#include "task.c"
#include "async.c"
#include "motors.c"
#include "sensors.c"
#include "joysticks.c"
#include "cycle.c"
#include "utilities.c"
#include "pid.c"
#include "state.c"

#include "Vex_Competition_Includes_Custom.c"

#include "controls.h"

#include "auto.h"
#include "auto_runs.h"

//#define DEBUG_TRACKING
#define TRACK_IN_DRIVER

unsigned long gOverAllTime = 0;
sCycleData gMainCycle;
int gNumCones = 0;
word gUserControlTaskId;

bool gDriveManual;

/* Drive */
void setDrive(word left, word right)
{
	gMotor[driveL1].power = gMotor[driveL2].power = left;
	gMotor[driveR1].power = gMotor[driveR2].power = right;
}

void handleDrive()
{
	if (gDriveManual)
	{
		//gJoy[JOY_TURN].deadzone = MAX(abs(gJoy[JOY_THROTTLE].cur) / 2, DZ_ARM);
		short y = gJoy[JOY_THROTTLE].cur;
		short a = gJoy[JOY_TURN].cur;

		if (!a && abs(gVelocity.a) > 0.2)
			a = -6 * sgn(gVelocity.a);

		setDrive(y + a, y - a);
	}
}


/* Lift */
typedef enum _tLiftStates {
	liftManaged,
	liftIdle,
	liftManual,
	liftToTarget,
	liftHold
} tLiftStates;

void setLift(word power,bool debug=false)
{
	if( debug )	writeDebugStreamLine("%06d Lift %4d", nPgmTime-gOverAllTime,power );
	gMotor[liftL].power = gMotor[liftR].power = power;
}

#define LIFT_TOP 3100
#define LIFT_BOTTOM 1050
#define LIFT_MID 1900

#define LIFT_MID_HEIGHT 22.5
#define LIFT_ARM_LEN 9

#define LIFT_HEIGHT(pos) (LIFT_MID_HEIGHT + 2 * LIFT_ARM_LEN * cos((pos - LIFT_MID) * PI / 3276))
#define LIFT_POS(height) (LIFT_MID + acos((height - LIFT_MID_HEIGHT) / (2 * LIFT_ARM_LEN)) * 3276 / PI)

MAKE_MACHINE(lift, tLiftStates, liftIdle,
{
case liftIdle:
	setLift(0);
	break;
case liftToTarget:
	if (arg != -1)
	{
		const float kP_vel = 1.0;
		const float kP_pwr = 1.0;
		int err;
		velocityClear(liftPoti);
		sCycleData cycle;
		initCycle(cycle, 10, "liftToTarget");
		do
		{
			err = (int)(arg - gSensor[liftPoti].value);
			velocityCheck(liftPoti);
			if (gSensor[liftPoti].velGood)
				setLift((word)(kP_pwr * (kP_vel * err - gSensor[liftPoti].velocity)));
			endCycle(cycle);
		} while (abs(err) > 75);
	}
	NEXT_STATE(liftHold, arg);
case liftHold:
	if (arg == -1) arg = gSensor[liftPoti].value;
	{
		sCycleData cycle;
		initCycle(cycle, 10, "liftHold");
		while (true)
		{
			setLift(8 + (word)(5 * cos((gSensor[liftPoti].value - LIFT_MID) * PI / 3276)));
			endCycle(cycle);
		}
	}
})

void handleLift()
{
	if (liftState == liftManaged ) return;

	if (RISING(JOY_LIFT))
	{
		liftSet(liftManual);
	}
	if (FALLING(JOY_LIFT))
	{
		liftSet(liftHold);
	}

	if (liftState == liftManual)
	{
		word value = gJoy[JOY_LIFT].cur * 2 - 128 * sgn(gJoy[JOY_LIFT].cur);
		if (gSensor[liftPoti].value <= LIFT_BOTTOM && value < -10) value = -10;
		if (gSensor[liftPoti].value >= LIFT_TOP && value > 10) value = 10;
		setLift(value);
	}
}


/* Arm */
typedef enum _tArmStates { // it used to be sArmStates. Isn't that only use for structs???
	armManaged,
	armIdle,
	armManual,
	armPlainPID,
	armRaise,
	armLower,
	armHold,
	armHorizontal
} tArmStates;

#define ARM_UP_KP 0.25
#define ARM_DOWN_KP 0.25
#define ARM_POSITIONS (ARR_LEN(gArmPositions) - 1)

#define ARM_TOP 1900
#define ARM_BOTTOM  115

short gArmPositions[] = { 130, 750, 1950 };
word gArmHoldPower[] = { -12, 0, 10 };
short gArmPosition = 2;
short gArmTarget;
//sArmStates gArmState = armIdle;
unsigned long gArmStart;
sPID gArmPID;
sPID* gArmPIDInUse = &gArmPID;

void setArm(word power, bool debug = false)
{
	if( debug ) writeDebugStreamLine("%06d Arm  %4d", nPgmTime-gOverAllTime, power);
	gMotor[arm].power = power;
	//	motor[arm]=power;
}

MAKE_MACHINE(arm, tArmStates, armIdle,
{
case armIdle:
	setArm (0);
	break;
case armManual:
	{
		word value = gJoy[JOY_ARM].cur * 2 - 128 * sgn(gJoy[JOY_ARM].cur);
		if (gSensor[armPoti].value >= ARM_TOP && value > 10) value = 10;
		if (gSensor[armPoti].value <= ARM_BOTTOM && value < -10) value = -10;
		setArm(value);
		break;
	}
case armPlainPID:
	{
		int value = gSensor[armPoti].value;
		if (gArmTarget >= ARM_TOP && value >= ARM_TOP)
		{
			writeDebugStreamLine("Arm raise: %d", nPgmTime - gArmStart);
			NEXT_STATE (armHold, -1);
		}
		else if (gArmTarget <= ARM_BOTTOM && value <= ARM_BOTTOM)
		{
			writeDebugStreamLine("Arm lower: %d", nPgmTime - gArmStart);
			NEXT_STATE (armHold, -1);
		}
		else
		{
			pidCalculate(gArmPID, (float)gArmTarget, (float)value);
			setArm((word)gArmPID.output);
		}
		break;
	}
case armRaise:
	{
		if (gSensor[armPoti].value >= gArmTarget)
		{
			writeDebugStreamLine("Arm raise: %d", nPgmTime - gArmStart);
			NEXT_STATE (armHold, -1);
		}
		else setArm(127);
		break;
	}
case armLower:
	{
		if (gSensor[armPoti].value <= gArmTarget)
		{
			writeDebugStreamLine("Arm lower: %d", nPgmTime - gArmStart);
			NEXT_STATE (armHold, -1);
		}
		else setArm(-127);
		break;
	}
case armHold:
	{
		if( gArmPosition == 1 )
		{
			if( gSensor[armPoti].value < 1700 )
				setArm(15);
			else
				setArm(9);
		}
		else
		{
			setArm(gArmHoldPower[gArmPosition]);
		}

		//setArm(gArmPosition == 1 ? gSensor[armPoti].value < 1700 ? 15 : 9 : gArmHoldPower[gArmPosition]);
		break;
	}
case armHorizontal:
	{
		int value = gSensor[armPoti].value;
		if (abs(gArmTarget - value) < 400)
		{
			velocityCheck(armPoti);
			if (gSensor[armPoti].velGood)
			{
				int power = 10 - gSensor[armPoti].velocity / 2;
				setArm(LIM_TO_VAL(power, 15));
			}
		}
		else if (value > gArmTarget) setArm(-127);
		else setArm(127);
		break;
	}
})

void handleArm()
{
	if (RISING(JOY_ARM))
	{
		armSet (armManual);
	}
	if (FALLING(JOY_ARM) && gArmState == armManual)
	{
		if (gSensor[armPoti].value <= ARM_BOTTOM) gArmPosition = 0;
		else if (gSensor[armPoti].value >= ARM_TOP) gArmPosition = 2;
		else gArmPosition = 1;
		armSet (armHold);
	}
	if (RISING(BTN_ARM_DOWN))
	{
		armSet (armLower);

		gArmPosition = 0;

		gArmTarget = gArmPositions[gArmPosition];
		gArmStart = nPgmTime;
	}
}


//void handleArm()
//{
//	if (RISING(JOY_ARM))
//	{
//		gArmState = armManual;
//	}
//	if (FALLING(JOY_ARM) && gArmState == armManual)
//	{
//		if (gSensor[armPoti].value <= ARM_BOTTOM) gArmPosition = 0;
//		else if (gSensor[armPoti].value >= ARM_TOP) gArmPosition = 2;
//		else gArmPosition = 1;
//		gArmState = armHold;
//	}
//	if (RISING(BTN_ARM_DOWN))
//	{
//		gArmState = armLower;

//		gArmPosition = 0;

//		gArmTarget = gArmPositions[gArmPosition];
//		gArmStart = nPgmTime;
//	}

//	if( gArmState==armManaged ) return;

//	switch (gArmState)
//	{
//	case armManual:
//		{
//			word value = gJoy[JOY_ARM].cur * 2 - 128 * sgn(gJoy[JOY_ARM].cur);
//			if (gSensor[armPoti].value >= ARM_TOP && value > 10) value = 10;
//			if (gSensor[armPoti].value <= ARM_BOTTOM && value < -10) value = -10;
//			setArm(value);
//			break;
//		}
//	case armPlainPID:
//		{
//			int value = gSensor[armPoti].value;
//			if (gArmTarget >= ARM_TOP && value >= ARM_TOP)
//			{
//				writeDebugStreamLine("Arm raise: %d", nPgmTime - gArmStart);
//				gArmState = armHold;
//			}
//			else if (gArmTarget <= ARM_BOTTOM && value <= ARM_BOTTOM)
//			{
//				writeDebugStreamLine("Arm lower: %d", nPgmTime - gArmStart);
//				gArmState = armHold;
//			}
//			else
//			{
//				pidCalculate(gArmPID, (float)gArmTarget, (float)value);
//				setArm((word)gArmPID.output);
//			}
//			break;
//		}
//	case armRaise:
//		{
//			if (gSensor[armPoti].value >= gArmTarget)
//			{
//				writeDebugStreamLine("Arm raise: %d", nPgmTime - gArmStart);
//				gArmState = armHold;
//			}
//			else setArm(127);
//			break;
//		}
//	case armLower:
//		{
//			if (gSensor[armPoti].value <= gArmTarget)
//			{
//				writeDebugStreamLine("Arm lower: %d", nPgmTime - gArmStart);
//				gArmState = armHold;
//			}
//			else setArm(-127);
//			break;
//		}
//	case armHold:
//		{
//			if( gArmPosition == 1 )
//			{
//				if( gSensor[armPoti].value < 1700 )
//					setArm(15);
//				else
//					setArm(9);
//			}
//			else
//			{
//				setArm(gArmHoldPower[gArmPosition]);
//			}


//			//setArm(gArmPosition == 1 ? gSensor[armPoti].value < 1700 ? 15 : 9 : gArmHoldPower[gArmPosition]);
//			break;
//		}
//	case armHorizontal:
//		{
//			int value = gSensor[armPoti].value;
//			if (abs(gArmTarget - value) < 400)
//			{
//				velocityCheck(armPoti);
//				if (gSensor[armPoti].velGood)
//				{
//					int power = 10 - gSensor[armPoti].velocity / 2;
//					setArm(LIM_TO_VAL(power, 15));
//				}
//			}
//			else if (value > gArmTarget) setArm(-127);
//			else setArm(127);
//			break;
//		}
//	case armIdle:
//		{
//			setArm(0);
//			break;
//		}
//	}
//}


/* Mobile */

typedef enum _tMobileStates {
	mobileManaged,
	mobileIdle,
	mobileTop,
	mobileBottom,
	mobileUpToMiddle,
	mobileDownToMiddle,
	mobileMiddle,
	mobileBrakes
} tMobileStates;

#define MOBILE_TOP 2250
#define MOBILE_BOTTOM 350
#define MOBILE_MIDDLE_UP 600
#define MOBILE_MIDDLE_DOWN 1200
#define MOBILE_MIDDLE_THRESHOLD 1900
#define MOBILE_HALFWAY 1200

#define MOBILE_UP_POWER 127
#define MOBILE_DOWN_POWER -127
#define MOBILE_UP_HOLD_POWER 10
#define MOBILE_DOWN_HOLD_POWER -10

void setMobile(word power)
{
	gMotor[mobile].power = power;
}

MAKE_MACHINE(mobile, tMobileStates, mobileIdle,
{
case mobileIdle:
	setMobile(0);
	break;
case mobileTop:
	{
		setMobile(MOBILE_UP_POWER);
		unsigned long timeout = nPgmTime + 2000;
		while (gSensor[mobilePoti].value < MOBILE_TOP && !TimedOut(timeout, "mobileTop")) sleep(10);
		setMobile(MOBILE_UP_HOLD_POWER);
		break;
	}
case mobileBottom:
	{
		setMobile(MOBILE_DOWN_POWER);
		unsigned long timeout = nPgmTime + 2000;
		while (gSensor[mobilePoti].value > MOBILE_BOTTOM && !TimedOut(timeout, "mobileBottom")) sleep(10);
		setMobile(MOBILE_DOWN_HOLD_POWER);
		break;
	}
case mobileUpToMiddle:
	{
		setMobile(MOBILE_UP_POWER);
		unsigned long timeout = nPgmTime + 1000;
		while (gSensor[mobilePoti].value < MOBILE_MIDDLE_UP && !TimedOut(timeout, "mobileUpToMiddle")) sleep(10);
		setMobile(15);
		NEXT_STATE(mobileMiddle, -1)
	}
case mobileDownToMiddle:
	{
		setMobile(MOBILE_DOWN_POWER);
		unsigned long timeout = nPgmTime + 1000;
		while (gSensor[mobilePoti].value > MOBILE_MIDDLE_DOWN && !TimedOut(timeout, "mobileUpToMiddle")) sleep(10);
		setMobile(15);
		NEXT_STATE(mobileMiddle, -1)
	}
case mobileMiddle:
	while (gSensor[mobilePoti].value < MOBILE_MIDDLE_THRESHOLD) sleep(10);
	NEXT_STATE(mobileTop, -1)
})

void handleMobile()
{
	if (mobileState == mobileUpToMiddle || mobileState == mobileDownToMiddle || mobileState == mobileMiddle)
	{
		if (RISING(BTN_MOBILE_TOGGLE))
			mobileSet(mobileTop);
		if (RISING(BTN_MOBILE_MIDDLE))
			mobileSet(mobileBottom);
	}
	else
	{
		if (RISING(BTN_MOBILE_TOGGLE))
			mobileSet(gSensor[mobilePoti].value > MOBILE_HALFWAY ? mobileBottom : mobileTop);
		if (RISING(BTN_MOBILE_MIDDLE))
			mobileSet(gSensor[mobilePoti].value > MOBILE_HALFWAY ? mobileDownToMiddle : mobileUpToMiddle);
	}
}


/* Macros + Autonomous */

bool gLiftAsyncDone;
bool gContinueLoader = false;
bool gLiftTargetReached;



byte stackAsync(bool arg0);
byte dropArmAsync();

bool gKillDriveOnTimeout = false;

bool TimedOut(unsigned long timeOut, const string description)
{
	if (nPgmTime > timeOut)
	{
		hogCPU();
		setLift(0);
		setArm(0);
		setMobile(0);
		if (gKillDriveOnTimeout) setDrive(0, 0);
		updateMotors();
		writeDebugStreamLine("%06d EXCEEDED TIME %d - %s", nPgmTime - gOverAllTime, timeOut - gOverAllTime, description);
		gArmState= armHold;
		liftReset();
		gDriveManual = true;
		int current = nCurrentTask;
		while (true)
		{
			int next = tEls[current].parent;
			if (next == -1 || next == gUserControlTaskId || next == main) break;
			current = next;
		}
		tStopAll(current);
		return true;
	}
	else
		return false;
}

bool gArmDown;
void dropArm()
{
	unsigned long armTimeOut;
	gArmDown = false;
	setArm(-127,true);
	armTimeOut = nPgmTime + 700;
	while (gSensor[armPoti].value > ARM_BOTTOM  + 700 && !TimedOut(armTimeOut, "dropArm")) sleep(10);
	setArm(30,true);
	sleep(60);
	setArm(-90,true);
	sleep(100);
	setArm(-15,true);
	sleep(120);
	gArmDown = true;
	gArmState = armHold;
	gArmPosition = 0;
}

NEW_ASYNC_VOID_0(dropArm);

//int gliftTargetA[11] = { 0, 0, 70, 190, 450, 975, 1500, 1900, 2600, 3250, 4095-LIFT_BOTTOM };
float gliftTargetA[11] =   { 0, 0, 40, 190, 280, 600,  930, 1250, 1700, 2100, LIFT_TOP-LIFT_BOTTOM };
//////      stacking ON    0  1   2   3    4    5     6     7     8     9     10

void clearArm()
{
	liftSet(liftToTarget, LIFT_POS(gliftTargetA[gNumCones]));
	unsigned long timeout = nPgmTime + 1500;
	while (liftState == liftToTarget && !TimedOut(timeout, "clear 1")) sleep(10);

	gArmState = armManaged;
	setArm(127);
	timeout = nPgmTime + 1500;
	while (gSensor[armPoti].value < gArmPositions[2] && !TimedOut(timeout, "clear 2")) sleep(10);
	gArmPosition = 2;
	gArmState = armHold;
}

NEW_ASYNC_VOID_0(clearArm);

void stack(bool downAfter)
{
	//gArmState = armManaged;
	//gClawState = clawManaged;
	//gLiftState = liftManaged;
	//unsigned long armTimeOut;
	//unsigned long clawTimeOut;
	//unsigned long liftTimeOut;

	//gLiftTarget =LIFT_BOTTOM + gliftTargetA[gNumCones];

	//if( gNumCones< 2 )
	//{
	//	gOverAllTime = nPgmTime;
	//	writeDebugStreamLine(" STACKING on %d", gNumCones );

	//	clawTimeOut = nPgmTime + 800;
	//	setClaw(128, true);
	//	while (SensorValue[clawPoti] > CLAW_CLOSE + 200 && !TimedOut(clawTimeOut, "stack 1")) sleep(10);
	//	setArm(128, true);
	//	clawTimeOut = nPgmTime + 500;
	//	armTimeOut = nPgmTime +600;
	//	while (SensorValue[clawPoti] > CLAW_CLOSE && !TimedOut(clawTimeOut, "stack 2")) sleep(10);

	//	//claw holding power while lifting
	//	setClaw (20, true) ;

	//	while (SensorValue[armPoti] < ARM_TOP - 600 && !TimedOut(armTimeOut, "stack 3")) sleep(10);
	//	//squeeze claw as it approaches the top of the arm swing
	//	setClaw ( 40, true );
	//	armTimeOut = nPgmTime +350;
	//	while (SensorValue[armPoti] < ARM_TOP && !TimedOut(armTimeOut, "stack 4")) sleep(10);
	//	setArm(15, true);
	//	setClaw(25, true);
	//	sleep(450);
	//	if (!AUTONOMOUS && (gMobileState != mobileHold || gMobileTarget != MOBILE_TOP))
	//	{
	//		gLiftState = liftIdle;
	//		gArmState = armIdle;
	//		gClawState = clawClosed;
	//		return;
	//	}
	//	setClaw(-128, true);
	//	while( SensorValue[clawPoti] < CLAW_OPEN-500 )  sleep(10);
	//	gArmDown= false;
	//	if (downAfter)
	//	{
	//		writeDebugStreamLine("%06d starting Task dropArm", nPgmTime-gOverAllTime);
	//		dropArmAsync();
	//	}

	//	while( SensorValue[clawPoti] < CLAW_OPEN )  sleep(10);
	//	setClaw(-10);
	//	gNumCones++;
	//	if (downAfter)
	//	{
	//		while( !gArmDown) sleep(10);
	//	}
	//	setArm(-15,true);
	//	writeDebugStreamLine("stack time %dms",nPgmTime- gOverAllTime );

	//}
	//else if (gNumCones == 2)
	//{

	//	gOverAllTime = nPgmTime;
	//	writeDebugStreamLine(" STACKING on %d", gNumCones );
	//	clawTimeOut = nPgmTime + 800;
	//	setClaw(128,true);
	//	while (SensorValue[clawPoti] > CLAW_CLOSE + 200 && !TimedOut(clawTimeOut, "stack 5")) sleep(10);

	//	setArm(128,true);
	//	armTimeOut = nPgmTime + 700;
	//	while (SensorValue[clawPoti] > CLAW_CLOSE && !TimedOut(clawTimeOut, "stack 6")) sleep(10);

	//	setClaw (20,true) ;
	//	gLiftTarget = LIFT_BOTTOM +  gliftTargetA[gNumCones];
	//	liftUpAsync();

	//	while (SensorValue[armPoti] < ARM_TOP - 600 && !TimedOut(armTimeOut, "stack 7")) sleep(10);

	//	setClaw (40);
	//	while (SensorValue[armPoti] < ARM_TOP && !TimedOut(armTimeOut, "stack 8")) sleep(10);

	//	setArm(20);
	//	setClaw(15);
	//	sleep(125);
	//	setArm(15);

	//	setClaw(-128);
	//	while( SensorValue[clawPoti] < CLAW_OPEN)  sleep(10);
	//	gNumCones++;
	//	setLift(-30);
	//	sleep(50);
	//	gArmDown= false;
	//	if (downAfter)
	//	{
	//		dropArmAsync();
	//	}
	//	setClaw(-10);
	//	setLift(-12);
	//	if (downAfter)
	//	{
	//		while( !gArmDown) sleep(10);
	//	}
	//	writeDebugStreamLine( " STACKTIME = %d ms", nPgmTime- gOverAllTime);
	//}
	//else if (gNumCones == 3)
	//{
	//	gOverAllTime = nPgmTime;
	//	writeDebugStreamLine(" -------------STACKING on %d", gNumCones );
	//	clawTimeOut = nPgmTime + 800;
	//	setClaw(128,true);
	//	while (SensorValue[clawPoti] > CLAW_CLOSE+200 && !TimedOut(clawTimeOut, "stack 9")) sleep(10);

	//	setArm(128,true);
	//	armTimeOut = nPgmTime + 600;
	//	while (SensorValue[clawPoti] > CLAW_CLOSE && !TimedOut(clawTimeOut, "stack 10")) sleep(10);

	//	setClaw (20,true) ;
	//	gLiftTarget = LIFT_BOTTOM +  gliftTargetA[gNumCones];
	//	liftUpAsync();
	//	while (SensorValue[armPoti] < ARM_TOP - 600 && !TimedOut(armTimeOut, "stack 11")) sleep(10);

	//	setClaw (40,true);
	//	while (SensorValue[armPoti] < ARM_TOP && !TimedOut(armTimeOut, "stack 12")) sleep(10);

	//	setArm (15,true);
	//	setClaw (15,true);
	//	setLift(-30,true);
	//	sleep(150);
	//	setClaw(-128,true);

	//	while ( SensorValue[liftPoti] > gLiftTarget-160  && !SensorValue[limBottom] ) sleep(10); //200
	//	setLift(15,true);

	//	writeDebugStreamLine("%d sleep(100)", nPgmTime-gOverAllTime);
	//	sleep(100);

	//	gLiftTarget = LIFT_BOTTOM + gliftTargetA[3] ;

	//	liftUpAsync();
	//	while( SensorValue[clawPoti] < CLAW_OPEN )  sleep(10);
	//	gNumCones++;
	//	setClaw(-10,true);
	//	while( !gLiftTargetReached) sleep(10);

	//	gArmDown= false;
	//	if (downAfter)
	//	{
	//		armTimeOut = nPgmTime + 600;
	//		dropArmAsync();
	//		while (SensorValue[armPoti] > ARM_TOP-600  && !TimedOut(armTimeOut, "stack 13")) sleep(10);
	//		setLift(-30,true);
	//		while ( !SensorValue[limBottom] ) sleep(10);
	//		setLift(-10,true);
	//		while( !gArmDown ) sleep(10);
	//	}

	//	writeDebugStreamLine( " STACKTIME = %d ms", nPgmTime- gOverAllTime);

	//}
	//else
	//{


	//	writeDebugStreamLine(" -------------STACKING on %d", gNumCones );

	//	// if the claw is open then make sure the arm is down close the claw
	//	/*if( gSensor[clawPoti].value > CLAW_CLOSE+100 )
	//	{
	//		setArm(-127,true);
	//		armTimeOut = nPgmTime + 500;
	//		while (gSensor[armPoti].value > ARM_BOTTOM && !TimedOut(armTimeOut, "stack 14")) sleep(10);
	//	}*/

	//	gOverAllTime = nPgmTime;
	//	setArm(-15,true);
	//	setClaw(CLAW_CLOSE_POWER,true);
	//	clawTimeOut = nPgmTime + 800;
	//	while (gSensor[clawPoti].value > CLAW_CLOSE+300 && !TimedOut(clawTimeOut, "stack 15")) sleep(10);

	//	// raise the arm while closing the claw along the way.
	//	if (gSensor[armPoti].value < ARM_BOTTOM + 100)
	//	{
	//		setArm(127,true);
	//		armTimeOut = nPgmTime + 500;
	//		clawTimeOut = nPgmTime + 300;
	//		while (gSensor[clawPoti].value > CLAW_CLOSE && !TimedOut(clawTimeOut, "stack 16")) sleep(10);
	//		setClaw(CLAW_CLOSE_HOLD_POWER,true);

	//		//wait for the arm to pull the cone away from the base before raising the lift
	//		while (gSensor[armPoti].value < ARM_BOTTOM+100 && !TimedOut(armTimeOut, "stack 17")) sleep(10);//200
	//	}

	//	setArm(25,true);
	//	liftUpAsync();

	//	//slow the arm down otherwise it will hit the stack on the way up
	//	while ( gSensor[liftPoti].value < gLiftTarget - 400 ) sleep(10);
	//	armTimeOut = nPgmTime + 650;
	//	setArm(127,true);

	//	// tighten the claw just before hitting the top
	//	while (gSensor[armPoti].value < ARM_TOP-600 && !TimedOut(armTimeOut, "stack 18")) sleep(10);
	//	setClaw(40,true);

	//	//arm reached the top
	//	while (gSensor[armPoti].value < ARM_TOP && !TimedOut(armTimeOut, "stack 19")) sleep(10);
	//	setArm(25,true);
	//	setClaw(CLAW_CLOSE_HOLD_POWER,true);


	//if( gNumCones < 10 )
	//{
	//	setLift(-80,true);
	//	liftTimeOut = nPgmTime+400;
	//	while (gSensor[liftPoti].value > gLiftTarget-60 && !gSensor[limBottom].value && !TimedOut(liftTimeOut, "stack 20")) sleep(10);
	//	setLift(15,true);

	//	setClaw(CLAW_OPEN_POWER,true);
	//	sleep(150);
	//	// dont lift as high as the original stack target
	//	gLiftTarget = gLiftTarget-30;//60
	//	liftUpAsync();
	//	while( gSensor[clawPoti].value< CLAW_OPEN ) sleep(10);
	//	setClaw(CLAW_OPEN_HOLD_POWER,true);
	//	gNumCones++;
	//	while( !gLiftTargetReached ) sleep(10);

	//	gArmDown = false;
	//	if (downAfter)
	//	{
	//		dropArmAsync();
	//		while( gSensor[armPoti].value > ARM_TOP-400 )  sleep(10);
	//		setLift(-127);
	//		while ( !gSensor[limBottom].value ) sleep(10);
	//		setLift(-10);
	//		while( !gArmDown) sleep(10);
	//	}
	//}
	//else
	//{
	//	setArm(15,true);
	//	setLift(15,true);
	//	sleep(200);
	//	setLift(-50,true);
	//	liftTimeOut = nPgmTime + 700;
	//	gLiftTarget = 2900; //JOHN
	//	while (gSensor[liftPoti].value > gLiftTarget && !gSensor[limBottom].value && !TimedOut(liftTimeOut, "stack 21")) sleep(10);
	//	setLift(15, true);
	//	setClaw(10,true);
	//}

	//	writeDebugStreamLine( "------ STACK %d cones in %d ms -----", gNumCones, nPgmTime-gOverAllTime);
	//	gLiftState = liftIdle;
	//	gArmState = downAfter && gNumCones < 11 ? armHold : armIdle;
	//	gClawState = clawIdle;
	//}
}

NEW_ASYNC_VOID_1(stack, bool);

float gLoaderOffset[12] = { 5.5, 4.5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };
sNotifier gStackFromLoaderNotifier;

void stackFromLoader(int max, bool wait, bool onMobile)
{
	//if (max > 7) max = 7;
	//gLiftState = liftManaged;
	//gArmState = armManaged;
	//gClawStart = clawManaged;
	//gDriveManual = false;
	//EndTimeSlice();
	//float sy = onMobile ? gLoaderOffset[gNumCones] : 2;
	//resetPositionFull(gPosition, sy, 0, 0);
	//trackPositionTaskAsync();
	//byte async = moveToTargetAsync(0.5, 0, sy, 0, -25, 3, 0.5, 0.5, true, false);
	//unsigned long driveTimeout = nPgmTime + 2000;
	////setClaw(CLAW_OPEN_POWER, true);
	//unsigned long coneTimeout = nPgmTime + 1000;
	////while (gSensor[clawPoti].value < CLAW_OPEN && !TimedOut(coneTimeout, "loader 1")) sleep(10);
	//setClaw(CLAW_OPEN_HOLD_POWER);
	//await(async, driveTimeout, "loader 1");
	//trackPositionTaskKill();
	//gDriveManual = true;
	//if (wait && waitOn(gStackFromLoaderNotifier, nPgmTime + 60_000, "loader 2") == -1) goto end;
	//while (gNumCones < max)
	//{
	//	gLiftState = liftManaged;
	//	gArmState = armManaged;
	//	gClawStart = clawManaged;
	//	if (gSensor[armPoti].value < 1400)
	//	{
	//		setArm(60);
	//		while (gSensor[armPoti].value < 1400) sleep(10);
	//		setArm(-7);
	//	}
	//	else if (gSensor[armPoti].value > 1500)
	//	{
	//		if (gSensor[armPoti].value > 1700)
	//		{
	//			setArm(-80);
	//			while (gSensor[armPoti].value > 1700) sleep(10);
	//			setArm(-50);
	//		}
	//		while (gSensor[armPoti].value > 1500) sleep(10);
	//		setArm(5);
	//	}
	//	else setArm(-10);
	//	setLift(-80);
	//	coneTimeout = nPgmTime + 1000;
	//	while (gSensor[liftPoti].value > LIFT_BOTTOM + 500 && !TimedOut(coneTimeout, "loader 2")) sleep(10);
	//	setLift(-60);
	//	while (!gSensor[limBottom].value && !TimedOut(coneTimeout, "loader 3")) sleep(10);
	//	setLift(-10);
	//	sleep(200);
	//	setArm(-60);
	//	coneTimeout = nPgmTime + 500;
	//	while (gSensor[armPoti].value > 1500 && !TimedOut(coneTimeout, "loader 4")) sleep(10);
	//	setArm(-10);
	//	sleep(300);
	//	stack(false);
	//}
	//end:
	//gLiftState = liftIdle;
	//gArmState = armIdle;
	//gClawState = clawIdle;
	//writeDebugStreamLine("Done stacking from loader");
}

NEW_ASYNC_VOID_3(stackFromLoader, int, bool, bool);

void stackExternal()
{
	//gArmState = armManaged;
	//gClawState = clawManaged;
	//gDriveManual = false;
	//resetPositionFull(gPosition, 0, 0, 0);
	//EndTimeSlice();
	//writeDebugStreamLine("%f %f %f", gPosition.y, gPosition.x, gPosition.a);
	//byte async = trackPositionTaskAsync();
	//EndTimeSlice();
	//writeDebugStreamLine("%f %f %f", gPosition.y, gPosition.x, gPosition.a);
	//moveToTargetAsync(-0.8, 0, 0, 0, -50, 4, 0.5, 0.5, true, true);
	//unsigned long driveTimeout = nPgmTime + 1000;
	//sleep(200);
	//setArm(-90);
	//unsigned long coneTimeout = nPgmTime + 1000;
	//while (gSensor[armPoti].value > 750 && !TimedOut(coneTimeout, "extern/driver 1")) sleep(10);
	//setArm(-10);
	//await(async, driveTimeout, "extern/driver 2");
	//trackPositionTaskKill();
	//setClaw(CLAW_OPEN_POWER);
	//coneTimeout = nPgmTime + 800;
	//while (gSensor[clawPoti].value < CLAW_OPEN && !TimedOut(coneTimeout, "extern/driver 3")) sleep(10);
	//setClaw(CLAW_OPEN_HOLD_POWER);
	//gClawState = clawOpened;
	//setArm(127);
	//coneTimeout = nPgmTime + 100;
	//while (gSensor[armPoti].value < gArmPositions[2]) sleep(10);
	//setArm(10);
	//gArmPosition = 2;
	//gArmState = armHold;
	//setDrive(80, 80);
	//sleep(400);
	//setDrive(0, 0);
	//gMobileTarget = MOBILE_TOP;
	//gMobileState = mobileRaise;
	//setMobile(MOBILE_UP_POWER);
	//gMobileHoldPower = MOBILE_UP_HOLD_POWER;
	//gNumCones = 1;
	//gDriveManual = true;
}

NEW_ASYNC_VOID_0(stackExternal);

bool cancel()
{
	if (stackKill() || stackFromLoaderKill() || stackExternalKill())
		return false;
	liftSet(liftIdle);
	gArmState = armIdle;
	gDriveManual = true;
	writeDebugStreamLine("Stack cancelled");
	return true;
}

void handleMacros()
{

	if (RISING(BTN_MACRO_CLEAR))
	{
		if (!cancel())
		{
			writeDebugStreamLine("Clearing lift and arm");
			clearArmAsync();
		}
	}

	if (RISING(BTN_MACRO_STACK) && gNumCones < 11 )
	{
		if (!cancel())
		{
			writeDebugStreamLine("Stacking");
			stackAsync(true);
			playSound(soundUpwardTones);
		}
	}

	//if (RISING(BTN_MACRO_LOADER))
	//{
	//	if (!cancel())
	//	{
	//		writeDebugStreamLine("Stacking from loader");
	//		stackFromLoaderAsync(11, true, gMobileState == mobileHold && gMobileTarget == MOBILE_TOP);
	//		playSound(soundUpwardTones);
	//	}
	//}

	//if (FALLING(BTN_MACRO_LOADER)) notify(gStackFromLoaderNotifier);

	if (RISING(BTN_MACRO_EXTERNAL))
	{
		if (!cancel())
		{
			writeDebugStreamLine("Stacking on external mobile");
			stackExternalAsync();
			playSound(soundUpwardTones);
		}
	}

	if (RISING(BTN_MACRO_CANCEL)) cancel();

	if (FALLING(BTN_MACRO_INC))
		writeDebugStreamLine("%06d MAcro_INC Released",nPgmTime,gNumCones);
	if (RISING(BTN_MACRO_INC) && gNumCones < 11) {
		++gNumCones;
		writeDebugStreamLine("%06d gNumCones= %d",nPgmTime,gNumCones);
	}

	if (RISING(BTN_MACRO_DEC) && gNumCones > 0) {
		--gNumCones;
		writeDebugStreamLine("%06d gNumCones= %d",nPgmTime,gNumCones);
	}

	if (FALLING(BTN_MACRO_ZERO))	writeDebugStreamLine("%06d MACRO_ZERO Released",nPgmTime,gNumCones);

	if (RISING(BTN_MACRO_ZERO)) {

		gNumCones = 0;
		writeDebugStreamLine("%06d gNumCones= %d",nPgmTime,gNumCones);
	}
}

#include "auto.c"
#include "auto_runs.c"


/* LCD */

void handleLcd()
{
	string line;

#ifdef DEBUG_TRACKING
	sprintf(line, "%3.2f %3.2f", gPosition.y, gPosition.x);
	clearLCDLine(0);
	displayLCDString(0, 0, line);

	sprintf(line, "%3.2f", radToDeg(gPosition.a));
	clearLCDLine(1);
	displayLCDString(1, 0, line);

	if (nLCDButtons) resetPositionFull(gPosition, 0, 0, 0);
#else
	sprintf(line, "%4d %4d %2d", gSensor[trackL].value, gSensor[trackR].value, gNumCones);
	clearLCDLine(0);
	displayLCDString(0, 0, line);

	velocityCheck(trackL);
	velocityCheck(trackR);

	sprintf(line, "%2.1f %2.1f %s%c", gSensor[trackL].velocity, gSensor[trackR].velocity, gAlliance == allianceRed ? "Red  " : "Blue ", '0' + gCurAuto);
	clearLCDLine(1);
	displayLCDString(1, 0, line);
#endif
}

// This function gets called 2 seconds after power on of the cortex and is the first bit of code that is run
void startup()
{
	clearDebugStream();
	writeDebugStreamLine("Code start");

	// Setup and initilize the necessary libraries
	setupMotors();
	setupSensors();
	setupJoysticks();
	tInit();

	mobileSetup();
	liftSetup();

	setupInvertedSen(jmpSkills);

	velocityClear(trackL);
	velocityClear(trackR);

	gJoy[JOY_TURN].deadzone = DZ_TURN;
	gJoy[JOY_THROTTLE].deadzone = DZ_THROTTLE;
	gJoy[JOY_LIFT].deadzone = DZ_LIFT;
	gJoy[JOY_ARM].deadzone = DZ_ARM;

	pidInit(gArmPID, 0.2, 0.001, 0.0, 50, 150, 5, 127);

	enableJoystick(JOY_TURN);
	enableJoystick(JOY_THROTTLE);
	enableJoystick(JOY_LIFT);
	enableJoystick(JOY_ARM);
	enableJoystick(BTN_ARM_DOWN);
	enableJoystick(BTN_MOBILE_TOGGLE);
	enableJoystick(BTN_MOBILE_MIDDLE);
	enableJoystick(BTN_MOBILE_BRAKES);
	enableJoystick(BTN_MACRO_ZERO);
	enableJoystick(BTN_MACRO_CLEAR);
	enableJoystick(BTN_MACRO_STACK);
	enableJoystick(BTN_MACRO_EXTERNAL);
	enableJoystick(BTN_MACRO_CANCEL);
	enableJoystick(BTN_MACRO_INC);
	enableJoystick(BTN_MACRO_DEC);
}

// This function gets called every 25ms during disabled (DO NOT PUT BLOCKING CODE IN HERE)
void disabled()
{
	updateSensorInput(autoPoti);
	selectAuto();
	handleLcd();
}

// This task gets started at the begining of the autonomous period
void autonomous()
{
	gAutoTime = nPgmTime;
	writeDebugStreamLine("Auto start %d", gAutoTime);

	startSensors(); // Initilize the sensors

	gKillDriveOnTimeout = true;

	resetPosition(gPosition);
	resetQuadratureEncoder(trackL);
	resetQuadratureEncoder(trackR);
	resetQuadratureEncoder(trackB);

	autoMotorSensorUpdateTaskAsync();
	trackPositionTaskAsync();

	runAuto();

	writeDebugStreamLine("Auto: %d ms", nPgmTime - gAutoTime);

	return_t;
}

// This task gets started at the beginning of the usercontrol period
void usercontrol()
{
	gUserControlTaskId = nCurrentTask;

	startSensors(); // Initilize the sensors
#if defined(DEBUG_TRACKING) || defined(TRACK_IN_DRIVER)
	initCycle(gMainCycle, 15, "main");
#else
	initCycle(gMainCycle, 10, "main");
#endif

	updateSensorInput(jmpSkills);

#if defined(DEBUG_TRACKING) || defined(TRACK_IN_DRIVER)
	trackPositionTaskAsync();
#endif

	if (gSensor[jmpSkills].value)
	{
		autoMotorSensorUpdateTaskAsync();
		trackPositionTaskAsync();

		driverSkillsStart();

		trackPositionTaskKill();
		autoMotorSensorUpdateTaskKill();

		mobileSet(mobileTop);

		gArmPosition = 2;
		gArmTarget = gArmPositions[2];
		gArmState = armHold;
	}

	gKillDriveOnTimeout = false;
	gDriveManual = true;

	while (true)
	{
		updateSensorInputs();
		updateJoysticks();

		selectAuto();

		handleDrive();
		handleLift();
		//handleArm();
		handleMobile();
		//handleMacros();

		if (RISING(BTN_MACRO_CANCEL))
			writeDebugStreamLine("%f %f %f", gPosition.y, gPosition.x, gPosition.a);

		handleLcd();

		updateSensorOutputs();
		updateMotors();
		endCycle(gMainCycle);
	}

	return_t;
}

ASYNC_ROUTINES
(
USE_ASYNC(autonomous)
USE_ASYNC(usercontrol)
USE_ASYNC(dropArm)
USE_ASYNC(stack)
USE_ASYNC(stackFromLoader)
USE_ASYNC(stackExternal)
USE_ASYNC(trackPositionTask)
USE_ASYNC(autoMotorSensorUpdateTask)
USE_ASYNC(autoSafetyTask)
USE_ASYNC(moveToTarget)
USE_ASYNC(turnToAngle)
USE_ASYNC(turnToTarget)
USE_MACHINE(mobile)
USE_MACHINE(lift)
)
