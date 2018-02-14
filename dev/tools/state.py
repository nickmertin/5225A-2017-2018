from sys import argv

print('#define PUSH_STATE(machine, state, code) \\')
print('{ \\')
print('  int machine##Last = machine##State; \\')
print('  machine##Set(state); \\')
print('  { code } \\')
print('  machine##Set(machine##Last); \\')
print('}')
print()

print('#define IS_CONFIGURED(machine) (machine##State != -1)')
print('#define USE_MACHINE(machine) { USE_ASYNC(machine##Internal) USE_ASYNC(machine##TimeoutWhile) USE_ASYNC(machine##TimeoutUntil) }')
print('#define NEXT_STATE(new_state) { state = new_state; goto top; }')
print()

print('typedef union _stateMachineArg {')
print('  long _long;')
print('  float _float;')
print('  void *_ptr;')
print('} uStateMachineArg;')
print()

print('#define DECLARE_MACHINE(machine, states) \\')
print('void machine##Setup(); \\')
print('void machine##Set(states state); \\')
print('void machine##Set(states state, long arg); \\')
print('void machine##Set(states state, float arg); \\')
print('void machine##Set(states state, void *arg); \\')
print('void machine##Reset(); \\')
print('void machine##TimeoutWhile(states state, unsigned long timeout, const string description); \\')
print('void machine##TimeoutUntil(states state, unsigned long timeout, const string description); \\')
print()

print('#define MAKE_MACHINE(machine, states, base, handler) \\')
print('states machine##State = -1; \\')
print('uStateMachineArg machine##Arg; \\')
print('void machine##Internal(states state) \\')
print('{ \\')
print('  uStateMachineArg &arg = machine##Arg; \\')
print('  top: \\')
print('  writeDebugStreamLine("%d " #machine " %d -> %d, %x", nPgmTime, machine##State, state, arg._long); \\')
print('  switch (machine##State = state) \\')
print('  handler \\')
print('} \\')
print('NEW_ASYNC_VOID_1(machine##Internal, states); \\')
print('void machine##Setup() \\')
print('{ \\')
print('  if (IS_CONFIGURED(machine)) \\')
print('    machine##InternalKill(); \\')
print('  machine##Arg._long = 0; \\')
print('  machine##InternalAsync(base); \\')
print('  writeDebugStreamLine("Initialized state machine " #machine " in base state " #base); \\')
print('} \\')
print('void machine##Set(states state) \\')
print('{ \\')
print('  machine##InternalKill(); \\')
print('  machine##InternalAsync(state); \\')
print('} \\')
print('void machine##Set(states state, long arg) \\')
print('{ \\')
print('  machine##InternalKill(); \\')
print('  machine##Arg._long = arg; \\')
print('  machine##InternalAsync(state); \\')
print('} \\')
print('void machine##Set(states state, float arg) \\')
print('{ \\')
print('  machine##InternalKill(); \\')
print('  machine##Arg._float = arg; \\')
print('  machine##InternalAsync(state); \\')
print('} \\')
print('void machine##Set(states state, void *arg) \\')
print('{ \\')
print('  machine##InternalKill(); \\')
print('  machine##Arg._ptr = arg; \\')
print('  machine##InternalAsync(state); \\')
print('} \\')
print('void machine##Reset() \\')
print('{ \\')
print('  machine##Set(base); \\')
print('} \\')
print('void machine##TimeoutWhile(states state, unsigned long timeout, const string description) \\')
print('{ \\')
print('  timeoutWhileEqual(&machine##State, &state, sizeof(states), timeout, description); \\')
print('} \\')
print('void machine##TimeoutUntil(states state, unsigned long timeout, const string description) \\')
print('{ \\')
print('  timeoutWhileNotEqual(&machine##State, &state, sizeof(states), timeout, description); \\')
print('}')
print()

for i in range(1, int(argv[1]) + 1):
    print('#define MAKE_ASYNC_ONLY_MACHINE_%d(machine, %s) \\' % (i, ', '.join('argc%d, argv%d, after%d' % (j, j, j) for j in range(i))))
    print('typedef enum _asyncStates_##machine { \\')
    print('  machine##NotRunning, \\')
    for j in range(i):
        print('  __ASYNC_STATE_NAME_##argc%d argv%d %s \\' % (j, j, ',' if j < i - 1 else ''))
    print('} sAsyncStates_##machine; \\')
    print('DECLARE_MACHINE(machine, sAsyncStates_##machine) \\')
    for j in range(i):
        print('__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc%d argv%d, argc%d, argv%d) \\' % (j, j, j, j))
    print('MAKE_MACHINE(machine, sAsyncStates_##machine, machine##NotRunning, { \\')
    for j in range(i):
        print('case __ASYNC_STATE_NAME_##argc%d argv%d : \\' % (j, j))
        print('  __ASYNC_STATE_INVOKE_##argc%d argv%d \\' % (j, j))
        print('  { after%d } \\' % j)
        print('  NEXT_STATE(machine##NotRunning) \\')
    print('})')
    print()
