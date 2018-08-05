#define ACCEL_TIME 75 \\amnt of MS that must elapse before starting to check vel safety 
 
/* Universal State Macros */ 
#define NOT_T_O(machineIn) ( (machineIn##Timeout <= 0)? 1 : (npgmTime < machineIn##Timeout) ) 
 
#define WHILE(machineIn, condition) while( NOT_T_O(machineIn) && machineIn##VelSafetyCount < 10 && (condition) ) 
 
#define LOG(machineIn) if(machineIn##Logs) writeDebugStreamLine 
 
#define VEL_CHECK_INC(machineIn, safetyType) machineIn##VelSafetyCheck(safetyType); \ 
machineIn##StateCycCount++ 
 
typedef enum _tVelDir 
{ 
	velEither = -1, 
	velUp = 0, 
	velDown = 1 
}tVelDir; 
typedef enum _tVelType 
{ 
	velSensor, 
	velLocalY, 
	velAngle 
} tVelType; 
/* /////////////// State Machine Macros (For X States) ////////////////// */ 
/* Create machine using: 
	CREATE_MACHINE (-----) 
	{ 
		--- 
	} 
*/ 

/*	Macro for Machine w/ 3 States	*/
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
		if (goodVel && curTime-machine##StateStartTime > ACCEL_TIME) \ 
		{ \ 
				if (machine##VelSafetyDir == velEither) \
				{ \
					if ( out < abs(machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount ++; \
						if(machine##Logs) writeDebugStreamLine("%d:"#machine"velSafety %f", npgmtime, out); \ 
					} \
				} \
				else \
				{ \
					if ( (sgn(machine##VelSafetyThresh) == 1)? (out < machine##VelSafetyThresh) :  (out > machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount ++; \
						if(machine##Logs) writeDebugStreamLine("%d:"#machine"velSafety %f", npgmtime, out); \ 
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

/*	Macro for Machine w/ 4 States	*/
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
		if (goodVel && curTime-machine##StateStartTime > ACCEL_TIME) \ 
		{ \ 
				if (machine##VelSafetyDir == velEither) \
				{ \
					if ( out < abs(machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount ++; \
						if(machine##Logs) writeDebugStreamLine("%d:"#machine"velSafety %f", npgmtime, out); \ 
					} \
				} \
				else \
				{ \
					if ( (sgn(machine##VelSafetyThresh) == 1)? (out < machine##VelSafetyThresh) :  (out > machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount ++; \
						if(machine##Logs) writeDebugStreamLine("%d:"#machine"velSafety %f", npgmtime, out); \ 
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

/*	Macro for Machine w/ 5 States	*/
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
		if (goodVel && curTime-machine##StateStartTime > ACCEL_TIME) \ 
		{ \ 
				if (machine##VelSafetyDir == velEither) \
				{ \
					if ( out < abs(machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount ++; \
						if(machine##Logs) writeDebugStreamLine("%d:"#machine"velSafety %f", npgmtime, out); \ 
					} \
				} \
				else \
				{ \
					if ( (sgn(machine##VelSafetyThresh) == 1)? (out < machine##VelSafetyThresh) :  (out > machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount ++; \
						if(machine##Logs) writeDebugStreamLine("%d:"#machine"velSafety %f", npgmtime, out); \ 
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

/*	Macro for Machine w/ 6 States	*/
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
		if (goodVel && curTime-machine##StateStartTime > ACCEL_TIME) \ 
		{ \ 
				if (machine##VelSafetyDir == velEither) \
				{ \
					if ( out < abs(machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount ++; \
						if(machine##Logs) writeDebugStreamLine("%d:"#machine"velSafety %f", npgmtime, out); \ 
					} \
				} \
				else \
				{ \
					if ( (sgn(machine##VelSafetyThresh) == 1)? (out < machine##VelSafetyThresh) :  (out > machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount ++; \
						if(machine##Logs) writeDebugStreamLine("%d:"#machine"velSafety %f", npgmtime, out); \ 
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

/*	Macro for Machine w/ 7 States	*/
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
		if (goodVel && curTime-machine##StateStartTime > ACCEL_TIME) \ 
		{ \ 
				if (machine##VelSafetyDir == velEither) \
				{ \
					if ( out < abs(machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount ++; \
						if(machine##Logs) writeDebugStreamLine("%d:"#machine"velSafety %f", npgmtime, out); \ 
					} \
				} \
				else \
				{ \
					if ( (sgn(machine##VelSafetyThresh) == 1)? (out < machine##VelSafetyThresh) :  (out > machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount ++; \
						if(machine##Logs) writeDebugStreamLine("%d:"#machine"velSafety %f", npgmtime, out); \ 
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

/*	Macro for Machine w/ 8 States	*/
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
		if (goodVel && curTime-machine##StateStartTime > ACCEL_TIME) \ 
		{ \ 
				if (machine##VelSafetyDir == velEither) \
				{ \
					if ( out < abs(machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount ++; \
						if(machine##Logs) writeDebugStreamLine("%d:"#machine"velSafety %f", npgmtime, out); \ 
					} \
				} \
				else \
				{ \
					if ( (sgn(machine##VelSafetyThresh) == 1)? (out < machine##VelSafetyThresh) :  (out > machine##VelSafetyThresh) ) \
					{ \
						machine##VelSafetyCount ++; \
						if(machine##Logs) writeDebugStreamLine("%d:"#machine"velSafety %f", npgmtime, out); \ 
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
