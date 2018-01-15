#pragma config(Sensor, in4,    liftPoti,       sensorPotentiometer)
#pragma config(Sensor, in5,    armPoti,        sensorPotentiometer)
#pragma config(Motor,  port2,           liftL,         tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           driveL1,       tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           driveL2,       tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           arm,           tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           driveR2,       tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           driveR1,       tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           liftR,         tmotorVex393HighSpeed_MC29, openLoop)

#define LIFT_TOP 37.66
#define LIFT_BOTTOM 5.25
#define LIFT_MID 20.25
#define LIFT_HOLD_DOWN_THRESHOLD 7.5
#define LIFT_HOLD_UP_THRESHOLD 34.5

#define LIFT_MID_POS 1900
#define LIFT_ARM_LEN 9

#define LIFT_HEIGHT(pos) (LIFT_MID + 2 * LIFT_ARM_LEN * sin((pos - LIFT_MID_POS) * PI / 2870))
#define LIFT_POS(height) (LIFT_MID_POS + asin((height - LIFT_MID) / (2 * LIFT_ARM_LEN)) * 2870 / PI)

#define ARM_TOP 3200
#define ARM_BOTTOM 550
#define ARM_PRESTACK 2300
#define ARM_CARRY 1500
#define ARM_STACK 2400
#define ARM_HORIZONTAL 1150

int gNumCones = 0;

int gLiftTarget;
int gArmTarget;
word gLiftPower;
word gArmPower;
word gLiftHoldPower;
word gArmHoldPower;
word gLiftBrakePower;
word gArmBrakePower;
bool gLiftDone;
bool gArmDone;

void setLift(word power)
{
	motor[liftL] = motor[liftR] = power;
}

void setArm(word power)
{
	motor[arm] = power;
}

task moveLiftUp()
{
	setLift(gLiftPower);
	int pos;
	while ((pos = SensorValue[liftPoti]) < gLiftTarget) sleep(10);
	if (gLiftBrakePower)
	{
		setLift(gLiftBrakePower);
		sleep(200);
		setLift(gLiftHoldPower);
	}
	else
		setLift(gLiftHoldPower);
	gLiftDone = true;
	writeDebugStreamLine("Lift moved up to %d | %d", gLiftTarget, pos);
}

task moveLiftDown()
{
	setLift(gLiftPower);
	int pos;
	while ((pos = SensorValue[liftPoti]) > gLiftTarget) sleep(10);
	if (gLiftBrakePower)
	{
		setLift(gLiftBrakePower);
		sleep(200);
		setLift(gLiftHoldPower);
	}
	else
		setLift(gLiftHoldPower);
	gLiftDone = true;
	writeDebugStreamLine("Lift moved down to %d | %d", gLiftTarget, pos);
}

task moveArmUp()
{
	setArm(gArmPower);
	int pos;
	while ((pos = SensorValue[armPoti]) < gArmTarget) sleep(10);
	if (gArmBrakePower)
	{
		setArm(gArmBrakePower);
		sleep(200);
		setArm(gArmHoldPower);
	}
	else
		setArm(gArmHoldPower);
	gArmDone = true;
	writeDebugStreamLine("Arm moved up to %d | %d", gArmTarget, pos);
}

task moveArmDown()
{
	setArm(gArmPower);
	int pos;
	while ((pos = SensorValue[armPoti]) > gArmTarget) sleep(10);
	if (gArmBrakePower)
	{
		setArm(gArmBrakePower);
		sleep(200);
		setArm(gArmHoldPower);
	}
	else
		setArm(gArmHoldPower);
	gArmDone = true;
	writeDebugStreamLine("Arm moved down to %d | %d", gArmTarget, pos);
}

void moveLiftUpTo(int pos, word power, word holdPower, word brakePower = 0)
{
	gLiftDone = false;
	gLiftTarget = pos;
	gLiftPower = power;
	gLiftHoldPower = holdPower;
	gLiftBrakePower = brakePower;
	startTask(moveLiftUp);
}

void moveLiftDownTo(int pos, word power, word holdPower, word brakePower = 0)
{
	gLiftDone = false;
	gLiftTarget = pos;
	gLiftPower = power;
	gLiftHoldPower = holdPower;
	gLiftBrakePower = brakePower;
	startTask(moveLiftDown);
}

void moveArmUpTo(int pos, word power, word holdPower, word brakePower = 0)
{
	gArmDone = false;
	gArmTarget = pos;
	gArmPower = power;
	gArmHoldPower = holdPower;
	gArmBrakePower = brakePower;
	startTask(moveArmUp);
}

void moveArmDownTo(int pos, word power, word holdPower, word brakePower = 0)
{
	gArmDone = false;
	gArmTarget = pos;
	gArmPower = power;
	gArmHoldPower = holdPower;
	gArmBrakePower = brakePower;
	startTask(moveArmDown);
}

task drive()
{
	bool lstInc = false, lstDec = false, lstReset = false;
	while (true)
	{
		word throttle = vexRT[Ch3];
		word turn = vexRT[Ch4];
		motor[driveL1] = motor[driveL2] = throttle + turn;
		motor[driveR1] = motor[driveR2] = throttle - turn;

		bool inc = vexRT[Btn5U];
		bool dec = vexRT[Btn5D];
		bool reset = vexRT[Btn7D];
		if (inc && !lstInc && gNumCones < 11) ++gNumCones;
		if (dec && !lstDec && gNumCones > 0) --gNumCones;
		if (reset && !lstReset) gNumCones = 0;
		lstInc = inc;
		lstDec = dec;
		lstReset = reset;

		sleep(10);
	}
}

//                                   0     1     2      3      4      5      6      7      8      9      10
const float gLiftRaiseTarget[11] = { 6.75, 10,   12.75, 15.75, 18.75, 21.75, 24.75, 28,    33,    36,    38 };
const float gLiftPlaceTarget[11] = { 5.25, 8,    10.6,  13.4,  16.2,  19,    21.8,  24.6,  27.4,  30.2,  33 };

task main()
{
	startTask(drive);
	while (true)
	{
		while (gNumCones == 11) sleep(10);
		while (!vexRT[Btn8D]) sleep(10);

		unsigned long timeStart = nPgmTime;

		moveLiftDownTo(LIFT_POS(LIFT_BOTTOM), -127, -15);
		while (SensorValue[liftPoti] > 1200) sleep(10);

		moveArmDownTo(ARM_BOTTOM + 340, -127, 12);
		while (!gLiftDone) sleep(10);
		while (!gArmDone) sleep(10);

		moveLiftUpTo(LIFT_POS(gLiftRaiseTarget[gNumCones]), 127, 7, -15);
		while (SensorValue[liftPoti] < LIFT_POS(gLiftPlaceTarget[gNumCones])) sleep(10);
		moveArmUpTo(ARM_STACK, 127, -12);
		while (!gLiftDone) sleep(10);
		while (!gArmDone) sleep(10);

		moveLiftDownTo(LIFT_POS(gLiftPlaceTarget[gNumCones]), -70, gNumCones == 0 ? -15 : 10);
		while (!gLiftDone) sleep(10);

		moveArmDownTo(ARM_HORIZONTAL, -127, 10, 25);
		while (SensorValue[armPoti] > ARM_PRESTACK) sleep(10);

		if (gNumCones <= 4)
			moveLiftUpTo(1500, 80, 10, -25);
		else
			moveLiftDownTo(1800, -80, -10, 15);
		while (!gArmDone)
		{
			sleep(10);
		}

		++gNumCones;

		writeDebugStreamLine("%d", nPgmTime - timeStart);
	}
}
