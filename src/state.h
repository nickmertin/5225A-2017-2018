/* /////////////// State Machine Macros ////////////////// */
/* Create machine using:
	CREATE_MACHINE (-----)
	{
		---
	}
*/
#define CREATE_MACHINE_5(machine, sensor, state0, state1, state2, state3, state4, type1, arg1Name, type2, arg2Name) \
const int machine##StateCount = 5; \
typedef enum _tStates##machine \
{ \
	machine##state0, \
	machine##state1, \
	machine##state2, \
	machine##state3, \
	machine##state4 \
}tStates##machine; \
\
tStates##machine machine##State = machine##state0; \
float machine##VelSafetyThresh = -1; \
long long machine##Timeout; \
type1 machine##arg1Name; \
type2 machine##arg2Name; \
int machine##VelSafetyCount = 0; \
void machine##StateChange(int stateIn, long timeout = -1, float velSafetyThresh = -1, type1 arg1In = -1, type2 arg2In = -1) \
{ \
	if (machine##State != stateIn) \
	{ \
		machine##VelSafetyCount = 0; \
		writeDebugStreamLine("local timeout = %d", timeout); \
		if (timeout == -1) \
			machine##Timeout = timeout; \
		else \
			machine##Timeout = timeout + npgmtime; \
		machine##VelSafetyThresh = -1; \
		machine##State = stateIn; \
		machine##arg1Name = arg1In; \
		machine##arg2Name = arg2In;  \
		writeDebugStreamLine ("%d" #machine "State:%d, %d, %d", npgmTime, machine##State, machine##arg1Name, machine##arg2Name); \
	} \
} \
\
void machine##VelSafetyCheck (bool useTracking = false) \
{ \
	if (machine##VelSafetyThresh != -1 && machine##VelSafetyThresh != 0) \
	{ \
		if (!useTracking) \
		{ \
			velocityCheck(sensor); \
			if (gSensor[sensor].velGood) \
				if ( (sgn(machine##VelSafetyThresh) == 1)? (gSensor[sensor].velocity < machine##VelSafetyThresh) :  (gSensor[sensor].velocity > machine##VelSafetyThresh) ) \
					machine##VelSafetyCount ++; \
		} \
	} \
} \
\
void machine##SafetyCheck(int timedOutState = machine##state0, type1 machine##arg1Name = -1, type2 machine##arg2Name = -1) \
{ \
		bool timedOut = false; \
		bool velSafety = false; \
		if (!NOT_T_O(machine)) \
			timedOut = true; \
		if (machine##VelSafetyCount > 10) \
			velSafety = true; \
		\
		if (velSafety || timedOut) \
		{ \
			writeDebugStreamLine("%d" #machine "safety: Timedout? %d %d VelSafety? %d", npgmTime, timedout, machine##Timeout, velSafety); \
			machine##StateChange(timedOutState, machine##arg1Name, machine##arg2Name); \
		} \
} /*\
task machine##Set () \ */

#define NOT_T_O(machineIn) ( (machineIn##Timeout == -1)? 1 : (npgmTime < machineIn##Timeout) )

#define WHILE(machineIn) while(NOT_T_O(machineIn) && machineIn##VelSafetyCount < 10 &&

/* Test macros: */
/*
CREATE_MACHINE(thing, Idle, Hold, Manual, Move, MoveSimple, int, A, int, B)
{
	thingState = thingIdle;
	while (true)
	{
		sleep(10);
	}
}
*/

/* /////////////// Macros to Convert Functions to States ////////////////// */

//Add other funcs to a machine starting at __ val - call in header
#define ADD_FUNCS_TO_MACHINE_1(machine, func0) \
typedef enum _tFuncStates##machine \
{ \
	machine##func0 = machine##StateCount \
}tFuncStates##machine; \
const int func0##Loc = machine##StateCount

#define ADD_FUNCS_TO_MACHINE_5(machine, func0, func1, func2, func3, func4, func5) \
typedef enum _tFuncStates##machine \
{ \
	machine##func0 = machine##StateCount, \
	machine##func1, \
	machine##func2, \
	machine##func3, \
	machine##func4, \
	machine##func5 \
}tFuncStates##machine; \
const int func0##Loc = machine##StateCount; \
const int func1##Loc = machine##StateCount + 1; \
const int func2##Loc = machine##StateCount + 2; \
const int func3##Loc = machine##StateCount + 3; \
const int func4##Loc = machine##StateCount + 4; \
const int func5##Loc = machine##StateCount + 5

//Create global variables for all args of a func - call in header
#define PREP_FUNC_STATE_4(func, type1, type2, type3, type4) \
const int func##ArgCount = 4; \
type1 func##Arg1 = -1; \
type1 func##Arg2 = -1; \
type1 func##Arg3 = -1; \
type1 func##Arg4 = -1

//Assign to all func args - call before putting machine into the func state
#define ASSIGN_FUNC_STATE_4(func, arg1In, arg2In, arg3In, arg4In) \
func##Arg1 = arg1In; \
func##Arg2 = arg2In; \
func##Arg3 = arg3In; \
func##Arg4 = arg4In

//Add function to the machine - call when defining switch statement
#define CALL_FUNC_STATE_4(func) func(func##Arg1, func##Arg2, func##Arg3, func##Arg4)

#define ADD_FUNC_TO_SWITCH_4(func, machine, nextState) \
case (func##Loc): \
{ \
	CALL_FUNC_STATE_4(func); \
	machine##SafetyCheck(); \
	machine##StateChange(nextState); \
	break; \
}
