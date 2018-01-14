#pragma config(Sensor, in4,    liftPoti,       sensorPotentiometer)
#pragma config(Sensor, in5,    armPoti,        sensorPotentiometer)
#pragma config(Motor,  port2,           liftL,         tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           arm,           tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           liftR,         tmotorVex393HighSpeed_MC29, openLoop)

#define LIM_TO_VAL(input, val) (abs(input) > (val) ? (val) * sgn(input) : (input))
#define LIM_TO_VAL_SET(input, val) input = LIM_TO_VAL(input, val)

#define LIFT_TOP 37.66
#define LIFT_BOTTOM 5
#define LIFT_MID 20.25
#define LIFT_HOLD_DOWN_THRESHOLD 7.5
#define LIFT_HOLD_UP_THRESHOLD 34.5
#define LIFT_LOADER 21.5

#define LIFT_MID_POS 1900
#define LIFT_ARM_LEN 9

#define LIFT_HEIGHT(pos) (LIFT_MID + 2 * LIFT_ARM_LEN * sin((pos - LIFT_MID_POS) * PI / 2870))
#define LIFT_POS(height) (LIFT_MID_POS + asin((height - LIFT_MID) / (2 * LIFT_ARM_LEN)) * 2870 / PI)

#define ARM_TOP 3200
#define ARM_BOTTOM 550
#define ARM_PRESTACK 2100
#define ARM_CARRY 1500
#define ARM_STACK 2600
#define ARM_HORIZONTAL 1150

int gNumCones = 5;

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

task moveLiftUpFancy()
{
	//setLift(gLiftPower);
	//int pos;
	//while ((pos = SensorValue[liftPoti]) < gLiftTarget) sleep(10);
	unsigned long begin = nPgmTime;
	float target = LIFT_HEIGHT(gLiftTarget);
	float last = LIFT_HEIGHT(SensorValue[liftPoti]);
	writeDebugStreamLine("liftToTarget %f -> %f", last, target);
	const float kP_vel = 0.01;
	const float kI_vel = 0.00001;
	float kP_pwr = 2500.0;
	float err;
	float vel;
	float integral = 0;
	float epsilon;
	int poti;
	sleep(20);
	do
	{
		poti = SensorValue[liftPoti];
		float cur = LIFT_HEIGHT(poti);
		vel = (cur - last) / 20;
		err = target - cur;
		if (fabs(err) < 3.0)
			integral += err * 20;
		else
			integral = 0;
		float power = kP_pwr * (kP_vel * err + kI_vel * integral - vel);
		if (sgn(power) == -sgn(vel)) power *= 0.05;
		LIM_TO_VAL_SET(power, 127);
		setLift((word)power);
		last = cur;
		epsilon = 20 * vel;
		LIM_TO_VAL_SET(epsilon, 4);
		if (fabs(epsilon) < 1.0) epsilon = sgn(vel);
		sleep(20);
	} while (err > epsilon);
	writeDebugStreamLine("Lift at target: %f %f %f | %d | %d ms", target, LIFT_HEIGHT(SensorValue[liftPoti]), err, poti, nPgmTime - begin);
	if (gLiftBrakePower)
	{
		setLift(gLiftBrakePower);
		sleep(200);
		setLift(gLiftHoldPower);
	}
	else
		setLift(gLiftHoldPower);
	gLiftDone = true;
	writeDebugStreamLine("Lift moved up to %d | %d", gLiftTarget, poti);
}

task moveLiftDownFancy()
{
	//setLift(gLiftPower);
	//int pos;
	//while ((pos = SensorValue[liftPoti]) > gLiftTarget) sleep(10);

	unsigned long begin = nPgmTime;
	float target = LIFT_HEIGHT(gLiftTarget);
	float last = LIFT_HEIGHT(SensorValue[liftPoti]);
	writeDebugStreamLine("liftToTarget %f -> %f", last, target);
	const float kP_vel = 0.01;
	const float kI_vel = 0.00001;
	float kP_pwr = 6000.0;
	float err;
	float vel;
	float integral = 0;
	float epsilon;
	int poti;
	sleep(20);
	do
	{
		poti = SensorValue[liftPoti];
		float cur = LIFT_HEIGHT(poti);
		vel = (cur - last) / 20;
		err = target - cur;
		if (fabs(err) < 3.0)
			integral += err * 20;
		else
			integral = 0;
		float power = kP_pwr * (kP_vel * err + kI_vel * integral - vel);
		if (sgn(power) == -sgn(vel)) power *= 0.05;
		LIM_TO_VAL_SET(power, 127);
		setLift((word)power);
		last = cur;
		epsilon = 20 * vel;
		LIM_TO_VAL_SET(epsilon, 4);
		if (fabs(epsilon) < 1.0) epsilon = sgn(vel);
		sleep(20);
	} while (err < epsilon);
	writeDebugStreamLine("Lift at target: %f %f %f | %d | %d ms", target, LIFT_HEIGHT(SensorValue[liftPoti]), err, poti, nPgmTime - begin);
	if (gLiftBrakePower)
	{
		setLift(gLiftBrakePower);
		sleep(200);
		setLift(gLiftHoldPower);
	}
	else
		setLift(gLiftHoldPower);
	gLiftDone = true;
	writeDebugStreamLine("Lift moved down to %d | %d", gLiftTarget, poti);
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

void moveLiftUpToFancy(int pos, word power, word holdPower, word brakePower = 0)
{
	gLiftDone = false;
	gLiftTarget = pos;
	gLiftPower = power;
	gLiftHoldPower = holdPower;
	gLiftBrakePower = brakePower;
	startTask(moveLiftUpFancy);
}

void moveLiftDownToFancy(int pos, word power, word holdPower, word brakePower = 0)
{
	gLiftDone = false;
	gLiftTarget = pos;
	gLiftPower = power;
	gLiftHoldPower = holdPower;
	gLiftBrakePower = brakePower;
	startTask(moveLiftDownFancy);
}

void moveLiftToFancy(int pos, word power, word holdPower, word brakePower = 0)
{
	if (SensorValue[liftPoti] < pos)
		moveLiftUpToFancy(pos, power, holdPower, -brakePower);
	else
		moveLiftDownToFancy(pos, -power, holdPower, brakePower);
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

void moveLiftTo(int pos, word power, word holdPower, word brakePower = 0)
{
	if (SensorValue[liftPoti] < pos)
		moveLiftUpTo(pos, power, holdPower, -brakePower);
	else
		moveLiftDownTo(pos, -power, holdPower, brakePower);
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

void moveArmTo(int pos, word power, word holdPower, word brakePower = 0)
{
	if (SensorValue[armPoti] < pos)
		moveArmUpTo(pos, power, holdPower, -brakePower);
	else
		moveArmDownTo(pos, -power, holdPower, brakePower);
}

task main()
{
	unsigned long timeStart = 0;
	while (true)
	{
		moveLiftToFancy(LIFT_POS(LIFT_LOADER), 127, 12, 25);
		while (SensorValue[liftPoti] < 1200) sleep(10);
		moveArmTo(ARM_HORIZONTAL, 127, 10, 25);
		while (!gLiftDone) sleep(10);
		while (!gArmDone) sleep(10);

		writeDebugStreamLine("%d", nPgmTime - timeStart);

		while (!vexRT[Btn8D]) sleep(10);

		timeStart = nPgmTime;

		moveLiftToFancy(LIFT_POS(13.5), 70, 10, 15);
		while (!gLiftDone) sleep(10);

		if (gNumCones == 0)
		{
			moveArmUpTo(ARM_STACK, 127, -12);
			while (SensorValue[armPoti] < ARM_HORIZONTAL + 200) sleep(10);
			moveLiftDownTo(LIFT_POS(LIFT_BOTTOM), -70, -15);
			while (!gArmDone) sleep(10);
			while (!gLiftDone) sleep(10);

			moveArmDownTo(2250, -127, 7);
			while (!gArmDone) sleep(10);
		}
		else if (gNumCones == 3)
		{
			moveLiftUpTo(LIFT_POS(15), 127, 10, -20);
			moveArmUpTo(ARM_STACK, 127, -12);
			while (!gLiftDone) sleep(10);
			while (!gArmDone) sleep(10);

			moveLiftDownTo(LIFT_POS(11), -70, 10);
			while (!gLiftDone) sleep(10);

			moveArmDownTo(2250, -127, 12, 25);
			while (!gArmDone) sleep(10);
		}
		else if (gNumCones == 4)
		{
			moveLiftUpTo(LIFT_POS(16.5), 127, 10, -20);
			moveArmUpTo(ARM_STACK, 127, -12);
			while (!gLiftDone) sleep(10);
			while (!gArmDone) sleep(10);

			moveLiftDownTo(LIFT_POS(15), -70, 10);
			while (!gLiftDone) sleep(10);

			moveArmDownTo(2250, -127, 0);
			while (!gArmDone) sleep(10);

			while (!vexRT[Btn8D]) sleep(10);
		}
		else if (gNumCones == 5)
		{
			moveLiftUpTo(LIFT_POS(18), 127, 10, -20);
			moveArmUpTo(ARM_STACK, 127, -12);
			while (!gLiftDone) sleep(10);
			while (!gArmDone) sleep(10);

			moveLiftDownTo(LIFT_POS(17), -70, 10);
			while (!gLiftDone) sleep(10);

			moveArmDownTo(2250, -127, 0);
			while (!gArmDone) sleep(10);

			//while (!vexRT[Btn8D]) sleep(10);
		}
	}
}
