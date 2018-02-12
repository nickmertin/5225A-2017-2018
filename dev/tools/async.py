from sys import argv

print('#define STATE_INVOKE_ASYNC(func) _asyncInvoke_##func((sAsyncData_##func *)arg._ptr);')
print('#define RUNNING(func, unique) (_asyncUnique_##func == unique)')
print()

for i in range(int(argv[1]) + 1):
    header = ', '.join(['func', *('type%d' % j for j in range(i))])

    print('#define NEW_ASYNC_VOID_TEMPLATE_%d(%s) \\' % (i, header))
    print('typedef struct _asyncData_##func { \\')
    if i > 0:
        for j in range(i):
            print('  type%d arg%d; \\' % (j, j))
    else:
        print('  char _dummy[0]; \\')
    print('} sAsyncData_##func; \\')
    print('void _asyncInvoke_##func(sAsyncData_##func *data) { \\')
    print('  func(%s); \\' % ', '.join('data->arg%d' % j for j in range(i)))
    print('}')
    print()

    print('#define NEW_ASYNC_VOID_API_%d(before, var, after, await, %s) \\' % (i, header))
    print('unsigned long _asyncUnique_##func = 0; \\')
    print('unsigned long func##Async(%s) { \\' % ', '.join('type%d arg%d' % (j, j) for j in range(i)))
    print('  tHog(); \\')
    print('  unsigned long unique = _asyncUnique++; \\')
    print('  _asyncUnique_##func = unique; \\')
    print('  tRelease(); \\')
    print('  before \\')
    for j in range(i):
        print('  var.arg%d = arg%d; \\' % (j, j))
    print('  after \\')
    print('  return unique; \\')
    print('} \\')
    print('void func##Await(unsigned long unique, unsigned long timeout, const string description) { \\')
    print('  await \\')
    print('} \\')
    print('void func##Await(unsigned long timeout, const string description) { \\')
    print('  func##Await(_asyncUnique_##func, timeout, description); \\')
    print('}')
    print()

    print('#define NEW_ASYNC_VOID_%d(%s) \\' % (i, header))
    print('NEW_ASYNC_VOID_TEMPLATE_%d(%s) \\' % (i, header))
    print('sAsyncData_##func _asyncDataStandalone_##func; \\')
    print('task _asyncTask_##func() { \\')
    print('  _asyncInvoke_##func(&_asyncDataStandalone_##func); \\')
    print('  return_t \\')
    print('} \\')
    print('NEW_ASYNC_VOID_API_%d(; , _asyncDataStandalone_##func, tStart(_asyncTask_##func);, '
          'while (RUNNING(func, unique) && tEls[_asyncTask_##func].parent != -1 && !TimedOut(timeout, "await"))'
          ' sleep(10);, %s) \\' % (i, header))
    print('void func##Kill(bool killAll = false) { \\')
    print('  if (killAll) \\')
    print('    tStopAll(_asyncTask_##func); \\')
    print('  else \\')
    print('    tStop(_asyncTask_##func); \\')
    print('}')
    print()

    print('#define NEW_ASYNC_VOID_STATE_%d(machine, state, %s) \\' % (i, header))
    print('NEW_ASYNC_VOID_TEMPLATE_%d(%s); \\' % (i, header))
    print('sAsyncData_##func _asyncDataState_##func; \\')
    print('NEW_ASYNC_VOID_API_%d(;, _asyncDataState_##func, machine##Set(state, &_asyncDataState_##func), '
          'machine##TimeoutWhile(state, timeout, description);, %s) \\' % (i, header))
    print()

print('unsigned long _asyncUnique = 1;')
print()
