#define NEW_ASYNC_VOID_0(func) \
byte func##Dummy; \
typedef struct _asyncData_##func { \
  int _dummy[0]; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data, sNotifier *notifier) { \
  sAsyncData_##func _data; \
  memcpy(&_data, data, sizeof(sAsyncData_##func)); \
  notify(*notifier); \
  func(); \
} \
unsigned long func##Async() { \
  sAsyncData_##func data; \
  return _startAsync(&func##Dummy, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].id == &func##Dummy && tEls[threadPoolTask0 + i].parent != -1) { \
      _killAsync(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_1(func, type0) \
byte func##Dummy; \
typedef struct _asyncData_##func { \
  type0 arg0; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data, sNotifier *notifier) { \
  sAsyncData_##func _data; \
  memcpy(&_data, data, sizeof(sAsyncData_##func)); \
  notify(*notifier); \
  func(_data.arg0); \
} \
unsigned long func##Async(type0 arg0) { \
  sAsyncData_##func data; \
  data.arg0 = arg0; \
  return _startAsync(&func##Dummy, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].id == &func##Dummy && tEls[threadPoolTask0 + i].parent != -1) { \
      _killAsync(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_2(func, type0, type1) \
byte func##Dummy; \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data, sNotifier *notifier) { \
  sAsyncData_##func _data; \
  memcpy(&_data, data, sizeof(sAsyncData_##func)); \
  notify(*notifier); \
  func(_data.arg0, _data.arg1); \
} \
unsigned long func##Async(type0 arg0, type1 arg1) { \
  sAsyncData_##func data; \
  data.arg0 = arg0; \
  data.arg1 = arg1; \
  return _startAsync(&func##Dummy, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].id == &func##Dummy && tEls[threadPoolTask0 + i].parent != -1) { \
      _killAsync(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_3(func, type0, type1, type2) \
byte func##Dummy; \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data, sNotifier *notifier) { \
  sAsyncData_##func _data; \
  memcpy(&_data, data, sizeof(sAsyncData_##func)); \
  notify(*notifier); \
  func(_data.arg0, _data.arg1, _data.arg2); \
} \
unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2) { \
  sAsyncData_##func data; \
  data.arg0 = arg0; \
  data.arg1 = arg1; \
  data.arg2 = arg2; \
  return _startAsync(&func##Dummy, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].id == &func##Dummy && tEls[threadPoolTask0 + i].parent != -1) { \
      _killAsync(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_4(func, type0, type1, type2, type3) \
byte func##Dummy; \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data, sNotifier *notifier) { \
  sAsyncData_##func _data; \
  memcpy(&_data, data, sizeof(sAsyncData_##func)); \
  notify(*notifier); \
  func(_data.arg0, _data.arg1, _data.arg2, _data.arg3); \
} \
unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3) { \
  sAsyncData_##func data; \
  data.arg0 = arg0; \
  data.arg1 = arg1; \
  data.arg2 = arg2; \
  data.arg3 = arg3; \
  return _startAsync(&func##Dummy, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].id == &func##Dummy && tEls[threadPoolTask0 + i].parent != -1) { \
      _killAsync(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_5(func, type0, type1, type2, type3, type4) \
byte func##Dummy; \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
  type4 arg4; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data, sNotifier *notifier) { \
  sAsyncData_##func _data; \
  memcpy(&_data, data, sizeof(sAsyncData_##func)); \
  notify(*notifier); \
  func(_data.arg0, _data.arg1, _data.arg2, _data.arg3, _data.arg4); \
} \
unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4) { \
  sAsyncData_##func data; \
  data.arg0 = arg0; \
  data.arg1 = arg1; \
  data.arg2 = arg2; \
  data.arg3 = arg3; \
  data.arg4 = arg4; \
  return _startAsync(&func##Dummy, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].id == &func##Dummy && tEls[threadPoolTask0 + i].parent != -1) { \
      _killAsync(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_6(func, type0, type1, type2, type3, type4, type5) \
byte func##Dummy; \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
  type4 arg4; \
  type5 arg5; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data, sNotifier *notifier) { \
  sAsyncData_##func _data; \
  memcpy(&_data, data, sizeof(sAsyncData_##func)); \
  notify(*notifier); \
  func(_data.arg0, _data.arg1, _data.arg2, _data.arg3, _data.arg4, _data.arg5); \
} \
unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5) { \
  sAsyncData_##func data; \
  data.arg0 = arg0; \
  data.arg1 = arg1; \
  data.arg2 = arg2; \
  data.arg3 = arg3; \
  data.arg4 = arg4; \
  data.arg5 = arg5; \
  return _startAsync(&func##Dummy, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].id == &func##Dummy && tEls[threadPoolTask0 + i].parent != -1) { \
      _killAsync(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_7(func, type0, type1, type2, type3, type4, type5, type6) \
byte func##Dummy; \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
  type4 arg4; \
  type5 arg5; \
  type6 arg6; \
} sAsyncData_##func; \
void _asyncTask_##func(sAsyncData_##func *data, sNotifier *notifier) { \
  sAsyncData_##func _data; \
  memcpy(&_data, data, sizeof(sAsyncData_##func)); \
  notify(*notifier); \
  func(_data.arg0, _data.arg1, _data.arg2, _data.arg3, _data.arg4, _data.arg5, _data.arg6); \
} \
unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6) { \
  sAsyncData_##func data; \
  data.arg0 = arg0; \
  data.arg1 = arg1; \
  data.arg2 = arg2; \
  data.arg3 = arg3; \
  data.arg4 = arg4; \
  data.arg5 = arg5; \
  data.arg6 = arg6; \
  return _startAsync(&func##Dummy, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].id == &func##Dummy && tEls[threadPoolTask0 + i].parent != -1) { \
      _killAsync(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_8(func, type0, type1, type2, type3, type4, type5, type6, type7) \
byte func##Dummy; \
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
void _asyncTask_##func(sAsyncData_##func *data, sNotifier *notifier) { \
  sAsyncData_##func _data; \
  memcpy(&_data, data, sizeof(sAsyncData_##func)); \
  notify(*notifier); \
  func(_data.arg0, _data.arg1, _data.arg2, _data.arg3, _data.arg4, _data.arg5, _data.arg6, _data.arg7); \
} \
unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7) { \
  sAsyncData_##func data; \
  data.arg0 = arg0; \
  data.arg1 = arg1; \
  data.arg2 = arg2; \
  data.arg3 = arg3; \
  data.arg4 = arg4; \
  data.arg5 = arg5; \
  data.arg6 = arg6; \
  data.arg7 = arg7; \
  return _startAsync(&func##Dummy, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].id == &func##Dummy && tEls[threadPoolTask0 + i].parent != -1) { \
      _killAsync(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_9(func, type0, type1, type2, type3, type4, type5, type6, type7, type8) \
byte func##Dummy; \
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
void _asyncTask_##func(sAsyncData_##func *data, sNotifier *notifier) { \
  sAsyncData_##func _data; \
  memcpy(&_data, data, sizeof(sAsyncData_##func)); \
  notify(*notifier); \
  func(_data.arg0, _data.arg1, _data.arg2, _data.arg3, _data.arg4, _data.arg5, _data.arg6, _data.arg7, _data.arg8); \
} \
unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7, type8 arg8) { \
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
  return _startAsync(&func##Dummy, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].id == &func##Dummy && tEls[threadPoolTask0 + i].parent != -1) { \
      _killAsync(i); \
      return true; \
    } \
  return false; \
}

#define NEW_ASYNC_VOID_10(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9) \
byte func##Dummy; \
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
void _asyncTask_##func(sAsyncData_##func *data, sNotifier *notifier) { \
  sAsyncData_##func _data; \
  memcpy(&_data, data, sizeof(sAsyncData_##func)); \
  notify(*notifier); \
  func(_data.arg0, _data.arg1, _data.arg2, _data.arg3, _data.arg4, _data.arg5, _data.arg6, _data.arg7, _data.arg8, _data.arg9); \
} \
unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7, type8 arg8, type9 arg9) { \
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
  return _startAsync(&func##Dummy, &data); \
} \
bool func##Kill() { \
  for (int i = 0; i < TASK_POOL_SIZE; ++i) \
    if (gAsyncTaskData[i].id == &func##Dummy && tEls[threadPoolTask0 + i].parent != -1) { \
      _killAsync(i); \
      return true; \
    } \
  return false; \
}

#define USE_ASYNC(func) \
if (data->id == &func##Dummy) { \
  _asyncTask_##func((sAsyncData_##func *)data->data, &data->notifier); \
  notify(data->notifier); \
  return true; \
}

#define ASYNC_ROUTINES(content) \
bool _runAsync(sAsyncTaskData *data) { \
  content \
  return false; \
}

typedef struct _sAsyncTaskData {
  byte *id;
  void *data;
  unsigned long unique;
  sNotifier notifier;
} sAsyncTaskData;

sAsyncTaskData gAsyncTaskData[TASK_POOL_SIZE];
unsigned long _asyncUnique = 1;

void asyncInit();
void await(unsigned long unique, unsigned long timeout, const string description);
void kill(unsigned long unique);
void _awaitAsync(byte index, unsigned long timeout, const string description);
void _killAsync(byte index);
bool _runAsync(sAsyncTaskData *data);
unsigned long _startAsync(byte *id, void *data);

#if TASK_POOL_SIZE > 0
task threadPoolTask0() {
  if (!_runAsync(&gAsyncTaskData[0]))
    writeDebugStreamLine("ASYNC 0");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 1
task threadPoolTask1() {
  if (!_runAsync(&gAsyncTaskData[1]))
    writeDebugStreamLine("ASYNC 1");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 2
task threadPoolTask2() {
  if (!_runAsync(&gAsyncTaskData[2]))
    writeDebugStreamLine("ASYNC 2");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 3
task threadPoolTask3() {
  if (!_runAsync(&gAsyncTaskData[3]))
    writeDebugStreamLine("ASYNC 3");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 4
task threadPoolTask4() {
  if (!_runAsync(&gAsyncTaskData[4]))
    writeDebugStreamLine("ASYNC 4");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 5
task threadPoolTask5() {
  if (!_runAsync(&gAsyncTaskData[5]))
    writeDebugStreamLine("ASYNC 5");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 6
task threadPoolTask6() {
  if (!_runAsync(&gAsyncTaskData[6]))
    writeDebugStreamLine("ASYNC 6");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 7
task threadPoolTask7() {
  if (!_runAsync(&gAsyncTaskData[7]))
    writeDebugStreamLine("ASYNC 7");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 8
task threadPoolTask8() {
  if (!_runAsync(&gAsyncTaskData[8]))
    writeDebugStreamLine("ASYNC 8");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 9
task threadPoolTask9() {
  if (!_runAsync(&gAsyncTaskData[9]))
    writeDebugStreamLine("ASYNC 9");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 10
task threadPoolTask10() {
  if (!_runAsync(&gAsyncTaskData[10]))
    writeDebugStreamLine("ASYNC 10");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 11
task threadPoolTask11() {
  if (!_runAsync(&gAsyncTaskData[11]))
    writeDebugStreamLine("ASYNC 11");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 12
task threadPoolTask12() {
  if (!_runAsync(&gAsyncTaskData[12]))
    writeDebugStreamLine("ASYNC 12");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 13
task threadPoolTask13() {
  if (!_runAsync(&gAsyncTaskData[13]))
    writeDebugStreamLine("ASYNC 13");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 14
task threadPoolTask14() {
  if (!_runAsync(&gAsyncTaskData[14]))
    writeDebugStreamLine("ASYNC 14");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 15
task threadPoolTask15() {
  if (!_runAsync(&gAsyncTaskData[15]))
    writeDebugStreamLine("ASYNC 15");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 16
task threadPoolTask16() {
  if (!_runAsync(&gAsyncTaskData[16]))
    writeDebugStreamLine("ASYNC 16");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 17
task threadPoolTask17() {
  if (!_runAsync(&gAsyncTaskData[17]))
    writeDebugStreamLine("ASYNC 17");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 18
task threadPoolTask18() {
  if (!_runAsync(&gAsyncTaskData[18]))
    writeDebugStreamLine("ASYNC 18");
  return_t;
}
#endif

#if TASK_POOL_SIZE > 19
task threadPoolTask19() {
  if (!_runAsync(&gAsyncTaskData[19]))
    writeDebugStreamLine("ASYNC 19");
  return_t;
}
#endif

