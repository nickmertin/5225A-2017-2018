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

	pidReset(PID);
}

bool pidCalculate(sPID& PID, float target, float input)
{
	if (PID.kI || PID.kD)
	{
		unsigned long time = nPgmTime;
		unsigned long deltaTime = time - PID.lstRunTime; // Calculate the time since PID last ran
		if (!PID.firstRun && deltaTime < 1) // If it hasn't been 1ms or more since PID last ran, don't run
			return false;
		PID.lstRunTime = time;

		float derivative = PID.firstRun ? 0 : (float)(input - PID.lstInput) / (float)(deltaTime); // Calculate the derivative, taking time into account
		PID.lstError = PID.error;
		PID.error = target - input; // Calculate error (the distance from the target)

		if (sgn(PID.error) != sgn(PID.lstError))
			PID.integral = 0.0;

		PID.lstIntegralSgn = sgn(PID.integral);

		if ((fabs(PID.error) > PID.kIInner && fabs(PID.error) < PID.kIOuter) || (PID.kIInner < 0 && PID.kIOuter < 0)) // If the error is within the acceptable range, update the integral
			PID.integral += PID.error * deltaTime;

		PID.output = PID.error * PID.kP + PID.integral * PID.kI - derivative * PID.kD; // Calculate the output
	}
	else
	{
		PID.error = target - input;
		PID.output = PID.error * PID.kP;
	}

	if (PID.minOutput >= 0 && abs(PID.output) < PID.minOutput)
		PID.output = PID.minOutput * sgn(PID.output);

	if (PID.maxOutput >= 0 && abs(PID.output) > PID.maxOutput)
		PID.output = PID.maxOutput * sgn(PID.output);

	PID.firstRun = false;

	return true;
}

void pidModify(sPID& PID, float kP, float kI, float kD)
{
	if (kP >= 0) PID.kP = kP;
	if (kI >= 0) PID.kI = kI;
	if (kD >= 0) PID.kD = kD;
}

void pidReset(sPID& PID)
{
	PID.error = 0; // Reset the error for a new set of PID values
	PID.lstError = 0; // Reset the last error for a new set of PID values
	PID.integral = 0; // Reset the integral for a new set of PID values
	PID.lstInput = 0; // Reset the last input for a new set of PID values
	PID.lstRunTime = nPgmTime;
	PID.firstRun = true;
}
