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

void setLift(word power)
{
	motor[liftL] = motor[liftR] = power;
}

void setArm(word power)
{
	motor[arm] = power;
}

task main()
{
	while (true)
	{
		while (!vexRT[Btn8D]) sleep(10);

		int liftTarget = LIFT_POS(LIFT_BOTTOM);
		setLift(-127);
		while (SensorValue[liftPoti] > liftTarget) sleep(10);
		setLift(-15);

		setArm(-127);
		while (SensorValue[armPoti] > ARM_BOTTOM + 150) sleep(10);
		setArm(25);
		sleep(150);
		setArm(80);
		while (SensorValue[armPoti] < ARM_BOTTOM + 650) sleep(10);
		setArm(12);

		liftTarget = LIFT_POS(9.0);
		setLift(127);
		while (SensorValue[liftPoti] < liftTarget) sleep(10);
		setLift(-10);
		sleep(200);
		setLift(7);

		setArm(127);
		while (SensorValue[armPoti] < ARM_STACK) sleep(10);
		setArm(-12);

		liftTarget = LIFT_POS(LIFT_BOTTOM);
		setLift(-70);
		while (SensorValue[liftPoti] > liftTarget) sleep(10);
		setLift(-15);

		sleep(400);

		setArm(-127);
		while (SensorValue[armPoti] > ARM_PRESTACK) sleep(10);
		setArm(12);
		sleep(200);
		setArm(7);
	}
}
