f = open('..\src\state.h', 'w')

'''
#define CREATE_MACHINE_(machine, sensor, state0, state1, state2, state3, state4, type1, arg1Name, type2, arg2Name) \
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
'''