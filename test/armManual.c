#pragma config(Sensor, in1,    poti1,          sensorPotentiometer)
#pragma config(Sensor, in2,    poti2,          sensorPotentiometer)
#pragma config(Sensor, dgtl1,  led1,           sensorLEDtoVCC)
#pragma config(Motor,  port2,           motorAR,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           motorAL,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           motorB,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           motorC,        tmotorServoStandard, openLoop)

typedef struct s_pos {
	float x;
	float y;
} pos;

#define DEG_PER_POT 18.2
#define BEAM_R 16
#define BEAM_S 16
#define CAL_BASE 180
#define A_VERTICAL 2668
#define B_VERTICAL 3051

void calculatePosition(pos &position, float a, float b, float R, float S)
{
	float cb = cosDegrees(b);
	float Scb = S * cb;
	float D = sqrt((R * R) + (S * S) - (2 * R * Scb));
	float g = acos((R - Scb) / D) * 180 / PI;
	position.x = D * sinDegrees(a - g);
	position.y = D * -cosDegrees(a - g);
}


float calculateTargetA(float x, float y, float R, float S)
{
	float Dsq = (x * x) + (y * y);
	float D = sqrt(Dsq);
	return (acos(((R * R) + Dsq - (S * S)) / (2 * R * D)) * 180 / PI) + 90 + (atan2(y, x) * 180 / PI);
}

float calculateTargetB(float x, float y, float R, float S)
{
	return acos(((R * R) + (S * S) - (x * x) - (y * y)) / (2 * R * S)) * 180 / PI;
}

float potiToDeg(short poti, short cal)
{
	return (poti + (CAL_BASE * DEG_PER_POT) - cal) / (float)DEG_PER_POT;
}

float degToPoti(float deg, short cal)
{
	return (deg * DEG_PER_POT) - (CAL_BASE * DEG_PER_POT) + cal;
}

task safety()
{
	SensorValue[led1] = 0;
	while (true)
	{
		if (SensorValue[poti1] > 3300 || SensorValue[poti2] > 3400 || SensorValue[poti1] < 200)
		{
			stopTask(0);
			for (int i = 0; i < 10; ++i) motor[i] = 0;
			SensorValue[led1] = 1;
			return;
		}
		sleep(10);
	}
}

/* Structures */
typedef struct _sPID
{
	float kP; // The multiplier for the error
	float kI; // The multiplier for the integral
	float kD; // The multiplier for the derivative
	float kIInner; // The integral will only be edited if the abs error is above this value
	float kIOuter; // The integral will only be edited if the abs error is below this value
	float error;
	float lstError;
	float integral; // The integral (error accumulated over time) for the PID function
	byte lstIntegralSgn; // The sign of the last integral
	unsigned long lstRunTime; // The time that this PID function last ran
	float lstInput; // The last value of the input for this PID function, used to calculate derivative
	float output; // The output of this PID function
	float minOutput; // The minimum output PID will provide
	float maxOutput; // The maximum output PID will provide
} sPID; // A structure used to store data about an instance of a PID function

/* Functions */
void pidInit(sPID& PID, float kP, float kI, float kD, float kIInner, float kIOuter, float minOutput, float maxOutput); // Setup the PID structure
bool pidCalculate(sPID& PID, float target, float input); // Run PID, return false if you should not update the output
void pidModify(sPID& PID, float kP, float kI, float kD);

sPID pidArm1, pidArm2;

int runArm(sPID& PID, float& target, int& last, TVexJoysticks input, tSensors poti) {
	int cur = vexRT[input];

	if (abs(cur) < 10) {
		cur = 0;
	}

	int result;

	int sensor = SensorValue[poti];

	if (cur) {
		result = cur;
	}
	else {
		if (last) {
			target = (float) (sensor + 5 * last);
		}
		pidCalculate(PID, target, (float) sensor);
		result = (int) PID.output;
	}

	if (sensor >= 3700 && result > 0) {
		result = 0;
	}
	if (sensor <= 300 && result < 0) {
		result = 0;
	}

	last = cur;
	return result;
}

task main()
{
	pidInit(pidArm1, 3, 0, 0, 0, 0, 0, 127);
	pidInit(pidArm2, 2.5, 0, 0, 0, 0, 0, 127);

	float targetA = potiToDeg((float) SensorValue[poti1], A_VERTICAL);
	float targetB = potiToDeg((float) SensorValue[poti2], B_VERTICAL);

	pos position;

	calculatePosition(position, targetA, targetB, BEAM_R, BEAM_S);

	while(true) {
		//motor[motorAR] = motor[motorAL] = runArm(pidArm1, target1, last1, Ch2, poti1);
		//motor[motorB] = runArm(pidArm2, target2, last2, Ch3, poti2);

		motor[motorC] = vexRT[Btn6U] ? 75 : -70;

		float deg1 = potiToDeg(SensorValue[poti1], A_VERTICAL);
		float deg2 = potiToDeg(SensorValue[poti2], B_VERTICAL);

		int joyX = vexRT[Ch1];
		int joyY = vexRT[Ch3];

		if (abs(joyX) > 10) {
			position.x += (float) joyX / 500.0;
		}
		if (abs(joyY) > 10) {
			position.y += (float) joyY / 500.0;
		}

		targetA = calculateTargetA(position.x, position.y, BEAM_R, BEAM_S);
		targetB = calculateTargetB(position.x, position.y, BEAM_R, BEAM_S);

		if (pidCalculate(pidArm1, targetA, deg1)) motor[motorAR] = motor[motorAL] = pidArm1.output;
		if (pidCalculate(pidArm2, targetB, deg2)) motor[motorB] = pidArm2.output;

		sleep(10);
	}
}


/* Functions */
void pidInit(sPID& PID, float kP, float kI, float kD, float kIInner, float kIOuter, float minOutput, float maxOutput)
{
	// Update the PID structure
	PID.kP = kP;
	PID.kI = kI;
	PID.kD = kD;
	PID.kIInner = kIInner;
	PID.kIOuter = kIOuter;
	PID.minOutput = minOutput;
	PID.maxOutput = maxOutput;

	PID.error = 0; // Reset the error for a new set of PID values
	PID.lstError = 0; // Reset the last error for a new set of PID values
	PID.integral = 0; // Reset the integral for a new set of PID values
	PID.lstInput = 0; // Reset the last input for a new set of PID values
	PID.lstRunTime = nPgmTime;
}

bool pidCalculate(sPID& PID, float target, float input)
{
	if (PID.kI || PID.kD)
	{
		unsigned long time = nPgmTime;
		unsigned long deltaTime = time - PID.lstRunTime; // Calculate the time since PID last ran
		if (deltaTime < 1) // If it hasn't been 1ms or more since PID last ran, don't run
			return false;
		PID.lstRunTime = time;

		float derivative = (float)(input - PID.lstInput) / (float)(deltaTime); // Calculate the derivative, taking time into account
		PID.lstError = PID.error;
		PID.error = target - input; // Calculate error (the distance from the target)

		if (sgn(PID.error) != sgn(PID.error))
			PID.integral = 0.0;

		PID.lstIntegralSgn = sgn(PID.integral);

		if ((fabs(PID.error) > PID.kIInner && fabs(PID.error) < PID.kIOuter) || (PID.kIInner < 0 && PID.kIOuter < 0)) // If the error is within the acceptable range, update the integral
			PID.integral += PID.error * deltaTime;

		PID.output = PID.error * PID.kP + PID.integral * PID.kI - derivative * PID.kD; // Calculate the output
	}
	else
		PID.output = (PID.error = (target - input)) * PID.kP;

	if (PID.minOutput >= 0 && abs(PID.output) < PID.minOutput)
		PID.output = 0;

	if (PID.maxOutput >= 0 && abs(PID.output) > PID.maxOutput)
		PID.output = PID.maxOutput * sgn(PID.output);

	return true;
}

void pidModify(sPID& PID, float kP, float kI, float kD)
{
	if (kP >= 0) PID.kP = kP;
	if (kI >= 0) PID.kI = kI;
	if (kD >= 0) PID.kD = kD;
}
