#define PUSH_STATE(machine, state, code) \
{ \
	int machine##Last = machine##State; \
	machine##Set(state); \
	{ code } \
	machine##Set(machine##Last); \
}

#define IS_CONFIGURED(machine) (machine##State != -1)
#define USE_MACHINE(machine) USE_ASYNC(machine##Internal)
#define NEXT_STATE(new_state, new_arg) { state = new_state; arg = new_arg; goto top; }

#define MAKE_MACHINE(machine, states, base, handler) \
states machine##State = -1; \
void machine##Internal(states state, long arg) \
{ \
	top: \
	writeDebugStreamLine("%d " #machine " %d -> %d, %d", nPgmTime, machine##State, state, arg); \
	switch (machine##State = state) \
	handler \
} \
NEW_ASYNC_VOID_2(machine##Internal, states, long); \
void machine##Setup() \
{ \
	if (IS_CONFIGURED(machine)) \
		machine##InternalKill(); \
	machine##InternalAsync(base, -1); \
	writeDebugStreamLine("Initialized state machine " #machine " in base state " #base); \
} \
void machine##Set(states state, long arg = -1) \
{ \
	machine##InternalKill(); \
	machine##InternalAsync(state, arg); \
} \
void machine##Reset() \
{ \
	machine##Set(base); \
}
