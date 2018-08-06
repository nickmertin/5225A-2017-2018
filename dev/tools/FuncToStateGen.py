f = open('..\..\src\FuncToState.h', 'w')
maxFuncNum = 7
maxParamNum = 13


f.write("/*/t Auto-Generated Macros to Add Up to %d Funcs to a Machine /t*/" %maxFuncNum + "\n")
f.write("/*/t TO BE CALLED IN HEADER /t*/" + "\n")
f.write("" + "\n")
for func in range (1, maxFuncNum+1):
	f.write("#define ADD_FUNCS_TO_MACHINE_%d(machine" %func)
	for i in range (1, func+1):
		f.write(", func%d" %i)
	f.write(") \\" + "\n")
	
	f.write("typedef enum _tFuncStates##machine \\" + "\n")
	f.write("{ \\" + "\n")
	for i in range (1, func+1):
		f.write("	machine##func%d" %i)
		if i is 1:
			f.write(" = machine##StateCount")
		if i is func:
			f.write(" \\" + "\n")
		else:
			f.write(", \\ \n")
	f.write("}tFuncStates##machine; \\" + "\n")
	
	for i in range (1, func+1):
		f.write("const int func%d##Loc = machine##StateCount + %d" %(i, (i-1)) )
		if i is func:
			f.write("\n")
		else:
			f.write("; \\ \n")
	f.write("" + "\n")

f.write("/* /t ///////////////////// /t */" + "\n")
f.write("/*/t Auto-Generated Macros to Use Funcs w/ up to %d Paramaters /t*/" %maxParamNum + "\n")
f.write("" + "\n")
for pNum in range (0, maxParamNum+1):
	f.write("/* Macros for %d Param Functions */" %pNum + "\n")
	if pNum is not 0:
		f.write("//Create global variables for all args of a func - TO BE CALLED IN HEADER" + "\n")
		f.write("#define PREP_FUNC_STATE_%d(func" %pNum) 
		for i in range (1, pNum+1):
			f.write(", type%d" %i)
		f.write(") \\" + "\n") 

		f.write("const int func##ArgCount = %d; \\" %pNum + "\n")
		for i in range(1, pNum+1):
			f.write("type%d func##Arg%d = -1" %(i, i) )
			if i is pNum:
				f.write("\n")
			else:
				f.write("; \\ \n")
		f.write("" + "\n")
		
		f.write("//Assign to all func args - TO BE CALLED RIGHT BEFORE PUTTING MACHINE INTO STATE" + "\n")
		f.write("#define ASSIGN_FUNC_STATE_%d(func" %pNum) 
		for i in range (1, pNum+1):
			f.write(", arg%dIn" %i)
		f.write(") \\" + "\n") 

		for i in range(1, pNum+1):
			f.write("func##Arg%d = arg%dIn" %(i, i) )
			if i is pNum:
				f.write("\n")
			else:
				f.write("; \\ \n")
		f.write("" + "\n")

	f.write("//Add function to the machine - TO BE CALLED IN STATE MACHINE SWITCH" + "\n")

	f.write("#define CALL_FUNC_STATE_%d(func) func(" %pNum)  
	if pNum is not 0:
		for i in range (1, pNum+1):
			f.write("func##Arg%d" %i)
			if i is not pNum:
				f.write(", ")
	f.write(")" + "\n")
	f.write("" + "\n")

	f.write("#define ADD_FUNC_TO_SWITCH_%d(func, machine, nextState, safetyState) \\" %pNum + "\n")
	f.write("case (func##Loc): \\" + "\n")
	f.write("{ \\" + "\n")
	f.write("	int curState = machine##State; \\" + "\n")
	f.write("	CALL_FUNC_STATE_%d(func); \\" %pNum + "\n")
	f.write("	machine##SafetyCheck(safetyState); \\" + "\n")
	f.write("	if (machine##State == curState) \\" + "\n")
	f.write("		machine##StateChange(nextState); \\" + "\n")
	f.write("	break; \\" + "\n")
	f.write("}" + "\n\n")

f.close