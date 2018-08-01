/* /////////////// State Machine Macros ////////////////// */
/* Create machine using:
	CREATE_MACHINE (-----)
	{
		---
	}
*/
#define CREATE_MACHINE(machine, state0, state1, state2, state3, state4, type1, arg1Name, type2, arg2Name) \
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
unsigned long machine##Timeout; \
type1 machine##arg1Name; \
type2 machine##arg2Name; \
void machine##StateChange(tStates##machine stateIn, unsigned long timeout = -1, type1 arg1In = -1, type2 arg2In = -1) \
{ \
	if (machine##State != stateIn) \
	{ \
		machine##Timeout = npgmTime + timeout; \
		machine##State = stateIn; \
		machine##arg1Name = arg1In; \
		machine##arg2Name = arg2In;  \
		writeDebugStreamLine ("%d" #machine "State:%d, %d, %d", npgmTime, machine##State, machine##arg1Name, machine##arg2Name); \
	} \
} \
void machine##TOCheck(tStates##machine timedOutState = machine##state0, type1 machine##arg1Name = -1, type2 machine##arg2Name = -1) \
{ \
		if (!NOT_T_O(machine)) \
			machine##StateChange(timedOutState, machine##arg1Name, machine##arg2Name); \
} \
task machine##Set () \

#define NOT_T_O(machineIn) (machineIn##Timeout == -1)? 1 : (npgmTime < machineIn##Timeout)

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

//Add other funcs to a machine starting at __ val
#define ADD_FUNCS_TO_MACHINE(machine, start, func0, func1, func2, func3, func4, func5) \
typedef enum _tFuncStates##machine \
{ \
	machine##func0 = start, \
	machine##func1, \
	machine##func2, \
	machine##func3, \
	machine##func4, \
	machine##func5 \
}tFuncStates##machine;

//Create global variables for all args of a func
#define PREP_FUNC_STATE(func, type1, arg1, type2, arg2, type3, arg3, type4, arg4) \
type1 func##arg1; \
type1 func##arg2; \
type1 func##arg3; \
type1 func##arg4;

//Assign to all func args
#define ASSIGN_FUNC_STATE(func, arg1, arg2, arg3, arg4) \
func##arg1 = arg1; \
func##arg2 = arg2; \
func##arg3 = arg3; \
func##arg4 = arg4;
