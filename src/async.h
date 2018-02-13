#define STATE_INVOKE_ASYNC(func) _asyncInvoke_##func((sAsyncData_##func *)arg._ptr);
#define CUR_UNIQUE(func) _asyncUnique_##func
#define RUNNING(func, unique) (CUR_UNIQUE(func) == unique)
#define RUNNING_STANDALONE(func, unique) (RUNNING(func, unique) && tEls[_asyncTask_##func].parent != -1)
#define RUNNING_STATE(machine, state, func, unique) (RUNNING(func, unique) && machine##State == state)

#define NEW_ASYNC_VOID_TEMPLATE_0(func) \
typedef struct _asyncData_##func { \
  char _dummy[0]; \
} sAsyncData_##func; \
void _asyncInvoke_##func(sAsyncData_##func *data) { \
  func(); \
}

#define NEW_ASYNC_VOID_API_0(before, var, after, await, func) \
unsigned long _asyncUnique_##func = 0; \
unsigned long func##Async() { \
  tHog(); \
  unsigned long unique = _asyncUnique++; \
  _asyncUnique_##func = unique; \
  tRelease(); \
  before \
  after \
  return unique; \
} \
void func##Await(unsigned long unique, unsigned long timeout, const string description) { \
  await \
} \
void func##Await(unsigned long timeout, const string description) { \
  func##Await(_asyncUnique_##func, timeout, description); \
}

#define NEW_ASYNC_VOID_0(func) \
NEW_ASYNC_VOID_TEMPLATE_0(func) \
sAsyncData_##func _asyncDataStandalone_##func; \
task _asyncTask_##func() { \
  _asyncInvoke_##func(&_asyncDataStandalone_##func); \
  return_t \
} \
NEW_ASYNC_VOID_API_0(; , _asyncDataStandalone_##func, tStart(_asyncTask_##func);, while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, "await")) sleep(10);, func) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
}

#define NEW_ASYNC_VOID_STATE_0(machine, state, func) \
NEW_ASYNC_VOID_TEMPLATE_0(func); \
sAsyncData_##func _asyncDataState_##func; \
NEW_ASYNC_VOID_API_0(;, _asyncDataState_##func, machine##Set(state, &_asyncDataState_##func), machine##TimeoutWhile(state, timeout, description);, func) \

#define NEW_ASYNC_VOID_TEMPLATE_1(func, type0) \
typedef struct _asyncData_##func { \
  type0 arg0; \
} sAsyncData_##func; \
void _asyncInvoke_##func(sAsyncData_##func *data) { \
  func(data->arg0); \
}

#define NEW_ASYNC_VOID_API_1(before, var, after, await, func, type0) \
unsigned long _asyncUnique_##func = 0; \
unsigned long func##Async(type0 arg0) { \
  tHog(); \
  unsigned long unique = _asyncUnique++; \
  _asyncUnique_##func = unique; \
  tRelease(); \
  before \
  var.arg0 = arg0; \
  after \
  return unique; \
} \
void func##Await(unsigned long unique, unsigned long timeout, const string description) { \
  await \
} \
void func##Await(unsigned long timeout, const string description) { \
  func##Await(_asyncUnique_##func, timeout, description); \
}

#define NEW_ASYNC_VOID_1(func, type0) \
NEW_ASYNC_VOID_TEMPLATE_1(func, type0) \
sAsyncData_##func _asyncDataStandalone_##func; \
task _asyncTask_##func() { \
  _asyncInvoke_##func(&_asyncDataStandalone_##func); \
  return_t \
} \
NEW_ASYNC_VOID_API_1(; , _asyncDataStandalone_##func, tStart(_asyncTask_##func);, while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, "await")) sleep(10);, func, type0) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
}

#define NEW_ASYNC_VOID_STATE_1(machine, state, func, type0) \
NEW_ASYNC_VOID_TEMPLATE_1(func, type0); \
sAsyncData_##func _asyncDataState_##func; \
NEW_ASYNC_VOID_API_1(;, _asyncDataState_##func, machine##Set(state, &_asyncDataState_##func), machine##TimeoutWhile(state, timeout, description);, func, type0) \

#define NEW_ASYNC_VOID_TEMPLATE_2(func, type0, type1) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
} sAsyncData_##func; \
void _asyncInvoke_##func(sAsyncData_##func *data) { \
  func(data->arg0, data->arg1); \
}

#define NEW_ASYNC_VOID_API_2(before, var, after, await, func, type0, type1) \
unsigned long _asyncUnique_##func = 0; \
unsigned long func##Async(type0 arg0, type1 arg1) { \
  tHog(); \
  unsigned long unique = _asyncUnique++; \
  _asyncUnique_##func = unique; \
  tRelease(); \
  before \
  var.arg0 = arg0; \
  var.arg1 = arg1; \
  after \
  return unique; \
} \
void func##Await(unsigned long unique, unsigned long timeout, const string description) { \
  await \
} \
void func##Await(unsigned long timeout, const string description) { \
  func##Await(_asyncUnique_##func, timeout, description); \
}

#define NEW_ASYNC_VOID_2(func, type0, type1) \
NEW_ASYNC_VOID_TEMPLATE_2(func, type0, type1) \
sAsyncData_##func _asyncDataStandalone_##func; \
task _asyncTask_##func() { \
  _asyncInvoke_##func(&_asyncDataStandalone_##func); \
  return_t \
} \
NEW_ASYNC_VOID_API_2(; , _asyncDataStandalone_##func, tStart(_asyncTask_##func);, while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, "await")) sleep(10);, func, type0, type1) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
}

#define NEW_ASYNC_VOID_STATE_2(machine, state, func, type0, type1) \
NEW_ASYNC_VOID_TEMPLATE_2(func, type0, type1); \
sAsyncData_##func _asyncDataState_##func; \
NEW_ASYNC_VOID_API_2(;, _asyncDataState_##func, machine##Set(state, &_asyncDataState_##func), machine##TimeoutWhile(state, timeout, description);, func, type0, type1) \

#define NEW_ASYNC_VOID_TEMPLATE_3(func, type0, type1, type2) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
} sAsyncData_##func; \
void _asyncInvoke_##func(sAsyncData_##func *data) { \
  func(data->arg0, data->arg1, data->arg2); \
}

#define NEW_ASYNC_VOID_API_3(before, var, after, await, func, type0, type1, type2) \
unsigned long _asyncUnique_##func = 0; \
unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2) { \
  tHog(); \
  unsigned long unique = _asyncUnique++; \
  _asyncUnique_##func = unique; \
  tRelease(); \
  before \
  var.arg0 = arg0; \
  var.arg1 = arg1; \
  var.arg2 = arg2; \
  after \
  return unique; \
} \
void func##Await(unsigned long unique, unsigned long timeout, const string description) { \
  await \
} \
void func##Await(unsigned long timeout, const string description) { \
  func##Await(_asyncUnique_##func, timeout, description); \
}

#define NEW_ASYNC_VOID_3(func, type0, type1, type2) \
NEW_ASYNC_VOID_TEMPLATE_3(func, type0, type1, type2) \
sAsyncData_##func _asyncDataStandalone_##func; \
task _asyncTask_##func() { \
  _asyncInvoke_##func(&_asyncDataStandalone_##func); \
  return_t \
} \
NEW_ASYNC_VOID_API_3(; , _asyncDataStandalone_##func, tStart(_asyncTask_##func);, while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, "await")) sleep(10);, func, type0, type1, type2) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
}

#define NEW_ASYNC_VOID_STATE_3(machine, state, func, type0, type1, type2) \
NEW_ASYNC_VOID_TEMPLATE_3(func, type0, type1, type2); \
sAsyncData_##func _asyncDataState_##func; \
NEW_ASYNC_VOID_API_3(;, _asyncDataState_##func, machine##Set(state, &_asyncDataState_##func), machine##TimeoutWhile(state, timeout, description);, func, type0, type1, type2) \

#define NEW_ASYNC_VOID_TEMPLATE_4(func, type0, type1, type2, type3) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
} sAsyncData_##func; \
void _asyncInvoke_##func(sAsyncData_##func *data) { \
  func(data->arg0, data->arg1, data->arg2, data->arg3); \
}

#define NEW_ASYNC_VOID_API_4(before, var, after, await, func, type0, type1, type2, type3) \
unsigned long _asyncUnique_##func = 0; \
unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3) { \
  tHog(); \
  unsigned long unique = _asyncUnique++; \
  _asyncUnique_##func = unique; \
  tRelease(); \
  before \
  var.arg0 = arg0; \
  var.arg1 = arg1; \
  var.arg2 = arg2; \
  var.arg3 = arg3; \
  after \
  return unique; \
} \
void func##Await(unsigned long unique, unsigned long timeout, const string description) { \
  await \
} \
void func##Await(unsigned long timeout, const string description) { \
  func##Await(_asyncUnique_##func, timeout, description); \
}

#define NEW_ASYNC_VOID_4(func, type0, type1, type2, type3) \
NEW_ASYNC_VOID_TEMPLATE_4(func, type0, type1, type2, type3) \
sAsyncData_##func _asyncDataStandalone_##func; \
task _asyncTask_##func() { \
  _asyncInvoke_##func(&_asyncDataStandalone_##func); \
  return_t \
} \
NEW_ASYNC_VOID_API_4(; , _asyncDataStandalone_##func, tStart(_asyncTask_##func);, while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, "await")) sleep(10);, func, type0, type1, type2, type3) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
}

#define NEW_ASYNC_VOID_STATE_4(machine, state, func, type0, type1, type2, type3) \
NEW_ASYNC_VOID_TEMPLATE_4(func, type0, type1, type2, type3); \
sAsyncData_##func _asyncDataState_##func; \
NEW_ASYNC_VOID_API_4(;, _asyncDataState_##func, machine##Set(state, &_asyncDataState_##func), machine##TimeoutWhile(state, timeout, description);, func, type0, type1, type2, type3) \

#define NEW_ASYNC_VOID_TEMPLATE_5(func, type0, type1, type2, type3, type4) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
  type4 arg4; \
} sAsyncData_##func; \
void _asyncInvoke_##func(sAsyncData_##func *data) { \
  func(data->arg0, data->arg1, data->arg2, data->arg3, data->arg4); \
}

#define NEW_ASYNC_VOID_API_5(before, var, after, await, func, type0, type1, type2, type3, type4) \
unsigned long _asyncUnique_##func = 0; \
unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4) { \
  tHog(); \
  unsigned long unique = _asyncUnique++; \
  _asyncUnique_##func = unique; \
  tRelease(); \
  before \
  var.arg0 = arg0; \
  var.arg1 = arg1; \
  var.arg2 = arg2; \
  var.arg3 = arg3; \
  var.arg4 = arg4; \
  after \
  return unique; \
} \
void func##Await(unsigned long unique, unsigned long timeout, const string description) { \
  await \
} \
void func##Await(unsigned long timeout, const string description) { \
  func##Await(_asyncUnique_##func, timeout, description); \
}

#define NEW_ASYNC_VOID_5(func, type0, type1, type2, type3, type4) \
NEW_ASYNC_VOID_TEMPLATE_5(func, type0, type1, type2, type3, type4) \
sAsyncData_##func _asyncDataStandalone_##func; \
task _asyncTask_##func() { \
  _asyncInvoke_##func(&_asyncDataStandalone_##func); \
  return_t \
} \
NEW_ASYNC_VOID_API_5(; , _asyncDataStandalone_##func, tStart(_asyncTask_##func);, while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, "await")) sleep(10);, func, type0, type1, type2, type3, type4) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
}

#define NEW_ASYNC_VOID_STATE_5(machine, state, func, type0, type1, type2, type3, type4) \
NEW_ASYNC_VOID_TEMPLATE_5(func, type0, type1, type2, type3, type4); \
sAsyncData_##func _asyncDataState_##func; \
NEW_ASYNC_VOID_API_5(;, _asyncDataState_##func, machine##Set(state, &_asyncDataState_##func), machine##TimeoutWhile(state, timeout, description);, func, type0, type1, type2, type3, type4) \

#define NEW_ASYNC_VOID_TEMPLATE_6(func, type0, type1, type2, type3, type4, type5) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
  type4 arg4; \
  type5 arg5; \
} sAsyncData_##func; \
void _asyncInvoke_##func(sAsyncData_##func *data) { \
  func(data->arg0, data->arg1, data->arg2, data->arg3, data->arg4, data->arg5); \
}

#define NEW_ASYNC_VOID_API_6(before, var, after, await, func, type0, type1, type2, type3, type4, type5) \
unsigned long _asyncUnique_##func = 0; \
unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5) { \
  tHog(); \
  unsigned long unique = _asyncUnique++; \
  _asyncUnique_##func = unique; \
  tRelease(); \
  before \
  var.arg0 = arg0; \
  var.arg1 = arg1; \
  var.arg2 = arg2; \
  var.arg3 = arg3; \
  var.arg4 = arg4; \
  var.arg5 = arg5; \
  after \
  return unique; \
} \
void func##Await(unsigned long unique, unsigned long timeout, const string description) { \
  await \
} \
void func##Await(unsigned long timeout, const string description) { \
  func##Await(_asyncUnique_##func, timeout, description); \
}

#define NEW_ASYNC_VOID_6(func, type0, type1, type2, type3, type4, type5) \
NEW_ASYNC_VOID_TEMPLATE_6(func, type0, type1, type2, type3, type4, type5) \
sAsyncData_##func _asyncDataStandalone_##func; \
task _asyncTask_##func() { \
  _asyncInvoke_##func(&_asyncDataStandalone_##func); \
  return_t \
} \
NEW_ASYNC_VOID_API_6(; , _asyncDataStandalone_##func, tStart(_asyncTask_##func);, while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, "await")) sleep(10);, func, type0, type1, type2, type3, type4, type5) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
}

#define NEW_ASYNC_VOID_STATE_6(machine, state, func, type0, type1, type2, type3, type4, type5) \
NEW_ASYNC_VOID_TEMPLATE_6(func, type0, type1, type2, type3, type4, type5); \
sAsyncData_##func _asyncDataState_##func; \
NEW_ASYNC_VOID_API_6(;, _asyncDataState_##func, machine##Set(state, &_asyncDataState_##func), machine##TimeoutWhile(state, timeout, description);, func, type0, type1, type2, type3, type4, type5) \

#define NEW_ASYNC_VOID_TEMPLATE_7(func, type0, type1, type2, type3, type4, type5, type6) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
  type4 arg4; \
  type5 arg5; \
  type6 arg6; \
} sAsyncData_##func; \
void _asyncInvoke_##func(sAsyncData_##func *data) { \
  func(data->arg0, data->arg1, data->arg2, data->arg3, data->arg4, data->arg5, data->arg6); \
}

#define NEW_ASYNC_VOID_API_7(before, var, after, await, func, type0, type1, type2, type3, type4, type5, type6) \
unsigned long _asyncUnique_##func = 0; \
unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6) { \
  tHog(); \
  unsigned long unique = _asyncUnique++; \
  _asyncUnique_##func = unique; \
  tRelease(); \
  before \
  var.arg0 = arg0; \
  var.arg1 = arg1; \
  var.arg2 = arg2; \
  var.arg3 = arg3; \
  var.arg4 = arg4; \
  var.arg5 = arg5; \
  var.arg6 = arg6; \
  after \
  return unique; \
} \
void func##Await(unsigned long unique, unsigned long timeout, const string description) { \
  await \
} \
void func##Await(unsigned long timeout, const string description) { \
  func##Await(_asyncUnique_##func, timeout, description); \
}

#define NEW_ASYNC_VOID_7(func, type0, type1, type2, type3, type4, type5, type6) \
NEW_ASYNC_VOID_TEMPLATE_7(func, type0, type1, type2, type3, type4, type5, type6) \
sAsyncData_##func _asyncDataStandalone_##func; \
task _asyncTask_##func() { \
  _asyncInvoke_##func(&_asyncDataStandalone_##func); \
  return_t \
} \
NEW_ASYNC_VOID_API_7(; , _asyncDataStandalone_##func, tStart(_asyncTask_##func);, while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, "await")) sleep(10);, func, type0, type1, type2, type3, type4, type5, type6) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
}

#define NEW_ASYNC_VOID_STATE_7(machine, state, func, type0, type1, type2, type3, type4, type5, type6) \
NEW_ASYNC_VOID_TEMPLATE_7(func, type0, type1, type2, type3, type4, type5, type6); \
sAsyncData_##func _asyncDataState_##func; \
NEW_ASYNC_VOID_API_7(;, _asyncDataState_##func, machine##Set(state, &_asyncDataState_##func), machine##TimeoutWhile(state, timeout, description);, func, type0, type1, type2, type3, type4, type5, type6) \

#define NEW_ASYNC_VOID_TEMPLATE_8(func, type0, type1, type2, type3, type4, type5, type6, type7) \
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
void _asyncInvoke_##func(sAsyncData_##func *data) { \
  func(data->arg0, data->arg1, data->arg2, data->arg3, data->arg4, data->arg5, data->arg6, data->arg7); \
}

#define NEW_ASYNC_VOID_API_8(before, var, after, await, func, type0, type1, type2, type3, type4, type5, type6, type7) \
unsigned long _asyncUnique_##func = 0; \
unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7) { \
  tHog(); \
  unsigned long unique = _asyncUnique++; \
  _asyncUnique_##func = unique; \
  tRelease(); \
  before \
  var.arg0 = arg0; \
  var.arg1 = arg1; \
  var.arg2 = arg2; \
  var.arg3 = arg3; \
  var.arg4 = arg4; \
  var.arg5 = arg5; \
  var.arg6 = arg6; \
  var.arg7 = arg7; \
  after \
  return unique; \
} \
void func##Await(unsigned long unique, unsigned long timeout, const string description) { \
  await \
} \
void func##Await(unsigned long timeout, const string description) { \
  func##Await(_asyncUnique_##func, timeout, description); \
}

#define NEW_ASYNC_VOID_8(func, type0, type1, type2, type3, type4, type5, type6, type7) \
NEW_ASYNC_VOID_TEMPLATE_8(func, type0, type1, type2, type3, type4, type5, type6, type7) \
sAsyncData_##func _asyncDataStandalone_##func; \
task _asyncTask_##func() { \
  _asyncInvoke_##func(&_asyncDataStandalone_##func); \
  return_t \
} \
NEW_ASYNC_VOID_API_8(; , _asyncDataStandalone_##func, tStart(_asyncTask_##func);, while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, "await")) sleep(10);, func, type0, type1, type2, type3, type4, type5, type6, type7) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
}

#define NEW_ASYNC_VOID_STATE_8(machine, state, func, type0, type1, type2, type3, type4, type5, type6, type7) \
NEW_ASYNC_VOID_TEMPLATE_8(func, type0, type1, type2, type3, type4, type5, type6, type7); \
sAsyncData_##func _asyncDataState_##func; \
NEW_ASYNC_VOID_API_8(;, _asyncDataState_##func, machine##Set(state, &_asyncDataState_##func), machine##TimeoutWhile(state, timeout, description);, func, type0, type1, type2, type3, type4, type5, type6, type7) \

#define NEW_ASYNC_VOID_TEMPLATE_9(func, type0, type1, type2, type3, type4, type5, type6, type7, type8) \
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
void _asyncInvoke_##func(sAsyncData_##func *data) { \
  func(data->arg0, data->arg1, data->arg2, data->arg3, data->arg4, data->arg5, data->arg6, data->arg7, data->arg8); \
}

#define NEW_ASYNC_VOID_API_9(before, var, after, await, func, type0, type1, type2, type3, type4, type5, type6, type7, type8) \
unsigned long _asyncUnique_##func = 0; \
unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7, type8 arg8) { \
  tHog(); \
  unsigned long unique = _asyncUnique++; \
  _asyncUnique_##func = unique; \
  tRelease(); \
  before \
  var.arg0 = arg0; \
  var.arg1 = arg1; \
  var.arg2 = arg2; \
  var.arg3 = arg3; \
  var.arg4 = arg4; \
  var.arg5 = arg5; \
  var.arg6 = arg6; \
  var.arg7 = arg7; \
  var.arg8 = arg8; \
  after \
  return unique; \
} \
void func##Await(unsigned long unique, unsigned long timeout, const string description) { \
  await \
} \
void func##Await(unsigned long timeout, const string description) { \
  func##Await(_asyncUnique_##func, timeout, description); \
}

#define NEW_ASYNC_VOID_9(func, type0, type1, type2, type3, type4, type5, type6, type7, type8) \
NEW_ASYNC_VOID_TEMPLATE_9(func, type0, type1, type2, type3, type4, type5, type6, type7, type8) \
sAsyncData_##func _asyncDataStandalone_##func; \
task _asyncTask_##func() { \
  _asyncInvoke_##func(&_asyncDataStandalone_##func); \
  return_t \
} \
NEW_ASYNC_VOID_API_9(; , _asyncDataStandalone_##func, tStart(_asyncTask_##func);, while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, "await")) sleep(10);, func, type0, type1, type2, type3, type4, type5, type6, type7, type8) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
}

#define NEW_ASYNC_VOID_STATE_9(machine, state, func, type0, type1, type2, type3, type4, type5, type6, type7, type8) \
NEW_ASYNC_VOID_TEMPLATE_9(func, type0, type1, type2, type3, type4, type5, type6, type7, type8); \
sAsyncData_##func _asyncDataState_##func; \
NEW_ASYNC_VOID_API_9(;, _asyncDataState_##func, machine##Set(state, &_asyncDataState_##func), machine##TimeoutWhile(state, timeout, description);, func, type0, type1, type2, type3, type4, type5, type6, type7, type8) \

#define NEW_ASYNC_VOID_TEMPLATE_10(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9) \
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
void _asyncInvoke_##func(sAsyncData_##func *data) { \
  func(data->arg0, data->arg1, data->arg2, data->arg3, data->arg4, data->arg5, data->arg6, data->arg7, data->arg8, data->arg9); \
}

#define NEW_ASYNC_VOID_API_10(before, var, after, await, func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9) \
unsigned long _asyncUnique_##func = 0; \
unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7, type8 arg8, type9 arg9) { \
  tHog(); \
  unsigned long unique = _asyncUnique++; \
  _asyncUnique_##func = unique; \
  tRelease(); \
  before \
  var.arg0 = arg0; \
  var.arg1 = arg1; \
  var.arg2 = arg2; \
  var.arg3 = arg3; \
  var.arg4 = arg4; \
  var.arg5 = arg5; \
  var.arg6 = arg6; \
  var.arg7 = arg7; \
  var.arg8 = arg8; \
  var.arg9 = arg9; \
  after \
  return unique; \
} \
void func##Await(unsigned long unique, unsigned long timeout, const string description) { \
  await \
} \
void func##Await(unsigned long timeout, const string description) { \
  func##Await(_asyncUnique_##func, timeout, description); \
}

#define NEW_ASYNC_VOID_10(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9) \
NEW_ASYNC_VOID_TEMPLATE_10(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9) \
sAsyncData_##func _asyncDataStandalone_##func; \
task _asyncTask_##func() { \
  _asyncInvoke_##func(&_asyncDataStandalone_##func); \
  return_t \
} \
NEW_ASYNC_VOID_API_10(; , _asyncDataStandalone_##func, tStart(_asyncTask_##func);, while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, "await")) sleep(10);, func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
}

#define NEW_ASYNC_VOID_STATE_10(machine, state, func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9) \
NEW_ASYNC_VOID_TEMPLATE_10(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9); \
sAsyncData_##func _asyncDataState_##func; \
NEW_ASYNC_VOID_API_10(;, _asyncDataState_##func, machine##Set(state, &_asyncDataState_##func), machine##TimeoutWhile(state, timeout, description);, func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9) \

unsigned long _asyncUnique = 1;

