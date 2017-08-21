#define HANDLE_STATE_REQUEST(machine, code, end) \
if (!machine##Data.stateDisabled && machine##Data.goToState == requestActive) \
{ \
	if (machine##Data.nextState == machine##Data.state) \
	{ \
		machine##Data.goToState = requestIdle; \
	} \
	else \
	{ \
		code machine##StateChangeHandler(); end \
	} \
}

#define STATE_END_GO(machine, toState) { if (!machine##Data.stateDisabled) { hogCPU(); if (machine##Data.goToState != requestActive) { machine##Data.nextState = toState; machine##Data.goToState = requestActive; } releaseCPU(); machine##StateChangeHandler(); } }
#define STATE_END(machine) STATE_END_GO(machine, machine##Data.baseState)
#define CHANGE_STATE(machine, toState) { if (!machine##Data.stateDisabled) { hogCPU(); if (machine##Data.goToState != requestActive) { machine##Data.nextState = toState; machine##Data.goToState = requestActive; } releaseCPU(); } }
#define CHANGE_STATE_FROM_STATE(machine, fromState, toState) { if (machine##Data.state == fromState) CHANGE_STATE(machine, toState); }
#define CHANGE_STATE_WAIT(machine, toState) { if (!machine##Data.stateDisabled) { while (true) { hogCPU(); if (machine##Data.goToState != requestActive) { machine##Data.nextState = toState; machine##Data.goToState = requestActive; releaseCPU(); break; } releaseCPU(); EndTimeSlice(); } } }
#define DISABLE(machine) machine##Data.stateDisabled++;
#define DISABLE_TO_STATE(machine, toState) { if (!machine##Data.stateDisabled) { if (machine##Data.state != toState) { CHANGE_STATE_WAIT(machine, toState) while (machine##Data.state != toState) EndTimeSlice(); } } DISABLE(machine) }
#define ENABLE(machine) { if (machine##Data.stateDisabled) machine##Data.stateDisabled--; if (!machine##Data.stateDisabled) STATE_END(machine) }
#define ENABLE_TO_STATE(machine, toState) { if (machine##Data.stateDisabled > 0 ) machine##Data.stateDisabled--; STATE_END_GO(machine, toState) }
#define ENABLE_FORCE(machine) { machine##Data.stateDisabled = 0; STATE_END(machine) }
#define ENABLE_FORCE_TO_STATE(machine, toState) { machine##Data.stateDisabled = 0; STATE_END_GO(machine, toState) }
#define IS_ENABLED(machine) !machine##Data.stateDisabled

#define MAKE_MACHINE(machine, states, base, handler) \
typedef struct machine##Data_S \
{ \
	states state; \
	states nextState; \
	states baseState; \
	tRequest goToState; \
	int stateDisabled; \
} machine##DataS; \
machine##DataS machine##Data; \
void machine##StateChangeHandlerInternal() \
{ \
	switch (machine##Data.state) \
	handler \
	machine##Data.goToState = requestIdle; \
} \
void machine##Setup(int disabled = 0) \
{ \
	machine##Data.goToState = requestIdle; \
	machine##Data.state = base; \
	machine##Data.baseState = base; \
	machine##Data.stateDisabled = disabled; \
	machine##StateChangeHandlerInternal(); \
} \
void machine##StateChangeHandler() \
{ \
	S_LOG #machine " %d -> %d", machine##Data.state, machine##Data.nextState E_LOG_DATA \
	machine##Data.state = machine##Data.nextState; \
	machine##Data.goToState = requestConfirmed; \
	machine##StateChangeHandlerInternal(); \
	machine##Data.goToState = requestIdle; \
}
