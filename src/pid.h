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
	bool firstRun; // True if lst- values are meaningless
} sPID; // A structure used to store data about an instance of a PID function

/* Functions */
void pidInit(sPID& PID, float kP, float kI, float kD, float kIInner, float kIOuter, float minOutput, float maxOutput); // Setup the PID structure
bool pidCalculate(sPID& PID, float target, float input); // Run PID, return false if you should not update the output
void pidModify(sPID& PID, float kP, float kI, float kD);
void pidReset(sPID& PID); // Reset accumulated values
