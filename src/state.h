/* Universal State Macros */
#define NOT_T_O(machineIn) ( (machineIn##Timeout <= 0)? 1 : (npgmTime < machineIn##Timeout) )

#define WHILE(machineIn) while(NOT_T_O(machineIn) && machineIn##VelSafetyCount < 10 &&

#define SAFETY_CHECK(machineIn) (NOT_T_O(machineIn) && machineIn##VelSafetyCount < 10) ) \
{ \
	machineIn##VelSafetyCheck();

#define LOG(machineIn) if(machineIn##Logs) writeDebugStreamLine

#define END_STATE(machineIn) \
machine##StateCycCount++; \
break

typedef enum _tVelDir
{
	either = -1,
	up = 0,
	down = 1
}tVelDir;
/* /////////////// State Machine Macros (For X Params) ////////////////// */
/* Create machine using:
	CREATE_MACHINE (-----)
	{
		---
	}
*/

/*	Macro for Machine w/ 3 Params	*/
#define CREATE_MACHINE_3(machine, sensor, state0, state1, state2, type1, arg1Name, type2, arg2Name) \
const int machine##StateCount = 3; \
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
unsigned long machine##StateCycCount = 0; \
bool machine##Logs = 0; \
void machine##StateChange(int stateIn, long timeout = -1, float velSafetyThresh = -1, tVelDir velDir = -1, type1 arg1In = -1, type2 arg2In = -1) \
{ \
	if (machine##State != stateIn) \
	{ \
		if (timeout <= 0) \
		{ \
			machine##Timeout = 0; \
		} \
		else \
		{ \
			unsigned long t = npgmtime; \
			machine##Timeout = ( timeout + t ); \
			writeDebugStreamLine("Add timeout %d with %d= %d, acc=%d", timeout, t, timeout+t, machine##Timeout); \
		} \
		\
		machine##VelSafetyCount = 0; \
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
void machine##VelSafetyCheck (bool useTracking = false) \
{ \
	if (machine##VelSafetyThresh != -1 && machine##VelSafetyThresh != 0) \
	{ \
		if (machine##VelSafetyDir == either || machine##VelSafetyDir == up) \
			machine##VelSafetyThresh = abs(machine##VelSafetyThresh); \
		else if (machine##VelSafetyDir == down) \
			machine##VelSafetyThresh = -1 * abs(machine##VelSafetyThresh); \
		if (!useTracking) \
		{ \
			velocityCheck(sensor); \
			if (gSensor[sensor].velGood) \
			{ \
				if (machine##VelSafetyDir == either) \
				{ \
					if ( gSensor[sensor].velocity < abs(machine##VelSafetyThresh) ) \
						machine##VelSafetyCount ++; \
				} \
				else \
				{ \
					if ( (sgn(machine##VelSafetyThresh) == 1)? (gSensor[sensor].velocity < machine##VelSafetyThresh) :  (gSensor[sensor].velocity > machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount ++; \
					} \
				} \
			} \
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
		if (machine##VelSafetyCount >= 10) \
			velSafety = true; \
		\
		if (velSafety || timedOut) \
		{ \
			writeDebugStreamLine("%d" #machine "safety: Timedout? %d at %d VelSafety? %d", npgmTime, timedout, machine##Timeout, velSafety); \
			machine##StateChange(timedOutState, machine##arg1Name, machine##arg2Name); \
		} \
}

/*	Macro for Machine w/ 4 Params	*/
#define CREATE_MACHINE_4(machine, sensor, state0, state1, state2, state3, type1, arg1Name, type2, arg2Name) \
const int machine##StateCount = 4; \
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
unsigned long machine##StateCycCount = 0; \
bool machine##Logs = 0; \
void machine##StateChange(int stateIn, long timeout = -1, float velSafetyThresh = -1, tVelDir velDir = -1, type1 arg1In = -1, type2 arg2In = -1) \
{ \
	if (machine##State != stateIn) \
	{ \
		if (timeout <= 0) \
		{ \
			machine##Timeout = 0; \
		} \
		else \
		{ \
			unsigned long t = npgmtime; \
			machine##Timeout = ( timeout + t ); \
			writeDebugStreamLine("Add timeout %d with %d= %d, acc=%d", timeout, t, timeout+t, machine##Timeout); \
		} \
		\
		machine##VelSafetyCount = 0; \
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
void machine##VelSafetyCheck (bool useTracking = false) \
{ \
	if (machine##VelSafetyThresh != -1 && machine##VelSafetyThresh != 0) \
	{ \
		if (machine##VelSafetyDir == either || machine##VelSafetyDir == up) \
			machine##VelSafetyThresh = abs(machine##VelSafetyThresh); \
		else if (machine##VelSafetyDir == down) \
			machine##VelSafetyThresh = -1 * abs(machine##VelSafetyThresh); \
		if (!useTracking) \
		{ \
			velocityCheck(sensor); \
			if (gSensor[sensor].velGood) \
			{ \
				if (machine##VelSafetyDir == either) \
				{ \
					if ( gSensor[sensor].velocity < abs(machine##VelSafetyThresh) ) \
						machine##VelSafetyCount ++; \
				} \
				else \
				{ \
					if ( (sgn(machine##VelSafetyThresh) == 1)? (gSensor[sensor].velocity < machine##VelSafetyThresh) :  (gSensor[sensor].velocity > machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount ++; \
					} \
				} \
			} \
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
		if (machine##VelSafetyCount >= 10) \
			velSafety = true; \
		\
		if (velSafety || timedOut) \
		{ \
			writeDebugStreamLine("%d" #machine "safety: Timedout? %d at %d VelSafety? %d", npgmTime, timedout, machine##Timeout, velSafety); \
			machine##StateChange(timedOutState, machine##arg1Name, machine##arg2Name); \
		} \
}

/*	Macro for Machine w/ 5 Params	*/
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
unsigned long machine##StateCycCount = 0; \
bool machine##Logs = 0; \
void machine##StateChange(int stateIn, long timeout = -1, float velSafetyThresh = -1, tVelDir velDir = -1, type1 arg1In = -1, type2 arg2In = -1) \
{ \
	if (machine##State != stateIn) \
	{ \
		if (timeout <= 0) \
		{ \
			machine##Timeout = 0; \
		} \
		else \
		{ \
			unsigned long t = npgmtime; \
			machine##Timeout = ( timeout + t ); \
			writeDebugStreamLine("Add timeout %d with %d= %d, acc=%d", timeout, t, timeout+t, machine##Timeout); \
		} \
		\
		machine##VelSafetyCount = 0; \
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
void machine##VelSafetyCheck (bool useTracking = false) \
{ \
	if (machine##VelSafetyThresh != -1 && machine##VelSafetyThresh != 0) \
	{ \
		if (machine##VelSafetyDir == either || machine##VelSafetyDir == up) \
			machine##VelSafetyThresh = abs(machine##VelSafetyThresh); \
		else if (machine##VelSafetyDir == down) \
			machine##VelSafetyThresh = -1 * abs(machine##VelSafetyThresh); \
		if (!useTracking) \
		{ \
			velocityCheck(sensor); \
			if (gSensor[sensor].velGood) \
			{ \
				if (machine##VelSafetyDir == either) \
				{ \
					if ( gSensor[sensor].velocity < abs(machine##VelSafetyThresh) ) \
						machine##VelSafetyCount ++; \
				} \
				else \
				{ \
					if ( (sgn(machine##VelSafetyThresh) == 1)? (gSensor[sensor].velocity < machine##VelSafetyThresh) :  (gSensor[sensor].velocity > machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount ++; \
					} \
				} \
			} \
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
		if (machine##VelSafetyCount >= 10) \
			velSafety = true; \
		\
		if (velSafety || timedOut) \
		{ \
			writeDebugStreamLine("%d" #machine "safety: Timedout? %d at %d VelSafety? %d", npgmTime, timedout, machine##Timeout, velSafety); \
			machine##StateChange(timedOutState, machine##arg1Name, machine##arg2Name); \
		} \
}

/*	Macro for Machine w/ 6 Params	*/
#define CREATE_MACHINE_6(machine, sensor, state0, state1, state2, state3, state4, state5, type1, arg1Name, type2, arg2Name) \
const int machine##StateCount = 6; \
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
unsigned long machine##StateCycCount = 0; \
bool machine##Logs = 0; \
void machine##StateChange(int stateIn, long timeout = -1, float velSafetyThresh = -1, tVelDir velDir = -1, type1 arg1In = -1, type2 arg2In = -1) \
{ \
	if (machine##State != stateIn) \
	{ \
		if (timeout <= 0) \
		{ \
			machine##Timeout = 0; \
		} \
		else \
		{ \
			unsigned long t = npgmtime; \
			machine##Timeout = ( timeout + t ); \
			writeDebugStreamLine("Add timeout %d with %d= %d, acc=%d", timeout, t, timeout+t, machine##Timeout); \
		} \
		\
		machine##VelSafetyCount = 0; \
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
void machine##VelSafetyCheck (bool useTracking = false) \
{ \
	if (machine##VelSafetyThresh != -1 && machine##VelSafetyThresh != 0) \
	{ \
		if (machine##VelSafetyDir == either || machine##VelSafetyDir == up) \
			machine##VelSafetyThresh = abs(machine##VelSafetyThresh); \
		else if (machine##VelSafetyDir == down) \
			machine##VelSafetyThresh = -1 * abs(machine##VelSafetyThresh); \
		if (!useTracking) \
		{ \
			velocityCheck(sensor); \
			if (gSensor[sensor].velGood) \
			{ \
				if (machine##VelSafetyDir == either) \
				{ \
					if ( gSensor[sensor].velocity < abs(machine##VelSafetyThresh) ) \
						machine##VelSafetyCount ++; \
				} \
				else \
				{ \
					if ( (sgn(machine##VelSafetyThresh) == 1)? (gSensor[sensor].velocity < machine##VelSafetyThresh) :  (gSensor[sensor].velocity > machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount ++; \
					} \
				} \
			} \
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
		if (machine##VelSafetyCount >= 10) \
			velSafety = true; \
		\
		if (velSafety || timedOut) \
		{ \
			writeDebugStreamLine("%d" #machine "safety: Timedout? %d at %d VelSafety? %d", npgmTime, timedout, machine##Timeout, velSafety); \
			machine##StateChange(timedOutState, machine##arg1Name, machine##arg2Name); \
		} \
}

/*	Macro for Machine w/ 7 Params	*/
#define CREATE_MACHINE_7(machine, sensor, state0, state1, state2, state3, state4, state5, state6, type1, arg1Name, type2, arg2Name) \
const int machine##StateCount = 7; \
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
unsigned long machine##StateCycCount = 0; \
bool machine##Logs = 0; \
void machine##StateChange(int stateIn, long timeout = -1, float velSafetyThresh = -1, tVelDir velDir = -1, type1 arg1In = -1, type2 arg2In = -1) \
{ \
	if (machine##State != stateIn) \
	{ \
		if (timeout <= 0) \
		{ \
			machine##Timeout = 0; \
		} \
		else \
		{ \
			unsigned long t = npgmtime; \
			machine##Timeout = ( timeout + t ); \
			writeDebugStreamLine("Add timeout %d with %d= %d, acc=%d", timeout, t, timeout+t, machine##Timeout); \
		} \
		\
		machine##VelSafetyCount = 0; \
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
void machine##VelSafetyCheck (bool useTracking = false) \
{ \
	if (machine##VelSafetyThresh != -1 && machine##VelSafetyThresh != 0) \
	{ \
		if (machine##VelSafetyDir == either || machine##VelSafetyDir == up) \
			machine##VelSafetyThresh = abs(machine##VelSafetyThresh); \
		else if (machine##VelSafetyDir == down) \
			machine##VelSafetyThresh = -1 * abs(machine##VelSafetyThresh); \
		if (!useTracking) \
		{ \
			velocityCheck(sensor); \
			if (gSensor[sensor].velGood) \
			{ \
				if (machine##VelSafetyDir == either) \
				{ \
					if ( gSensor[sensor].velocity < abs(machine##VelSafetyThresh) ) \
						machine##VelSafetyCount ++; \
				} \
				else \
				{ \
					if ( (sgn(machine##VelSafetyThresh) == 1)? (gSensor[sensor].velocity < machine##VelSafetyThresh) :  (gSensor[sensor].velocity > machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount ++; \
					} \
				} \
			} \
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
		if (machine##VelSafetyCount >= 10) \
			velSafety = true; \
		\
		if (velSafety || timedOut) \
		{ \
			writeDebugStreamLine("%d" #machine "safety: Timedout? %d at %d VelSafety? %d", npgmTime, timedout, machine##Timeout, velSafety); \
			machine##StateChange(timedOutState, machine##arg1Name, machine##arg2Name); \
		} \
}

/*	Macro for Machine w/ 8 Params	*/
#define CREATE_MACHINE_8(machine, sensor, state0, state1, state2, state3, state4, state5, state6, state7, type1, arg1Name, type2, arg2Name) \
const int machine##StateCount = 8; \
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
unsigned long machine##StateCycCount = 0; \
bool machine##Logs = 0; \
void machine##StateChange(int stateIn, long timeout = -1, float velSafetyThresh = -1, tVelDir velDir = -1, type1 arg1In = -1, type2 arg2In = -1) \
{ \
	if (machine##State != stateIn) \
	{ \
		if (timeout <= 0) \
		{ \
			machine##Timeout = 0; \
		} \
		else \
		{ \
			unsigned long t = npgmtime; \
			machine##Timeout = ( timeout + t ); \
			writeDebugStreamLine("Add timeout %d with %d= %d, acc=%d", timeout, t, timeout+t, machine##Timeout); \
		} \
		\
		machine##VelSafetyCount = 0; \
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
void machine##VelSafetyCheck (bool useTracking = false) \
{ \
	if (machine##VelSafetyThresh != -1 && machine##VelSafetyThresh != 0) \
	{ \
		if (machine##VelSafetyDir == either || machine##VelSafetyDir == up) \
			machine##VelSafetyThresh = abs(machine##VelSafetyThresh); \
		else if (machine##VelSafetyDir == down) \
			machine##VelSafetyThresh = -1 * abs(machine##VelSafetyThresh); \
		if (!useTracking) \
		{ \
			velocityCheck(sensor); \
			if (gSensor[sensor].velGood) \
			{ \
				if (machine##VelSafetyDir == either) \
				{ \
					if ( gSensor[sensor].velocity < abs(machine##VelSafetyThresh) ) \
						machine##VelSafetyCount ++; \
				} \
				else \
				{ \
					if ( (sgn(machine##VelSafetyThresh) == 1)? (gSensor[sensor].velocity < machine##VelSafetyThresh) :  (gSensor[sensor].velocity > machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount ++; \
					} \
				} \
			} \
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
		if (machine##VelSafetyCount >= 10) \
			velSafety = true; \
		\
		if (velSafety || timedOut) \
		{ \
			writeDebugStreamLine("%d" #machine "safety: Timedout? %d at %d VelSafety? %d", npgmTime, timedout, machine##Timeout, velSafety); \
			machine##StateChange(timedOutState, machine##arg1Name, machine##arg2Name); \
		} \
}
