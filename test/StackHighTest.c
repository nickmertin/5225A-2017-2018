#pragma config(Sensor, in4,    liftPoti,       sensorPotentiometer)
#pragma config(Sensor, in5,    armPoti,        sensorPotentiometer)
#pragma config(Motor,  port2,           liftL,         tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           arm,           tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           liftR,         tmotorVex393HighSpeed_MC29, openLoop)

#define LIFT_TOP 38.16
#define LIFT_BOTTOM 5.75
#define LIFT_MID 20.75
#define LIFT_HOLD_DOWN_THRESHOLD 8
#define LIFT_HOLD_UP_THRESHOLD 35

#define LIFT_MID_POS 1900
#define LIFT_ARM_LEN 9

#define LIFT_HEIGHT(pos) (LIFT_MID + 2 * LIFT_ARM_LEN * sin((pos - LIFT_MID_POS) * PI / 2870))
#define LIFT_POS(height) (LIFT_MID_POS + asin((height - LIFT_MID) / (2 * LIFT_ARM_LEN)) * 2870 / PI)

#define ARM_TOP 3200
#define ARM_BOTTOM 550
#define ARM_PRESTACK 2100
#define ARM_CARRY 1500
#define ARM_STACK 2700

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
	while (SensorValue[liftPoti] < gLiftTarget) sleep(10);
	if (gLiftBrakePower)
	{
		setLift(gLiftBrakePower);
		sleep(200);
		setLift(gLiftHoldPower);
	}
	else
		setLift(gLiftHoldPower);
	gLiftDone = true;
}

task moveLiftDown()
{
	setLift(gLiftPower);
	while (SensorValue[liftPoti] > gLiftTarget) sleep(10);
	if (gLiftBrakePower)
	{
		setLift(gLiftBrakePower);
		sleep(200);
		setLift(gLiftHoldPower);
	}
	else
		setLift(gLiftHoldPower);
	gLiftDone = true;
}

task moveArmUp()
{
	setArm(gArmPower);
	while (SensorValue[armPoti] < gArmTarget) sleep(10);
	if (gArmBrakePower)
	{
		setArm(gArmBrakePower);
		sleep(200);
		setArm(gArmHoldPower);
	}
	else
		setArm(gArmHoldPower);
	gArmDone = true;
}

task moveArmDown()
{
	setArm(gArmPower);
	while (SensorValue[armPoti] > gArmTarget) sleep(10);
	if (gArmBrakePower)
	{
		setArm(gArmBrakePower);
		sleep(200);
		setArm(gArmHoldPower);
	}
	else
		setArm(gArmHoldPower);
	gArmDone = true;
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

task main()
{
	while (true)
	{
		while (!vexRT[Btn8D]) sleep(10);

		unsigned long timeStart = nPgmTime;

		moveLiftDownTo(LIFT_POS(LIFT_BOTTOM), -127, -15);
		while (!gLiftDone) sleep(10);

		moveArmDownTo(ARM_BOTTOM + 380, -127, 12);
		while (!gArmDone) sleep(10);

		moveLiftUpTo(LIFT_POS(15.15), 127, 7, -15);
		int liftThreshold = LIFT_POS(10);
		while (SensorValue[liftPoti] < liftThreshold) sleep(10);

		moveArmUpTo(ARM_STACK, 127, -12);
		while (!gLiftDone) sleep(10);
		while (!gArmDone) sleep(10);

		moveLiftDownTo(LIFT_POS(11), -70, -15);
		while (!gLiftDone) sleep(10);

		moveArmDownTo(ARM_PRESTACK, -127, 7, 12);
		while (!gArmDone) sleep(10);

		moveLiftDownTo(LIFT_POS(LIFT_BOTTOM), -70, -15);
		while (!gLiftDone) sleep(10);

		writeDebugStreamLine("%d", nPgmTime - timeStart);
	}
}
