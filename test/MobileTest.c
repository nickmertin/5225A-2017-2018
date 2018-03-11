#pragma config(Sensor, in1,    autoPoti,       sensorPotentiometer)
#pragma config(Sensor, in2,    mobilePoti,     sensorPotentiometer)
#pragma config(Sensor, in3,    liftPoti,       sensorPotentiometer)
#pragma config(Sensor, in4,    armPoti,        sensorPotentiometer)
#pragma config(Sensor, in5,    expander,       sensorAnalog)
#pragma config(Sensor, in6,    lsBarL,         sensorReflection)
#pragma config(Sensor, in7,    lsBarR,         sensorReflection)
#pragma config(Sensor, dgtl1,  trackL,         sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  trackR,         sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  trackB,         sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  sonarL,         sensorSONAR_mm)
#pragma config(Sensor, dgtl9,  limMobile,      sensorTouch)
#pragma config(Sensor, dgtl10, jmpSkills,      sensorDigitalIn)
#pragma config(Sensor, dgtl11, sonarR,         sensorSONAR_mm)
#pragma config(Motor,  port2,           liftL,         tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           driveL1,       tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           driveL2,       tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           arm,           tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           mobile,        tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           driveR2,       tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           driveR1,       tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           liftR,         tmotorVex393HighSpeed_MC29, openLoop)

#define MOBILE_IN 2200
#define MOBILE_OUT 400
#define MOBILE_TOP 1500
#define MOBILE_SLOW (MOBILE_TOP + 150)
#define MOBILE_SLOWER (MOBILE_OUT + 800)

bool mobileInRobot = SensorValue[mobilePoti] > MOBILE_TOP;

void setMobile(word power)
{
	motor[mobile] = power;
}

void setDrive(word left, word right)
{
	motor[driveL1] = motor[driveL2] = left;
	motor[driveR1] = motor[driveR2] = right;
}

float map(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

task mobileOut()
{
	mobileInRobot = false;
	setMobile(-50);
	while (SensorValue[mobilePoti] > MOBILE_SLOW)
	{
		setMobile(round(map(SensorValue[mobilePoti], MOBILE_SLOW, MOBILE_IN, -30, -127)));
		sleep(1);
	}
	setMobile(-10);
	while (SensorValue[mobilePoti] > MOBILE_SLOWER) sleep(1);
	setMobile(-5);
	while (SensorValue[mobilePoti] > MOBILE_OUT) sleep(1);
	setMobile(-10);
}

task mobileIn()
{
	mobileInRobot = true;
	setMobile(127);
	while (SensorValue[mobilePoti] < MOBILE_TOP) sleep(1);
	setMobile(10);
}

void stopMobile()
{
	stopTask(mobileOut);
	stopTask(mobileIn);
}

bool mobileBtn = false, mobileLst = false;

task main()
{
	while (true)
	{
		mobileBtn = (bool)vexRT[Btn6U];

		if (mobileBtn && !mobileLst)
		{
			stopMobile();

			if (mobileInRobot)
				startTask(mobileOut);
			else
				startTask(mobileIn);
		}

		word y = vexRT[Ch3];
		word a = vexRT[Ch4];
		if (abs(y) < 15) y = 0;
		if (abs(a) < 15) a = 0;
		setDrive(y + a, y - a);

		mobileLst = mobileBtn;
	}
}
