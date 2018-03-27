from sys import argv

maxArgc = int(argv[1])

for i in range(maxArgc + 1):
    header = ', '.join(['func', *('type%d' % j for j in range(i))])

    print('#define __ASYNC_UNIQUE_%d(%s) _asyncUnique_##func' % (i, header))
    print('#define __ASYNC_STATE_NAME_%d(%s) func##State' % (i, header))
    print('#define __ASYNC_STATE_INVOKE_%d(%s) STATE_INVOKE_ASYNC(func)' % (i, header))
    print('#define __ASYNC_HEADER_%d(%s) unsigned long func##Async(%s)'
          % (i, header, ', '.join([*['type%d arg%d' % (j, j) for j in range(i)], 'bool detached = false'])))
    print()

    print('#define __ASYNC_COPYTOVAR_%d(%s) \\' % (i, header))
    print('{ \\')
    for j in range(i):
        print('  _asyncDataVar_##func.arg%d = arg%d; \\' % (j, j))
    print('}')
    print()

print('#define ASYNC_TASK_NAME(func) _asyncTask_##func')
print('#define STATE_INVOKE_ASYNC(func) _asyncInvoke_##func();')
print('#define CUR_UNIQUE(func) _asyncUnique_##func')
print('#define RUNNING(func, unique) (CUR_UNIQUE(func) == unique)')
print('#define RUNNING_STANDALONE(func, unique) (RUNNING(func, unique) && tEls[_asyncTask_##func].parent != -1)')
print('#define RUNNING_STATE(machine, state, func, unique) (RUNNING(func, unique) && machine##State == state)')
print()

print('#define __ASYNC_API(before, after, n, argv) \\')
print('unsigned long __ASYNC_UNIQUE_##n argv = 0; \\')
print('__ASYNC_HEADER_##n argv { \\')
print('  tHog(); \\')
print('  unsigned long unique = _asyncUnique++; \\')
print('  __ASYNC_UNIQUE_##n argv = unique; \\')
print('  tRelease(); \\')
print('  before \\')
print('  __ASYNC_COPYTOVAR_##n argv \\')
print('  after \\')
print('  return unique; \\')
print('}')
print()

print('#define __ASYNC_STATE_INTERNAL(machine, state, n, argv) \\')
print('__ASYNC_TEMPLATE_##n argv \\')
print('__ASYNC_API(;, machine##Set(state, 0, detached);, n, argv) \\')
print()

for i in range(maxArgc + 1):
    header = ', '.join(['func', *('type%d' % j for j in range(i))])

    print('#define __ASYNC_TEMPLATE_%d(%s) \\' % (i, header))
    print('typedef struct _asyncData_##func { \\')
    if i > 0:
        for j in range(i):
            print('  type%d arg%d; \\' % (j, j))
    else:
        print('  char _dummy[0]; \\')
    print('} sAsyncData_##func; \\')
    print('sAsyncData_##func _asyncDataVar_##func; \\')
    print('void _asyncInvoke_##func() { \\')
    print('  func(%s); \\' % ', '.join('_asyncDataVar_##func.arg%d' % j for j in range(i)))
    print('}')
    print()

    print('#define NEW_ASYNC_VOID_%d(%s) \\' % (i, header))
    print('__ASYNC_TEMPLATE_%d(%s) \\' % (i, header))
    print('bool _asyncFlag_##func; \\')
    print('task _asyncTask_##func() { \\')
    print('  _asyncFlag_##func = true; \\')
    print('  _asyncInvoke_##func(); \\')
    print('  return_t \\')
    print('} \\')
    print('__ASYNC_API(; , tHog(); _asyncFlag_##func = false; tStart(_asyncTask_##func, detached); tRelease(); while(!_asyncFlag_##func) EndTimeSlice();, %d, (%s)) \\' % (i, header))
    print('void func##Kill(bool killAll = false) { \\')
    print('  if (killAll) \\')
    print('    tStopAll(_asyncTask_##func); \\')
    print('  else \\')
    print('    tStop(_asyncTask_##func); \\')
    print('} \\')
    print('void func##Await(unsigned long unique, unsigned long timeout, const unsigned char *routine, unsigned short id) { \\')
    print('  while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, routine, id)) sleep(10); \\')
    print('} \\')
    print('void func##Await(unsigned long timeout, const unsigned char *routine, unsigned short id) { \\')
    print('  func##Await(_asyncUnique_##func, timeout, routine, id); \\')
    print('}')
    print()

    print('#define NEW_ASYNC_VOID_STATE_%d(machine, state, %s) __ASYNC_STATE_INTERNAL(machine, state, %d, (%s))'
          % (i, header, i, header))
    print()

print('unsigned long _asyncUnique = 1;')
print()
