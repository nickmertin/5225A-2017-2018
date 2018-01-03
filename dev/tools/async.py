from sys import argv

max = int(argv[1])

for i in range(max + 1):
    print('#define NEW_ASYNC_VOID_%d(%s) \\' % (i, ', '.join(['func', *('type%d' % j for j in range(i))])))
    if i > 0:
        print('typedef struct _asyncData_##func { \\')
        for j in range(i):
            print('  type%d arg%d; \\' % (j, j))
        print('} sAsyncData_##func; \\')
        print('void _asyncTask_##func(sAsyncData_##func *data) { \\')
    else:
        print('void _asyncTask_##func(void *data) { \\')
    print('  func(%s); \\' % ', '.join('data->arg%d' % j for j in range(i)))
    print('} \\')
    print('byte func##Async(%s) { \\' % ', '.join('type%d arg%d' % (j, j) for j in range(i)))
    if i > 0:
        print('sAsyncData_##func data; \\')
        for j in range(i):
            print('  data.arg%d = arg%d; \\' % (j, j))
        print('  return _startAsync(#func, &data); \\')
    else:
        print('  return _startAsync(#func, NULL); \\')
    print('}')
    print()

print('#define USE_ASYNC(func) \\')
print('if (data->name == #func) { \\')
print('  _asyncTask_##func((sAsyncData_##func *)data->data); \\')
print('  return true; \\')
print('}')
print()

print('#define ASYNC_SETUP(content) \\')
print('bool _runAsync(sAsyncTaskData *data) { \\')
print('  content \\')
print('  return false; \\')
print('}')
print()

print('typedef struct _sAsyncTaskData {')
print('  string name;')
print('  void *data;')
print('  sNotifier *notifier;')
print('} sAsyncTaskData;')
print('sAsyncTaskData gAsyncTaskData[TASK_POOL_SIZE];')
print()

print('void await(byte index);')
print('void kill(byte index);')
print('bool _runAsync(sAsyncTaskData *data);')
print('byte _startAsync(const string name, void *data);')
print()

for i in range(20):
    print('#if TASK_POOL_SIZE > %d' % i)
    print('task threadPoolTask%d() {' % i)
    print('  if (!_runAsync(&gAsyncTaskData[%d]))' % i)
    print('    writeDebugStreamLine("Failed to start %%s on threadPoolTask%d!", gAsyncTaskData[%d].name);' % (i, i))
    print('}')
    print('#endif')
    print()
