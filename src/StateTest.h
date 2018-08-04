typedef enum _tVelDir
{
	velEither = -1,
	velUp = 0,
	velDown = 1
}tVelDir;
/// new
typedef enum _tVelType
{
	velSensor,
	velLocalY,
	velAngle
} tVelType;
///

/* /////////////// State Machine Macros (For X Params) ////////////////// */
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
tVelDir machine##VelSafetyDir = -1; \
unsigned long machine##Timeout; \
type1 machine##arg1Name; \
type2 machine##arg2Name; \
int machine##VelSafetyCount = 0; \
unsigned long machine##StateStartTime = 0; \
unsigned long machine##StateCycCount = 0; \
bool machine##Logs = 0; \
void machine##StateChange(int stateIn, long timeout = -1, float velSafetyThresh = -1, tVelDir velDir = -1, type1 arg1In = -1, type2 arg2In = -1) \
{ \
	if (machine##State != stateIn) \
	{ \
		unsigned long curTime = npgmtime; \
		if (timeout <= 0) \
		{ \
			machine##Timeout = 0; \
		} \
		else \
		{ \
			machine##Timeout = ( timeout + curTime ); \
		} \
		\
		machine##VelSafetyCount = 0; \
		machine##StateStartTime = curTime; \
		machine##StateCycCount = 0; \
		machine##VelSafetyThresh = velSafetyThresh; \
		machine##VelSafetyDir = velDir; \
		machine##State = stateIn; \
		machine##arg1Name = arg1In; \
		machine##arg2Name = arg2In;  \
		writeDebugStreamLine ("%d" #machine "State:%d, TO:%d velS:%f, %d, %d", npgmTime, machine##State, machine##timeout, machine##VelSafetyThresh, machine##arg1Name, machine##arg2Name); \
	} \
} \
\
void machine##VelSafetyCheck (tVelType velType = velSensor) \
{ \
	if (machine##VelSafetyThresh != -1 && machine##VelSafetyThresh != 0) \
	{ \
		if (machine##VelSafetyDir == velEither || machine##VelSafetyDir == velUp) \
			machine##VelSafetyThresh = abs(machine##VelSafetyThresh); \
		else if (machine##VelSafetyDir == velDown) \
			machine##VelSafetyThresh = -1 * abs(machine##VelSafetyThresh); \
	\
	tHog(); \
		float out = 0; \
		bool goodVel = false; \
		switch (velType) \
		{ \
			case velSensor: \
			{ \
				if (machine##StateCycCount == 0) \
					velocityClear(sensor); \
				velocityCheck(sensor); \
				if (gSensor[sensor].velGood) \
				{ \
					out = gSensor[sensor].velocity; \
					goodVel = true; \
				} \
				break; \
			} \
			case velLocalY: \
			{ \
				out = gVelocity.x * sin(gPosition.a) + gVelocity.y * cos(gPosition.a); \
				goodVel = true; \
				break; \
			} \
			case velAngle: \
			{ \
				out = gVelocity.a; \
				goodVel = true; \
				break; \
			} \
		} \
		unsigned long curTime = npgmTime; \
		if (goodVel && curTime-machine##StateStartTime > 100) \
		{ \
				if (machine##VelSafetyDir == velEither) \
				{ \
					if ( out < abs(machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount++; \
						if(machine##Logs) writeDebugStreamLine("%d:"#machine"velSafety %f", npgmtime, out); \
					} \
				} \
				else \
				{ \
					if ( (sgn(machine##VelSafetyThresh) == 1)? (out < machine##VelSafetyThresh) :  (out > machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount++; \
						if(machine##Logs) writeDebugStreamLine("%d:"#machine"velSafety %f", npgmtime, out); \
					} \
				} \
		} \
		tRelease(); \
	} \
} \
\
void machine##SafetyCheck(int timedOutState = machine##state0, type1 machine##arg1Name = -1, type2 machine##arg2Name = -1) \
{ \
		bool timedOut = false; \
		bool velSafety = false; \
		if (!NOT_T_O(machine)) \
			timedOut = true; \
		if (machine##VelSafetyCount >= 10) \
			velSafety = true; \
		\
		if (velSafety || timedOut) \
		{ \
			writeDebugStreamLine("%d" #machine "safety: Timedout? %d at %d VelSafety? %d", npgmTime, timedout, machine##Timeout, velSafety); \
			machine##StateChange(timedOutState, machine##arg1Name, machine##arg2Name); \
		} \
} /*\
task machine##Set () \ */

/* Universal State Macros */
#define NOT_T_O(machineIn) ( (machineIn##Timeout <= 0)? 1 : (npgmTime < machineIn##Timeout) )

#define WHILE(machineIn, condition) while( NOT_T_O(machineIn) && machineIn##VelSafetyCount < 10 && (condition) )

#define SAFETY_CHECK(machineIn) (NOT_T_O(machineIn) && machineIn##VelSafetyCount < 10) ) \
{ \
	machineIn##VelSafetyCheck();

#define LOG(machineIn) if(machineIn##Logs) writeDebugStreamLine


#define END_STATE(machineIn) \
machine##StateCycCount++; \
break

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
type2 func##Arg2 = -1; \
type3 func##Arg3 = -1; \
type4 func##Arg4 = -1;

//Assign to all func args - call before putting machine into the func state
#define ASSIGN_FUNC_STATE_4(func, arg1In, arg2In, arg3In, arg4In) \
func##Arg1 = arg1In; \
func##Arg2 = arg2In; \
func##Arg3 = arg3In; \
func##Arg4 = arg4In;

//Add function to the machine - call when defining switch statement
#define CALL_FUNC_STATE_4(func) func(func##Arg1, func##Arg2, func##Arg3, func##Arg4)

#define ADD_FUNC_TO_SWITCH_4(func, machine, nextState, safetyState) \
case (func##Loc): \
{ \
	int curState = machine##State; \
	CALL_FUNC_STATE_4(func); \
	machine##SafetyCheck(safetyState); \
	if (machine##State == curState) \
		machine##StateChange(nextState); \
	break; \
}
