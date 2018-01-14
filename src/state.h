#define PUSH_STATE(machine, state, code) \
{ \
	int machine##Last = machine##State; \
	machine##Set(state); \
	{ code } \
	machine##Set(machine##Last); \
}

#define IS_CONFIGURED(machine) (machine##State != -1)
#define USE_MACHINE(machine) USE_ASYNC(machine##Internal)
#define NEXT_STATE(new_state) { state = new_state; goto top; }

typedef union _stateMachineArg {
	long _long;
	float _float;
	void *_ptr;
} uStateMachineArg;

#define MAKE_MACHINE(machine, states, base, handler) \
states machine##State = -1; \
uStateMachineArg machine##Arg; \
void machine##Internal(states state) \
{ \
	uStateMachineArg &arg = machine##Arg; \
	top: \
	writeDebugStreamLine("%d " #machine " %d -> %d, %d", nPgmTime, machine##State, state, arg); \
	switch (machine##State = state) \
	handler \
} \
NEW_ASYNC_VOID_1(machine##Internal, states); \
void machine##Setup() \
{ \
	if (IS_CONFIGURED(machine)) \
		machine##InternalKill(); \
	machine##Arg._long = 0; \
	machine##InternalAsync(base); \
	writeDebugStreamLine("Initialized state machine " #machine " in base state " #base); \
} \
void machine##Set(states state) \
{ \
	machine##InternalKill(); \
	machine##InternalAsync(state); \
} \
void machine##Set(states state, long arg) \
{ \
	machine##InternalKill(); \
	machine##Arg._long = arg; \
	machine##InternalAsync(state); \
} \
void machine##Set(states state, float arg) \
{ \
	machine##InternalKill(); \
	machine##Arg._float = arg; \
	machine##InternalAsync(state); \
} \
void machine##Set(states state, void *arg) \
{ \
	machine##InternalKill(); \
	machine##Arg._ptr = arg; \
	machine##InternalAsync(state); \
} \
void machine##Reset() \
{ \
	machine##Set(base); \
}
