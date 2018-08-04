/*/t Auto-Generated Macros to Add X Funcs to a Machine /t*/
/*/t TO BE CALLED IN HEADER /t*/

#define ADD_FUNCS_TO_MACHINE_1(machine, func1) \
typedef enum _tFuncStates##machine \
{ \
	machine##func1 = machine##StateCount \
}tFuncStates##machine; \
const int func1##Loc = machine##StateCount + 0

#define ADD_FUNCS_TO_MACHINE_2(machine, func1, func2) \
typedef enum _tFuncStates##machine \
{ \
	machine##func1 = machine##StateCount, \ 
	machine##func2 \
}tFuncStates##machine; \
const int func1##Loc = machine##StateCount + 0; \ 
const int func2##Loc = machine##StateCount + 1

#define ADD_FUNCS_TO_MACHINE_3(machine, func1, func2, func3) \
typedef enum _tFuncStates##machine \
{ \
	machine##func1 = machine##StateCount, \ 
	machine##func2, \ 
	machine##func3 \
}tFuncStates##machine; \
const int func1##Loc = machine##StateCount + 0; \ 
const int func2##Loc = machine##StateCount + 1; \ 
const int func3##Loc = machine##StateCount + 2

#define ADD_FUNCS_TO_MACHINE_4(machine, func1, func2, func3, func4) \
typedef enum _tFuncStates##machine \
{ \
	machine##func1 = machine##StateCount, \ 
	machine##func2, \ 
	machine##func3, \ 
	machine##func4 \
}tFuncStates##machine; \
const int func1##Loc = machine##StateCount + 0; \ 
const int func2##Loc = machine##StateCount + 1; \ 
const int func3##Loc = machine##StateCount + 2; \ 
const int func4##Loc = machine##StateCount + 3

#define ADD_FUNCS_TO_MACHINE_5(machine, func1, func2, func3, func4, func5) \
typedef enum _tFuncStates##machine \
{ \
	machine##func1 = machine##StateCount, \ 
	machine##func2, \ 
	machine##func3, \ 
	machine##func4, \ 
	machine##func5 \
}tFuncStates##machine; \
const int func1##Loc = machine##StateCount + 0; \ 
const int func2##Loc = machine##StateCount + 1; \ 
const int func3##Loc = machine##StateCount + 2; \ 
const int func4##Loc = machine##StateCount + 3; \ 
const int func5##Loc = machine##StateCount + 4

/* /t ///////////////////// /t */
/*/t Auto-Generated Macros to Use Funcs w/ X Paramaters /t*/

/* Macros for 0 Param Functions */
//Add function to the machine - TO BE CALLED IN STATE MACHINE SWITCH
#define CALL_FUNC_STATE_0(func) func()

#define ADD_FUNC_TO_SWITCH_0(func, machine, nextState, safetyState) \
case (func##Loc): \
{ \
	int curState = machine##State; \
	CALL_FUNC_STATE_0(func); \
	machine##SafetyCheck(safetyState); \
	if (machine##State == curState) \
		machine##StateChange(nextState); \
	break; \
}

/* Macros for 1 Param Functions */
//Create global variables for all args of a func - TO BE CALLED IN HEADER
#define PREP_FUNC_STATE_1(func, type1) \
const int func##ArgCount = 1; \
type1 func##Arg1 = -1

//Assign to all func args - TO BE CALLED RIGHT BEFORE PUTTING MACHINE INTO STATE
#define ASSIGN_FUNC_STATE_1(func, arg1In) \
func##Arg1 = arg1In

//Add function to the machine - TO BE CALLED IN STATE MACHINE SWITCH
#define CALL_FUNC_STATE_1(func) func(func##Arg1)

#define ADD_FUNC_TO_SWITCH_1(func, machine, nextState, safetyState) \
case (func##Loc): \
{ \
	int curState = machine##State; \
	CALL_FUNC_STATE_1(func); \
	machine##SafetyCheck(safetyState); \
	if (machine##State == curState) \
		machine##StateChange(nextState); \
	break; \
}

/* Macros for 2 Param Functions */
//Create global variables for all args of a func - TO BE CALLED IN HEADER
#define PREP_FUNC_STATE_2(func, type1, type2) \
const int func##ArgCount = 2; \
type1 func##Arg1 = -1; \ 
type2 func##Arg2 = -1

//Assign to all func args - TO BE CALLED RIGHT BEFORE PUTTING MACHINE INTO STATE
#define ASSIGN_FUNC_STATE_2(func, arg1In, arg2In) \
func##Arg1 = arg1In; \ 
func##Arg2 = arg2In

//Add function to the machine - TO BE CALLED IN STATE MACHINE SWITCH
#define CALL_FUNC_STATE_2(func) func(func##Arg1, func##Arg2)

#define ADD_FUNC_TO_SWITCH_2(func, machine, nextState, safetyState) \
case (func##Loc): \
{ \
	int curState = machine##State; \
	CALL_FUNC_STATE_2(func); \
	machine##SafetyCheck(safetyState); \
	if (machine##State == curState) \
		machine##StateChange(nextState); \
	break; \
}

/* Macros for 3 Param Functions */
//Create global variables for all args of a func - TO BE CALLED IN HEADER
#define PREP_FUNC_STATE_3(func, type1, type2, type3) \
const int func##ArgCount = 3; \
type1 func##Arg1 = -1; \ 
type2 func##Arg2 = -1; \ 
type3 func##Arg3 = -1

//Assign to all func args - TO BE CALLED RIGHT BEFORE PUTTING MACHINE INTO STATE
#define ASSIGN_FUNC_STATE_3(func, arg1In, arg2In, arg3In) \
func##Arg1 = arg1In; \ 
func##Arg2 = arg2In; \ 
func##Arg3 = arg3In

//Add function to the machine - TO BE CALLED IN STATE MACHINE SWITCH
#define CALL_FUNC_STATE_3(func) func(func##Arg1, func##Arg2, func##Arg3)

#define ADD_FUNC_TO_SWITCH_3(func, machine, nextState, safetyState) \
case (func##Loc): \
{ \
	int curState = machine##State; \
	CALL_FUNC_STATE_3(func); \
	machine##SafetyCheck(safetyState); \
	if (machine##State == curState) \
		machine##StateChange(nextState); \
	break; \
}

/* Macros for 4 Param Functions */
//Create global variables for all args of a func - TO BE CALLED IN HEADER
#define PREP_FUNC_STATE_4(func, type1, type2, type3, type4) \
const int func##ArgCount = 4; \
type1 func##Arg1 = -1; \ 
type2 func##Arg2 = -1; \ 
type3 func##Arg3 = -1; \ 
type4 func##Arg4 = -1

//Assign to all func args - TO BE CALLED RIGHT BEFORE PUTTING MACHINE INTO STATE
#define ASSIGN_FUNC_STATE_4(func, arg1In, arg2In, arg3In, arg4In) \
func##Arg1 = arg1In; \ 
func##Arg2 = arg2In; \ 
func##Arg3 = arg3In; \ 
func##Arg4 = arg4In

//Add function to the machine - TO BE CALLED IN STATE MACHINE SWITCH
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

/* Macros for 5 Param Functions */
//Create global variables for all args of a func - TO BE CALLED IN HEADER
#define PREP_FUNC_STATE_5(func, type1, type2, type3, type4, type5) \
const int func##ArgCount = 5; \
type1 func##Arg1 = -1; \ 
type2 func##Arg2 = -1; \ 
type3 func##Arg3 = -1; \ 
type4 func##Arg4 = -1; \ 
type5 func##Arg5 = -1

//Assign to all func args - TO BE CALLED RIGHT BEFORE PUTTING MACHINE INTO STATE
#define ASSIGN_FUNC_STATE_5(func, arg1In, arg2In, arg3In, arg4In, arg5In) \
func##Arg1 = arg1In; \ 
func##Arg2 = arg2In; \ 
func##Arg3 = arg3In; \ 
func##Arg4 = arg4In; \ 
func##Arg5 = arg5In

//Add function to the machine - TO BE CALLED IN STATE MACHINE SWITCH
#define CALL_FUNC_STATE_5(func) func(func##Arg1, func##Arg2, func##Arg3, func##Arg4, func##Arg5)

#define ADD_FUNC_TO_SWITCH_5(func, machine, nextState, safetyState) \
case (func##Loc): \
{ \
	int curState = machine##State; \
	CALL_FUNC_STATE_5(func); \
	machine##SafetyCheck(safetyState); \
	if (machine##State == curState) \
		machine##StateChange(nextState); \
	break; \
}

/* Macros for 6 Param Functions */
//Create global variables for all args of a func - TO BE CALLED IN HEADER
#define PREP_FUNC_STATE_6(func, type1, type2, type3, type4, type5, type6) \
const int func##ArgCount = 6; \
type1 func##Arg1 = -1; \ 
type2 func##Arg2 = -1; \ 
type3 func##Arg3 = -1; \ 
type4 func##Arg4 = -1; \ 
type5 func##Arg5 = -1; \ 
type6 func##Arg6 = -1

//Assign to all func args - TO BE CALLED RIGHT BEFORE PUTTING MACHINE INTO STATE
#define ASSIGN_FUNC_STATE_6(func, arg1In, arg2In, arg3In, arg4In, arg5In, arg6In) \
func##Arg1 = arg1In; \ 
func##Arg2 = arg2In; \ 
func##Arg3 = arg3In; \ 
func##Arg4 = arg4In; \ 
func##Arg5 = arg5In; \ 
func##Arg6 = arg6In

//Add function to the machine - TO BE CALLED IN STATE MACHINE SWITCH
#define CALL_FUNC_STATE_6(func) func(func##Arg1, func##Arg2, func##Arg3, func##Arg4, func##Arg5, func##Arg6)

#define ADD_FUNC_TO_SWITCH_6(func, machine, nextState, safetyState) \
case (func##Loc): \
{ \
	int curState = machine##State; \
	CALL_FUNC_STATE_6(func); \
	machine##SafetyCheck(safetyState); \
	if (machine##State == curState) \
		machine##StateChange(nextState); \
	break; \
}

/* Macros for 7 Param Functions */
//Create global variables for all args of a func - TO BE CALLED IN HEADER
#define PREP_FUNC_STATE_7(func, type1, type2, type3, type4, type5, type6, type7) \
const int func##ArgCount = 7; \
type1 func##Arg1 = -1; \ 
type2 func##Arg2 = -1; \ 
type3 func##Arg3 = -1; \ 
type4 func##Arg4 = -1; \ 
type5 func##Arg5 = -1; \ 
type6 func##Arg6 = -1; \ 
type7 func##Arg7 = -1

//Assign to all func args - TO BE CALLED RIGHT BEFORE PUTTING MACHINE INTO STATE
#define ASSIGN_FUNC_STATE_7(func, arg1In, arg2In, arg3In, arg4In, arg5In, arg6In, arg7In) \
func##Arg1 = arg1In; \ 
func##Arg2 = arg2In; \ 
func##Arg3 = arg3In; \ 
func##Arg4 = arg4In; \ 
func##Arg5 = arg5In; \ 
func##Arg6 = arg6In; \ 
func##Arg7 = arg7In

//Add function to the machine - TO BE CALLED IN STATE MACHINE SWITCH
#define CALL_FUNC_STATE_7(func) func(func##Arg1, func##Arg2, func##Arg3, func##Arg4, func##Arg5, func##Arg6, func##Arg7)

#define ADD_FUNC_TO_SWITCH_7(func, machine, nextState, safetyState) \
case (func##Loc): \
{ \
	int curState = machine##State; \
	CALL_FUNC_STATE_7(func); \
	machine##SafetyCheck(safetyState); \
	if (machine##State == curState) \
		machine##StateChange(nextState); \
	break; \
}

/* Macros for 8 Param Functions */
//Create global variables for all args of a func - TO BE CALLED IN HEADER
#define PREP_FUNC_STATE_8(func, type1, type2, type3, type4, type5, type6, type7, type8) \
const int func##ArgCount = 8; \
type1 func##Arg1 = -1; \ 
type2 func##Arg2 = -1; \ 
type3 func##Arg3 = -1; \ 
type4 func##Arg4 = -1; \ 
type5 func##Arg5 = -1; \ 
type6 func##Arg6 = -1; \ 
type7 func##Arg7 = -1; \ 
type8 func##Arg8 = -1

//Assign to all func args - TO BE CALLED RIGHT BEFORE PUTTING MACHINE INTO STATE
#define ASSIGN_FUNC_STATE_8(func, arg1In, arg2In, arg3In, arg4In, arg5In, arg6In, arg7In, arg8In) \
func##Arg1 = arg1In; \ 
func##Arg2 = arg2In; \ 
func##Arg3 = arg3In; \ 
func##Arg4 = arg4In; \ 
func##Arg5 = arg5In; \ 
func##Arg6 = arg6In; \ 
func##Arg7 = arg7In; \ 
func##Arg8 = arg8In

//Add function to the machine - TO BE CALLED IN STATE MACHINE SWITCH
#define CALL_FUNC_STATE_8(func) func(func##Arg1, func##Arg2, func##Arg3, func##Arg4, func##Arg5, func##Arg6, func##Arg7, func##Arg8)

#define ADD_FUNC_TO_SWITCH_8(func, machine, nextState, safetyState) \
case (func##Loc): \
{ \
	int curState = machine##State; \
	CALL_FUNC_STATE_8(func); \
	machine##SafetyCheck(safetyState); \
	if (machine##State == curState) \
		machine##StateChange(nextState); \
	break; \
}

/* Macros for 9 Param Functions */
//Create global variables for all args of a func - TO BE CALLED IN HEADER
#define PREP_FUNC_STATE_9(func, type1, type2, type3, type4, type5, type6, type7, type8, type9) \
const int func##ArgCount = 9; \
type1 func##Arg1 = -1; \ 
type2 func##Arg2 = -1; \ 
type3 func##Arg3 = -1; \ 
type4 func##Arg4 = -1; \ 
type5 func##Arg5 = -1; \ 
type6 func##Arg6 = -1; \ 
type7 func##Arg7 = -1; \ 
type8 func##Arg8 = -1; \ 
type9 func##Arg9 = -1

//Assign to all func args - TO BE CALLED RIGHT BEFORE PUTTING MACHINE INTO STATE
#define ASSIGN_FUNC_STATE_9(func, arg1In, arg2In, arg3In, arg4In, arg5In, arg6In, arg7In, arg8In, arg9In) \
func##Arg1 = arg1In; \ 
func##Arg2 = arg2In; \ 
func##Arg3 = arg3In; \ 
func##Arg4 = arg4In; \ 
func##Arg5 = arg5In; \ 
func##Arg6 = arg6In; \ 
func##Arg7 = arg7In; \ 
func##Arg8 = arg8In; \ 
func##Arg9 = arg9In

//Add function to the machine - TO BE CALLED IN STATE MACHINE SWITCH
#define CALL_FUNC_STATE_9(func) func(func##Arg1, func##Arg2, func##Arg3, func##Arg4, func##Arg5, func##Arg6, func##Arg7, func##Arg8, func##Arg9)

#define ADD_FUNC_TO_SWITCH_9(func, machine, nextState, safetyState) \
case (func##Loc): \
{ \
	int curState = machine##State; \
	CALL_FUNC_STATE_9(func); \
	machine##SafetyCheck(safetyState); \
	if (machine##State == curState) \
		machine##StateChange(nextState); \
	break; \
}

/* Macros for 10 Param Functions */
//Create global variables for all args of a func - TO BE CALLED IN HEADER
#define PREP_FUNC_STATE_10(func, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10) \
const int func##ArgCount = 10; \
type1 func##Arg1 = -1; \ 
type2 func##Arg2 = -1; \ 
type3 func##Arg3 = -1; \ 
type4 func##Arg4 = -1; \ 
type5 func##Arg5 = -1; \ 
type6 func##Arg6 = -1; \ 
type7 func##Arg7 = -1; \ 
type8 func##Arg8 = -1; \ 
type9 func##Arg9 = -1; \ 
type10 func##Arg10 = -1

//Assign to all func args - TO BE CALLED RIGHT BEFORE PUTTING MACHINE INTO STATE
#define ASSIGN_FUNC_STATE_10(func, arg1In, arg2In, arg3In, arg4In, arg5In, arg6In, arg7In, arg8In, arg9In, arg10In) \
func##Arg1 = arg1In; \ 
func##Arg2 = arg2In; \ 
func##Arg3 = arg3In; \ 
func##Arg4 = arg4In; \ 
func##Arg5 = arg5In; \ 
func##Arg6 = arg6In; \ 
func##Arg7 = arg7In; \ 
func##Arg8 = arg8In; \ 
func##Arg9 = arg9In; \ 
func##Arg10 = arg10In

//Add function to the machine - TO BE CALLED IN STATE MACHINE SWITCH
#define CALL_FUNC_STATE_10(func) func(func##Arg1, func##Arg2, func##Arg3, func##Arg4, func##Arg5, func##Arg6, func##Arg7, func##Arg8, func##Arg9, func##Arg10)

#define ADD_FUNC_TO_SWITCH_10(func, machine, nextState, safetyState) \
case (func##Loc): \
{ \
	int curState = machine##State; \
	CALL_FUNC_STATE_10(func); \
	machine##SafetyCheck(safetyState); \
	if (machine##State == curState) \
		machine##StateChange(nextState); \
	break; \
}

/* Macros for 11 Param Functions */
//Create global variables for all args of a func - TO BE CALLED IN HEADER
#define PREP_FUNC_STATE_11(func, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11) \
const int func##ArgCount = 11; \
type1 func##Arg1 = -1; \ 
type2 func##Arg2 = -1; \ 
type3 func##Arg3 = -1; \ 
type4 func##Arg4 = -1; \ 
type5 func##Arg5 = -1; \ 
type6 func##Arg6 = -1; \ 
type7 func##Arg7 = -1; \ 
type8 func##Arg8 = -1; \ 
type9 func##Arg9 = -1; \ 
type10 func##Arg10 = -1; \ 
type11 func##Arg11 = -1

//Assign to all func args - TO BE CALLED RIGHT BEFORE PUTTING MACHINE INTO STATE
#define ASSIGN_FUNC_STATE_11(func, arg1In, arg2In, arg3In, arg4In, arg5In, arg6In, arg7In, arg8In, arg9In, arg10In, arg11In) \
func##Arg1 = arg1In; \ 
func##Arg2 = arg2In; \ 
func##Arg3 = arg3In; \ 
func##Arg4 = arg4In; \ 
func##Arg5 = arg5In; \ 
func##Arg6 = arg6In; \ 
func##Arg7 = arg7In; \ 
func##Arg8 = arg8In; \ 
func##Arg9 = arg9In; \ 
func##Arg10 = arg10In; \ 
func##Arg11 = arg11In

//Add function to the machine - TO BE CALLED IN STATE MACHINE SWITCH
#define CALL_FUNC_STATE_11(func) func(func##Arg1, func##Arg2, func##Arg3, func##Arg4, func##Arg5, func##Arg6, func##Arg7, func##Arg8, func##Arg9, func##Arg10, func##Arg11)

#define ADD_FUNC_TO_SWITCH_11(func, machine, nextState, safetyState) \
case (func##Loc): \
{ \
	int curState = machine##State; \
	CALL_FUNC_STATE_11(func); \
	machine##SafetyCheck(safetyState); \
	if (machine##State == curState) \
		machine##StateChange(nextState); \
	break; \
}

/* Macros for 12 Param Functions */
//Create global variables for all args of a func - TO BE CALLED IN HEADER
#define PREP_FUNC_STATE_12(func, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11, type12) \
const int func##ArgCount = 12; \
type1 func##Arg1 = -1; \ 
type2 func##Arg2 = -1; \ 
type3 func##Arg3 = -1; \ 
type4 func##Arg4 = -1; \ 
type5 func##Arg5 = -1; \ 
type6 func##Arg6 = -1; \ 
type7 func##Arg7 = -1; \ 
type8 func##Arg8 = -1; \ 
type9 func##Arg9 = -1; \ 
type10 func##Arg10 = -1; \ 
type11 func##Arg11 = -1; \ 
type12 func##Arg12 = -1

//Assign to all func args - TO BE CALLED RIGHT BEFORE PUTTING MACHINE INTO STATE
#define ASSIGN_FUNC_STATE_12(func, arg1In, arg2In, arg3In, arg4In, arg5In, arg6In, arg7In, arg8In, arg9In, arg10In, arg11In, arg12In) \
func##Arg1 = arg1In; \ 
func##Arg2 = arg2In; \ 
func##Arg3 = arg3In; \ 
func##Arg4 = arg4In; \ 
func##Arg5 = arg5In; \ 
func##Arg6 = arg6In; \ 
func##Arg7 = arg7In; \ 
func##Arg8 = arg8In; \ 
func##Arg9 = arg9In; \ 
func##Arg10 = arg10In; \ 
func##Arg11 = arg11In; \ 
func##Arg12 = arg12In

//Add function to the machine - TO BE CALLED IN STATE MACHINE SWITCH
#define CALL_FUNC_STATE_12(func) func(func##Arg1, func##Arg2, func##Arg3, func##Arg4, func##Arg5, func##Arg6, func##Arg7, func##Arg8, func##Arg9, func##Arg10, func##Arg11, func##Arg12)

#define ADD_FUNC_TO_SWITCH_12(func, machine, nextState, safetyState) \
case (func##Loc): \
{ \
	int curState = machine##State; \
	CALL_FUNC_STATE_12(func); \
	machine##SafetyCheck(safetyState); \
	if (machine##State == curState) \
		machine##StateChange(nextState); \
	break; \
}

/* Macros for 13 Param Functions */
//Create global variables for all args of a func - TO BE CALLED IN HEADER
#define PREP_FUNC_STATE_13(func, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11, type12, type13) \
const int func##ArgCount = 13; \
type1 func##Arg1 = -1; \ 
type2 func##Arg2 = -1; \ 
type3 func##Arg3 = -1; \ 
type4 func##Arg4 = -1; \ 
type5 func##Arg5 = -1; \ 
type6 func##Arg6 = -1; \ 
type7 func##Arg7 = -1; \ 
type8 func##Arg8 = -1; \ 
type9 func##Arg9 = -1; \ 
type10 func##Arg10 = -1; \ 
type11 func##Arg11 = -1; \ 
type12 func##Arg12 = -1; \ 
type13 func##Arg13 = -1

//Assign to all func args - TO BE CALLED RIGHT BEFORE PUTTING MACHINE INTO STATE
#define ASSIGN_FUNC_STATE_13(func, arg1In, arg2In, arg3In, arg4In, arg5In, arg6In, arg7In, arg8In, arg9In, arg10In, arg11In, arg12In, arg13In) \
func##Arg1 = arg1In; \ 
func##Arg2 = arg2In; \ 
func##Arg3 = arg3In; \ 
func##Arg4 = arg4In; \ 
func##Arg5 = arg5In; \ 
func##Arg6 = arg6In; \ 
func##Arg7 = arg7In; \ 
func##Arg8 = arg8In; \ 
func##Arg9 = arg9In; \ 
func##Arg10 = arg10In; \ 
func##Arg11 = arg11In; \ 
func##Arg12 = arg12In; \ 
func##Arg13 = arg13In

//Add function to the machine - TO BE CALLED IN STATE MACHINE SWITCH
#define CALL_FUNC_STATE_13(func) func(func##Arg1, func##Arg2, func##Arg3, func##Arg4, func##Arg5, func##Arg6, func##Arg7, func##Arg8, func##Arg9, func##Arg10, func##Arg11, func##Arg12, func##Arg13)

#define ADD_FUNC_TO_SWITCH_13(func, machine, nextState, safetyState) \
case (func##Loc): \
{ \
	int curState = machine##State; \
	CALL_FUNC_STATE_13(func); \
	machine##SafetyCheck(safetyState); \
	if (machine##State == curState) \
		machine##StateChange(nextState); \
	break; \
}

