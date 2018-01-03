#define NEW_ASYNC_VOID_0(func) \
typedef struct _asyncData_##func { \
  int _dummy[0]; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data) { \
  func(); \
} \
byte func##Async() { \
  sAsyncData_##func data; \
  return _startAsync(#func, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].name == #func) { \
      kill(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_1(func, type0) \
typedef struct _asyncData_##func { \
  type0 arg0; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data) { \
  func(data->arg0); \
} \
byte func##Async(type0 arg0) { \
  sAsyncData_##func data; \
  data.arg0 = arg0; \
  return _startAsync(#func, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].name == #func) { \
      kill(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_2(func, type0, type1) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data) { \
  func(data->arg0, data->arg1); \
} \
byte func##Async(type0 arg0, type1 arg1) { \
  sAsyncData_##func data; \
  data.arg0 = arg0; \
  data.arg1 = arg1; \
  return _startAsync(#func, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].name == #func) { \
      kill(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_3(func, type0, type1, type2) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data) { \
  func(data->arg0, data->arg1, data->arg2); \
} \
byte func##Async(type0 arg0, type1 arg1, type2 arg2) { \
  sAsyncData_##func data; \
  data.arg0 = arg0; \
  data.arg1 = arg1; \
  data.arg2 = arg2; \
  return _startAsync(#func, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].name == #func) { \
      kill(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_4(func, type0, type1, type2, type3) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data) { \
  func(data->arg0, data->arg1, data->arg2, data->arg3); \
} \
byte func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3) { \
  sAsyncData_##func data; \
  data.arg0 = arg0; \
  data.arg1 = arg1; \
  data.arg2 = arg2; \
  data.arg3 = arg3; \
  return _startAsync(#func, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].name == #func) { \
      kill(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_5(func, type0, type1, type2, type3, type4) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
  type4 arg4; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data) { \
  func(data->arg0, data->arg1, data->arg2, data->arg3, data->arg4); \
} \
byte func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4) { \
  sAsyncData_##func data; \
  data.arg0 = arg0; \
  data.arg1 = arg1; \
  data.arg2 = arg2; \
  data.arg3 = arg3; \
  data.arg4 = arg4; \
  return _startAsync(#func, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].name == #func) { \
      kill(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_6(func, type0, type1, type2, type3, type4, type5) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
  type4 arg4; \
  type5 arg5; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data) { \
  func(data->arg0, data->arg1, data->arg2, data->arg3, data->arg4, data->arg5); \
} \
byte func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5) { \
  sAsyncData_##func data; \
  data.arg0 = arg0; \
  data.arg1 = arg1; \
  data.arg2 = arg2; \
  data.arg3 = arg3; \
  data.arg4 = arg4; \
  data.arg5 = arg5; \
  return _startAsync(#func, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].name == #func) { \
      kill(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_7(func, type0, type1, type2, type3, type4, type5, type6) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
  type4 arg4; \
  type5 arg5; \
  type6 arg6; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data) { \
  func(data->arg0, data->arg1, data->arg2, data->arg3, data->arg4, data->arg5, data->arg6); \
} \
byte func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6) { \
  sAsyncData_##func data; \
  data.arg0 = arg0; \
  data.arg1 = arg1; \
  data.arg2 = arg2; \
  data.arg3 = arg3; \
  data.arg4 = arg4; \
  data.arg5 = arg5; \
  data.arg6 = arg6; \
  return _startAsync(#func, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].name == #func) { \
      kill(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_8(func, type0, type1, type2, type3, type4, type5, type6, type7) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
  type4 arg4; \
  type5 arg5; \
  type6 arg6; \
  type7 arg7; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data) { \
  func(data->arg0, data->arg1, data->arg2, data->arg3, data->arg4, data->arg5, data->arg6, data->arg7); \
} \
byte func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7) { \
  sAsyncData_##func data; \
  data.arg0 = arg0; \
  data.arg1 = arg1; \
  data.arg2 = arg2; \
  data.arg3 = arg3; \
  data.arg4 = arg4; \
  data.arg5 = arg5; \
  data.arg6 = arg6; \
  data.arg7 = arg7; \
  return _startAsync(#func, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].name == #func) { \
      kill(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_9(func, type0, type1, type2, type3, type4, type5, type6, type7, type8) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
  type4 arg4; \
  type5 arg5; \
  type6 arg6; \
  type7 arg7; \
  type8 arg8; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data) { \
  func(data->arg0, data->arg1, data->arg2, data->arg3, data->arg4, data->arg5, data->arg6, data->arg7, data->arg8); \
} \
byte func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7, type8 arg8) { \
  sAsyncData_##func data; \
  data.arg0 = arg0; \
  data.arg1 = arg1; \
  data.arg2 = arg2; \
  data.arg3 = arg3; \
  data.arg4 = arg4; \
  data.arg5 = arg5; \
  data.arg6 = arg6; \
  data.arg7 = arg7; \
  data.arg8 = arg8; \
  return _startAsync(#func, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].name == #func) { \
      kill(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_10(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
  type4 arg4; \
  type5 arg5; \
  type6 arg6; \
  type7 arg7; \
  type8 arg8; \
  type9 arg9; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data) { \
  func(data->arg0, data->arg1, data->arg2, data->arg3, data->arg4, data->arg5, data->arg6, data->arg7, data->arg8, data->arg9); \
} \
byte func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7, type8 arg8, type9 arg9) { \
  sAsyncData_##func data; \
  data.arg0 = arg0; \
  data.arg1 = arg1; \
  data.arg2 = arg2; \
  data.arg3 = arg3; \
  data.arg4 = arg4; \
  data.arg5 = arg5; \
  data.arg6 = arg6; \
  data.arg7 = arg7; \
  data.arg8 = arg8; \
  data.arg9 = arg9; \
  return _startAsync(#func, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].name == #func) { \
      kill(i); \
      return true; \
    } \
  return false; \
}

#define USE_ASYNC(func) \
if (data->name == #func) { \
  _asyncTask_##func((sAsyncData_##func *)data->data); \
  notify(data->notifier); \
  return true; \
}

#define ASYNC_ROUTINES(content) \
bool _runAsync(sAsyncTaskData *data) { \
  content \
  return false; \
}

typedef struct _sAsyncTaskData {
  string name;
  void *data;
  sNotifier notifier;
} sAsyncTaskData;
sAsyncTaskData gAsyncTaskData[TASK_POOL_SIZE];

void await(byte index, unsigned long timeout, const string description);
void kill(byte index);
bool _runAsync(sAsyncTaskData *data);
byte _startAsync(const string name, void *data);

#if TASK_POOL_SIZE > 0
task threadPoolTask0() {
  if (!_runAsync(&gAsyncTaskData[0]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask0!", gAsyncTaskData[0].name);
}
#endif

#if TASK_POOL_SIZE > 1
task threadPoolTask1() {
  if (!_runAsync(&gAsyncTaskData[1]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask1!", gAsyncTaskData[1].name);
}
#endif

#if TASK_POOL_SIZE > 2
task threadPoolTask2() {
  if (!_runAsync(&gAsyncTaskData[2]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask2!", gAsyncTaskData[2].name);
}
#endif

#if TASK_POOL_SIZE > 3
task threadPoolTask3() {
  if (!_runAsync(&gAsyncTaskData[3]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask3!", gAsyncTaskData[3].name);
}
#endif

#if TASK_POOL_SIZE > 4
task threadPoolTask4() {
  if (!_runAsync(&gAsyncTaskData[4]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask4!", gAsyncTaskData[4].name);
}
#endif

#if TASK_POOL_SIZE > 5
task threadPoolTask5() {
  if (!_runAsync(&gAsyncTaskData[5]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask5!", gAsyncTaskData[5].name);
}
#endif

#if TASK_POOL_SIZE > 6
task threadPoolTask6() {
  if (!_runAsync(&gAsyncTaskData[6]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask6!", gAsyncTaskData[6].name);
}
#endif

#if TASK_POOL_SIZE > 7
task threadPoolTask7() {
  if (!_runAsync(&gAsyncTaskData[7]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask7!", gAsyncTaskData[7].name);
}
#endif

#if TASK_POOL_SIZE > 8
task threadPoolTask8() {
  if (!_runAsync(&gAsyncTaskData[8]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask8!", gAsyncTaskData[8].name);
}
#endif

#if TASK_POOL_SIZE > 9
task threadPoolTask9() {
  if (!_runAsync(&gAsyncTaskData[9]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask9!", gAsyncTaskData[9].name);
}
#endif

#if TASK_POOL_SIZE > 10
task threadPoolTask10() {
  if (!_runAsync(&gAsyncTaskData[10]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask10!", gAsyncTaskData[10].name);
}
#endif

#if TASK_POOL_SIZE > 11
task threadPoolTask11() {
  if (!_runAsync(&gAsyncTaskData[11]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask11!", gAsyncTaskData[11].name);
}
#endif

#if TASK_POOL_SIZE > 12
task threadPoolTask12() {
  if (!_runAsync(&gAsyncTaskData[12]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask12!", gAsyncTaskData[12].name);
}
#endif

#if TASK_POOL_SIZE > 13
task threadPoolTask13() {
  if (!_runAsync(&gAsyncTaskData[13]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask13!", gAsyncTaskData[13].name);
}
#endif

#if TASK_POOL_SIZE > 14
task threadPoolTask14() {
  if (!_runAsync(&gAsyncTaskData[14]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask14!", gAsyncTaskData[14].name);
}
#endif

#if TASK_POOL_SIZE > 15
task threadPoolTask15() {
  if (!_runAsync(&gAsyncTaskData[15]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask15!", gAsyncTaskData[15].name);
}
#endif

#if TASK_POOL_SIZE > 16
task threadPoolTask16() {
  if (!_runAsync(&gAsyncTaskData[16]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask16!", gAsyncTaskData[16].name);
}
#endif

#if TASK_POOL_SIZE > 17
task threadPoolTask17() {
  if (!_runAsync(&gAsyncTaskData[17]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask17!", gAsyncTaskData[17].name);
}
#endif

#if TASK_POOL_SIZE > 18
task threadPoolTask18() {
  if (!_runAsync(&gAsyncTaskData[18]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask18!", gAsyncTaskData[18].name);
}
#endif

#if TASK_POOL_SIZE > 19
task threadPoolTask19() {
  if (!_runAsync(&gAsyncTaskData[19]))
    writeDebugStreamLine("Failed to start %s on threadPoolTask19!", gAsyncTaskData[19].name);
}
#endif

