from sys import argv

max = int(argv[1])

for i in range(max + 1):
    print('#define NEW_ASYNC_VOID_%d(%s) \\' % (i, ', '.join(['func', *('type%d' % j for j in range(i))])))
    if i > 0:
        print('typedef struct _asyncData_##func { \\')
        for j in range(i):
            print('  type%d arg%d; \\' % (j, j))
        print('} sAsyncData_##func; \\')
        print('sAsyncData_##func gAsyncData_##func; \\')
    print('task asyncTask_##func() { \\')
    print('  func(%s); \\' % ', '.join('gAsyncData_##func.arg%d' % j for j in range(i)))
    print('  return_t; \\')
    print('} \\')
    print('void func##Async(%s) { \\' % ', '.join('type%d arg%d' % (j, j) for j in range(i)))
    for j in range(i):
        print('  gAsyncData_##func.arg%d = arg%d; \\' % (j, j))
    print('  tStart(asyncTask_##func); \\')
    print('} \\')
    print('void func##Await(unsigned long timeout) { \\')
    print('  while (tEls[asyncTask_##func].parent != -1 && !TimedOut(timeout, "await - " #func)) sleep(10); \\')
    print('} \\')
    print('void func##Kill() { \\')
    print('  tStopAll(asyncTask_##func); \\')
    print('}')
    print()
