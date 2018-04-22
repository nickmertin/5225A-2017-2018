#define __ASYNC_UNIQUE_0(func) _asyncUnique_##func
#define __ASYNC_STATE_NAME_0(func) func##State
#define __ASYNC_STATE_INVOKE_0(func) STATE_INVOKE_ASYNC(func)
#define __ASYNC_HEADER_0(func) unsigned long func##Async(bool detached = false)

#define __ASYNC_COPYTOVAR_0(func) \
{ \
}

#define __ASYNC_UNIQUE_1(func, type0) _asyncUnique_##func
#define __ASYNC_STATE_NAME_1(func, type0) func##State
#define __ASYNC_STATE_INVOKE_1(func, type0) STATE_INVOKE_ASYNC(func)
#define __ASYNC_HEADER_1(func, type0) unsigned long func##Async(type0 arg0, bool detached = false)

#define __ASYNC_COPYTOVAR_1(func, type0) \
{ \
  _asyncDataVar_##func.arg0 = arg0; \
}

#define __ASYNC_UNIQUE_2(func, type0, type1) _asyncUnique_##func
#define __ASYNC_STATE_NAME_2(func, type0, type1) func##State
#define __ASYNC_STATE_INVOKE_2(func, type0, type1) STATE_INVOKE_ASYNC(func)
#define __ASYNC_HEADER_2(func, type0, type1) unsigned long func##Async(type0 arg0, type1 arg1, bool detached = false)

#define __ASYNC_COPYTOVAR_2(func, type0, type1) \
{ \
  _asyncDataVar_##func.arg0 = arg0; \
  _asyncDataVar_##func.arg1 = arg1; \
}

#define __ASYNC_UNIQUE_3(func, type0, type1, type2) _asyncUnique_##func
#define __ASYNC_STATE_NAME_3(func, type0, type1, type2) func##State
#define __ASYNC_STATE_INVOKE_3(func, type0, type1, type2) STATE_INVOKE_ASYNC(func)
#define __ASYNC_HEADER_3(func, type0, type1, type2) unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, bool detached = false)

#define __ASYNC_COPYTOVAR_3(func, type0, type1, type2) \
{ \
  _asyncDataVar_##func.arg0 = arg0; \
  _asyncDataVar_##func.arg1 = arg1; \
  _asyncDataVar_##func.arg2 = arg2; \
}

#define __ASYNC_UNIQUE_4(func, type0, type1, type2, type3) _asyncUnique_##func
#define __ASYNC_STATE_NAME_4(func, type0, type1, type2, type3) func##State
#define __ASYNC_STATE_INVOKE_4(func, type0, type1, type2, type3) STATE_INVOKE_ASYNC(func)
#define __ASYNC_HEADER_4(func, type0, type1, type2, type3) unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, bool detached = false)

#define __ASYNC_COPYTOVAR_4(func, type0, type1, type2, type3) \
{ \
  _asyncDataVar_##func.arg0 = arg0; \
  _asyncDataVar_##func.arg1 = arg1; \
  _asyncDataVar_##func.arg2 = arg2; \
  _asyncDataVar_##func.arg3 = arg3; \
}

#define __ASYNC_UNIQUE_5(func, type0, type1, type2, type3, type4) _asyncUnique_##func
#define __ASYNC_STATE_NAME_5(func, type0, type1, type2, type3, type4) func##State
#define __ASYNC_STATE_INVOKE_5(func, type0, type1, type2, type3, type4) STATE_INVOKE_ASYNC(func)
#define __ASYNC_HEADER_5(func, type0, type1, type2, type3, type4) unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, bool detached = false)

#define __ASYNC_COPYTOVAR_5(func, type0, type1, type2, type3, type4) \
{ \
  _asyncDataVar_##func.arg0 = arg0; \
  _asyncDataVar_##func.arg1 = arg1; \
  _asyncDataVar_##func.arg2 = arg2; \
  _asyncDataVar_##func.arg3 = arg3; \
  _asyncDataVar_##func.arg4 = arg4; \
}

#define __ASYNC_UNIQUE_6(func, type0, type1, type2, type3, type4, type5) _asyncUnique_##func
#define __ASYNC_STATE_NAME_6(func, type0, type1, type2, type3, type4, type5) func##State
#define __ASYNC_STATE_INVOKE_6(func, type0, type1, type2, type3, type4, type5) STATE_INVOKE_ASYNC(func)
#define __ASYNC_HEADER_6(func, type0, type1, type2, type3, type4, type5) unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, bool detached = false)

#define __ASYNC_COPYTOVAR_6(func, type0, type1, type2, type3, type4, type5) \
{ \
  _asyncDataVar_##func.arg0 = arg0; \
  _asyncDataVar_##func.arg1 = arg1; \
  _asyncDataVar_##func.arg2 = arg2; \
  _asyncDataVar_##func.arg3 = arg3; \
  _asyncDataVar_##func.arg4 = arg4; \
  _asyncDataVar_##func.arg5 = arg5; \
}

#define __ASYNC_UNIQUE_7(func, type0, type1, type2, type3, type4, type5, type6) _asyncUnique_##func
#define __ASYNC_STATE_NAME_7(func, type0, type1, type2, type3, type4, type5, type6) func##State
#define __ASYNC_STATE_INVOKE_7(func, type0, type1, type2, type3, type4, type5, type6) STATE_INVOKE_ASYNC(func)
#define __ASYNC_HEADER_7(func, type0, type1, type2, type3, type4, type5, type6) unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, bool detached = false)

#define __ASYNC_COPYTOVAR_7(func, type0, type1, type2, type3, type4, type5, type6) \
{ \
  _asyncDataVar_##func.arg0 = arg0; \
  _asyncDataVar_##func.arg1 = arg1; \
  _asyncDataVar_##func.arg2 = arg2; \
  _asyncDataVar_##func.arg3 = arg3; \
  _asyncDataVar_##func.arg4 = arg4; \
  _asyncDataVar_##func.arg5 = arg5; \
  _asyncDataVar_##func.arg6 = arg6; \
}

#define __ASYNC_UNIQUE_8(func, type0, type1, type2, type3, type4, type5, type6, type7) _asyncUnique_##func
#define __ASYNC_STATE_NAME_8(func, type0, type1, type2, type3, type4, type5, type6, type7) func##State
#define __ASYNC_STATE_INVOKE_8(func, type0, type1, type2, type3, type4, type5, type6, type7) STATE_INVOKE_ASYNC(func)
#define __ASYNC_HEADER_8(func, type0, type1, type2, type3, type4, type5, type6, type7) unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7, bool detached = false)

#define __ASYNC_COPYTOVAR_8(func, type0, type1, type2, type3, type4, type5, type6, type7) \
{ \
  _asyncDataVar_##func.arg0 = arg0; \
  _asyncDataVar_##func.arg1 = arg1; \
  _asyncDataVar_##func.arg2 = arg2; \
  _asyncDataVar_##func.arg3 = arg3; \
  _asyncDataVar_##func.arg4 = arg4; \
  _asyncDataVar_##func.arg5 = arg5; \
  _asyncDataVar_##func.arg6 = arg6; \
  _asyncDataVar_##func.arg7 = arg7; \
}

#define __ASYNC_UNIQUE_9(func, type0, type1, type2, type3, type4, type5, type6, type7, type8) _asyncUnique_##func
#define __ASYNC_STATE_NAME_9(func, type0, type1, type2, type3, type4, type5, type6, type7, type8) func##State
#define __ASYNC_STATE_INVOKE_9(func, type0, type1, type2, type3, type4, type5, type6, type7, type8) STATE_INVOKE_ASYNC(func)
#define __ASYNC_HEADER_9(func, type0, type1, type2, type3, type4, type5, type6, type7, type8) unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7, type8 arg8, bool detached = false)

#define __ASYNC_COPYTOVAR_9(func, type0, type1, type2, type3, type4, type5, type6, type7, type8) \
{ \
  _asyncDataVar_##func.arg0 = arg0; \
  _asyncDataVar_##func.arg1 = arg1; \
  _asyncDataVar_##func.arg2 = arg2; \
  _asyncDataVar_##func.arg3 = arg3; \
  _asyncDataVar_##func.arg4 = arg4; \
  _asyncDataVar_##func.arg5 = arg5; \
  _asyncDataVar_##func.arg6 = arg6; \
  _asyncDataVar_##func.arg7 = arg7; \
  _asyncDataVar_##func.arg8 = arg8; \
}

#define __ASYNC_UNIQUE_10(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9) _asyncUnique_##func
#define __ASYNC_STATE_NAME_10(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9) func##State
#define __ASYNC_STATE_INVOKE_10(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9) STATE_INVOKE_ASYNC(func)
#define __ASYNC_HEADER_10(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9) unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7, type8 arg8, type9 arg9, bool detached = false)

#define __ASYNC_COPYTOVAR_10(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9) \
{ \
  _asyncDataVar_##func.arg0 = arg0; \
  _asyncDataVar_##func.arg1 = arg1; \
  _asyncDataVar_##func.arg2 = arg2; \
  _asyncDataVar_##func.arg3 = arg3; \
  _asyncDataVar_##func.arg4 = arg4; \
  _asyncDataVar_##func.arg5 = arg5; \
  _asyncDataVar_##func.arg6 = arg6; \
  _asyncDataVar_##func.arg7 = arg7; \
  _asyncDataVar_##func.arg8 = arg8; \
  _asyncDataVar_##func.arg9 = arg9; \
}

#define __ASYNC_UNIQUE_11(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10) _asyncUnique_##func
#define __ASYNC_STATE_NAME_11(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10) func##State
#define __ASYNC_STATE_INVOKE_11(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10) STATE_INVOKE_ASYNC(func)
#define __ASYNC_HEADER_11(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10) unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7, type8 arg8, type9 arg9, type10 arg10, bool detached = false)

#define __ASYNC_COPYTOVAR_11(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10) \
{ \
  _asyncDataVar_##func.arg0 = arg0; \
  _asyncDataVar_##func.arg1 = arg1; \
  _asyncDataVar_##func.arg2 = arg2; \
  _asyncDataVar_##func.arg3 = arg3; \
  _asyncDataVar_##func.arg4 = arg4; \
  _asyncDataVar_##func.arg5 = arg5; \
  _asyncDataVar_##func.arg6 = arg6; \
  _asyncDataVar_##func.arg7 = arg7; \
  _asyncDataVar_##func.arg8 = arg8; \
  _asyncDataVar_##func.arg9 = arg9; \
  _asyncDataVar_##func.arg10 = arg10; \
}

#define __ASYNC_UNIQUE_12(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11) _asyncUnique_##func
#define __ASYNC_STATE_NAME_12(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11) func##State
#define __ASYNC_STATE_INVOKE_12(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11) STATE_INVOKE_ASYNC(func)
#define __ASYNC_HEADER_12(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11) unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7, type8 arg8, type9 arg9, type10 arg10, type11 arg11, bool detached = false)

#define __ASYNC_COPYTOVAR_12(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11) \
{ \
  _asyncDataVar_##func.arg0 = arg0; \
  _asyncDataVar_##func.arg1 = arg1; \
  _asyncDataVar_##func.arg2 = arg2; \
  _asyncDataVar_##func.arg3 = arg3; \
  _asyncDataVar_##func.arg4 = arg4; \
  _asyncDataVar_##func.arg5 = arg5; \
  _asyncDataVar_##func.arg6 = arg6; \
  _asyncDataVar_##func.arg7 = arg7; \
  _asyncDataVar_##func.arg8 = arg8; \
  _asyncDataVar_##func.arg9 = arg9; \
  _asyncDataVar_##func.arg10 = arg10; \
  _asyncDataVar_##func.arg11 = arg11; \
}

#define __ASYNC_UNIQUE_13(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11, type12) _asyncUnique_##func
#define __ASYNC_STATE_NAME_13(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11, type12) func##State
#define __ASYNC_STATE_INVOKE_13(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11, type12) STATE_INVOKE_ASYNC(func)
#define __ASYNC_HEADER_13(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11, type12) unsigned long func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7, type8 arg8, type9 arg9, type10 arg10, type11 arg11, type12 arg12, bool detached = false)

#define __ASYNC_COPYTOVAR_13(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11, type12) \
{ \
  _asyncDataVar_##func.arg0 = arg0; \
  _asyncDataVar_##func.arg1 = arg1; \
  _asyncDataVar_##func.arg2 = arg2; \
  _asyncDataVar_##func.arg3 = arg3; \
  _asyncDataVar_##func.arg4 = arg4; \
  _asyncDataVar_##func.arg5 = arg5; \
  _asyncDataVar_##func.arg6 = arg6; \
  _asyncDataVar_##func.arg7 = arg7; \
  _asyncDataVar_##func.arg8 = arg8; \
  _asyncDataVar_##func.arg9 = arg9; \
  _asyncDataVar_##func.arg10 = arg10; \
  _asyncDataVar_##func.arg11 = arg11; \
  _asyncDataVar_##func.arg12 = arg12; \
}

#define ASYNC_TASK_NAME(func) _asyncTask_##func
#define STATE_INVOKE_ASYNC(func) _asyncInvoke_##func();
#define CUR_UNIQUE(func) _asyncUnique_##func
#define RUNNING(func, unique) (CUR_UNIQUE(func) == unique)
#define RUNNING_STANDALONE(func, unique) (RUNNING(func, unique) && tEls[_asyncTask_##func].parent != -1)
#define RUNNING_STATE(machine, state, func, unique) (RUNNING(func, unique) && machine##State == state)

#define __ASYNC_API(before, after, n, argv) \
unsigned long __ASYNC_UNIQUE_##n argv = 0; \
__ASYNC_HEADER_##n argv { \
  tHog(); \
  unsigned long unique = _asyncUnique++; \
  __ASYNC_UNIQUE_##n argv = unique; \
  tRelease(); \
  before \
  __ASYNC_COPYTOVAR_##n argv \
  after \
  return unique; \
}

#define __ASYNC_STATE_INTERNAL(machine, state, n, argv) \
__ASYNC_TEMPLATE_##n argv \
__ASYNC_API(;, machine##Set(state, 0, detached);, n, argv) \

#define __ASYNC_TEMPLATE_0(func) \
typedef struct _asyncData_##func { \
  char _dummy[0]; \
} sAsyncData_##func; \
sAsyncData_##func _asyncDataVar_##func; \
void _asyncInvoke_##func() { \
  func(); \
}

#define NEW_ASYNC_VOID_0(func) \
__ASYNC_TEMPLATE_0(func) \
bool _asyncFlag_##func; \
task _asyncTask_##func() { \
  _asyncFlag_##func = true; \
  _asyncInvoke_##func(); \
  return_t \
} \
__ASYNC_API(; , tHog(); _asyncFlag_##func = false; tStart(_asyncTask_##func, detached); tRelease(); while(!_asyncFlag_##func) EndTimeSlice();, 0, (func)) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
} \
void func##Await(unsigned long unique, unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, routine, id)) sleep(10); \
} \
void func##Await(unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  func##Await(_asyncUnique_##func, timeout, routine, id); \
}

#define NEW_ASYNC_VOID_STATE_0(machine, state, func) __ASYNC_STATE_INTERNAL(machine, state, 0, (func))

#define __ASYNC_TEMPLATE_1(func, type0) \
typedef struct _asyncData_##func { \
  type0 arg0; \
} sAsyncData_##func; \
sAsyncData_##func _asyncDataVar_##func; \
void _asyncInvoke_##func() { \
  func(_asyncDataVar_##func.arg0); \
}

#define NEW_ASYNC_VOID_1(func, type0) \
__ASYNC_TEMPLATE_1(func, type0) \
bool _asyncFlag_##func; \
task _asyncTask_##func() { \
  _asyncFlag_##func = true; \
  _asyncInvoke_##func(); \
  return_t \
} \
__ASYNC_API(; , tHog(); _asyncFlag_##func = false; tStart(_asyncTask_##func, detached); tRelease(); while(!_asyncFlag_##func) EndTimeSlice();, 1, (func, type0)) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
} \
void func##Await(unsigned long unique, unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, routine, id)) sleep(10); \
} \
void func##Await(unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  func##Await(_asyncUnique_##func, timeout, routine, id); \
}

#define NEW_ASYNC_VOID_STATE_1(machine, state, func, type0) __ASYNC_STATE_INTERNAL(machine, state, 1, (func, type0))

#define __ASYNC_TEMPLATE_2(func, type0, type1) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
} sAsyncData_##func; \
sAsyncData_##func _asyncDataVar_##func; \
void _asyncInvoke_##func() { \
  func(_asyncDataVar_##func.arg0, _asyncDataVar_##func.arg1); \
}

#define NEW_ASYNC_VOID_2(func, type0, type1) \
__ASYNC_TEMPLATE_2(func, type0, type1) \
bool _asyncFlag_##func; \
task _asyncTask_##func() { \
  _asyncFlag_##func = true; \
  _asyncInvoke_##func(); \
  return_t \
} \
__ASYNC_API(; , tHog(); _asyncFlag_##func = false; tStart(_asyncTask_##func, detached); tRelease(); while(!_asyncFlag_##func) EndTimeSlice();, 2, (func, type0, type1)) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
} \
void func##Await(unsigned long unique, unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, routine, id)) sleep(10); \
} \
void func##Await(unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  func##Await(_asyncUnique_##func, timeout, routine, id); \
}

#define NEW_ASYNC_VOID_STATE_2(machine, state, func, type0, type1) __ASYNC_STATE_INTERNAL(machine, state, 2, (func, type0, type1))

#define __ASYNC_TEMPLATE_3(func, type0, type1, type2) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
} sAsyncData_##func; \
sAsyncData_##func _asyncDataVar_##func; \
void _asyncInvoke_##func() { \
  func(_asyncDataVar_##func.arg0, _asyncDataVar_##func.arg1, _asyncDataVar_##func.arg2); \
}

#define NEW_ASYNC_VOID_3(func, type0, type1, type2) \
__ASYNC_TEMPLATE_3(func, type0, type1, type2) \
bool _asyncFlag_##func; \
task _asyncTask_##func() { \
  _asyncFlag_##func = true; \
  _asyncInvoke_##func(); \
  return_t \
} \
__ASYNC_API(; , tHog(); _asyncFlag_##func = false; tStart(_asyncTask_##func, detached); tRelease(); while(!_asyncFlag_##func) EndTimeSlice();, 3, (func, type0, type1, type2)) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
} \
void func##Await(unsigned long unique, unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, routine, id)) sleep(10); \
} \
void func##Await(unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  func##Await(_asyncUnique_##func, timeout, routine, id); \
}

#define NEW_ASYNC_VOID_STATE_3(machine, state, func, type0, type1, type2) __ASYNC_STATE_INTERNAL(machine, state, 3, (func, type0, type1, type2))

#define __ASYNC_TEMPLATE_4(func, type0, type1, type2, type3) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
} sAsyncData_##func; \
sAsyncData_##func _asyncDataVar_##func; \
void _asyncInvoke_##func() { \
  func(_asyncDataVar_##func.arg0, _asyncDataVar_##func.arg1, _asyncDataVar_##func.arg2, _asyncDataVar_##func.arg3); \
}

#define NEW_ASYNC_VOID_4(func, type0, type1, type2, type3) \
__ASYNC_TEMPLATE_4(func, type0, type1, type2, type3) \
bool _asyncFlag_##func; \
task _asyncTask_##func() { \
  _asyncFlag_##func = true; \
  _asyncInvoke_##func(); \
  return_t \
} \
__ASYNC_API(; , tHog(); _asyncFlag_##func = false; tStart(_asyncTask_##func, detached); tRelease(); while(!_asyncFlag_##func) EndTimeSlice();, 4, (func, type0, type1, type2, type3)) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
} \
void func##Await(unsigned long unique, unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, routine, id)) sleep(10); \
} \
void func##Await(unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  func##Await(_asyncUnique_##func, timeout, routine, id); \
}

#define NEW_ASYNC_VOID_STATE_4(machine, state, func, type0, type1, type2, type3) __ASYNC_STATE_INTERNAL(machine, state, 4, (func, type0, type1, type2, type3))

#define __ASYNC_TEMPLATE_5(func, type0, type1, type2, type3, type4) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
  type4 arg4; \
} sAsyncData_##func; \
sAsyncData_##func _asyncDataVar_##func; \
void _asyncInvoke_##func() { \
  func(_asyncDataVar_##func.arg0, _asyncDataVar_##func.arg1, _asyncDataVar_##func.arg2, _asyncDataVar_##func.arg3, _asyncDataVar_##func.arg4); \
}

#define NEW_ASYNC_VOID_5(func, type0, type1, type2, type3, type4) \
__ASYNC_TEMPLATE_5(func, type0, type1, type2, type3, type4) \
bool _asyncFlag_##func; \
task _asyncTask_##func() { \
  _asyncFlag_##func = true; \
  _asyncInvoke_##func(); \
  return_t \
} \
__ASYNC_API(; , tHog(); _asyncFlag_##func = false; tStart(_asyncTask_##func, detached); tRelease(); while(!_asyncFlag_##func) EndTimeSlice();, 5, (func, type0, type1, type2, type3, type4)) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
} \
void func##Await(unsigned long unique, unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, routine, id)) sleep(10); \
} \
void func##Await(unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  func##Await(_asyncUnique_##func, timeout, routine, id); \
}

#define NEW_ASYNC_VOID_STATE_5(machine, state, func, type0, type1, type2, type3, type4) __ASYNC_STATE_INTERNAL(machine, state, 5, (func, type0, type1, type2, type3, type4))

#define __ASYNC_TEMPLATE_6(func, type0, type1, type2, type3, type4, type5) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
  type4 arg4; \
  type5 arg5; \
} sAsyncData_##func; \
sAsyncData_##func _asyncDataVar_##func; \
void _asyncInvoke_##func() { \
  func(_asyncDataVar_##func.arg0, _asyncDataVar_##func.arg1, _asyncDataVar_##func.arg2, _asyncDataVar_##func.arg3, _asyncDataVar_##func.arg4, _asyncDataVar_##func.arg5); \
}

#define NEW_ASYNC_VOID_6(func, type0, type1, type2, type3, type4, type5) \
__ASYNC_TEMPLATE_6(func, type0, type1, type2, type3, type4, type5) \
bool _asyncFlag_##func; \
task _asyncTask_##func() { \
  _asyncFlag_##func = true; \
  _asyncInvoke_##func(); \
  return_t \
} \
__ASYNC_API(; , tHog(); _asyncFlag_##func = false; tStart(_asyncTask_##func, detached); tRelease(); while(!_asyncFlag_##func) EndTimeSlice();, 6, (func, type0, type1, type2, type3, type4, type5)) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
} \
void func##Await(unsigned long unique, unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, routine, id)) sleep(10); \
} \
void func##Await(unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  func##Await(_asyncUnique_##func, timeout, routine, id); \
}

#define NEW_ASYNC_VOID_STATE_6(machine, state, func, type0, type1, type2, type3, type4, type5) __ASYNC_STATE_INTERNAL(machine, state, 6, (func, type0, type1, type2, type3, type4, type5))

#define __ASYNC_TEMPLATE_7(func, type0, type1, type2, type3, type4, type5, type6) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
  type4 arg4; \
  type5 arg5; \
  type6 arg6; \
} sAsyncData_##func; \
sAsyncData_##func _asyncDataVar_##func; \
void _asyncInvoke_##func() { \
  func(_asyncDataVar_##func.arg0, _asyncDataVar_##func.arg1, _asyncDataVar_##func.arg2, _asyncDataVar_##func.arg3, _asyncDataVar_##func.arg4, _asyncDataVar_##func.arg5, _asyncDataVar_##func.arg6); \
}

#define NEW_ASYNC_VOID_7(func, type0, type1, type2, type3, type4, type5, type6) \
__ASYNC_TEMPLATE_7(func, type0, type1, type2, type3, type4, type5, type6) \
bool _asyncFlag_##func; \
task _asyncTask_##func() { \
  _asyncFlag_##func = true; \
  _asyncInvoke_##func(); \
  return_t \
} \
__ASYNC_API(; , tHog(); _asyncFlag_##func = false; tStart(_asyncTask_##func, detached); tRelease(); while(!_asyncFlag_##func) EndTimeSlice();, 7, (func, type0, type1, type2, type3, type4, type5, type6)) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
} \
void func##Await(unsigned long unique, unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, routine, id)) sleep(10); \
} \
void func##Await(unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  func##Await(_asyncUnique_##func, timeout, routine, id); \
}

#define NEW_ASYNC_VOID_STATE_7(machine, state, func, type0, type1, type2, type3, type4, type5, type6) __ASYNC_STATE_INTERNAL(machine, state, 7, (func, type0, type1, type2, type3, type4, type5, type6))

#define __ASYNC_TEMPLATE_8(func, type0, type1, type2, type3, type4, type5, type6, type7) \
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
sAsyncData_##func _asyncDataVar_##func; \
void _asyncInvoke_##func() { \
  func(_asyncDataVar_##func.arg0, _asyncDataVar_##func.arg1, _asyncDataVar_##func.arg2, _asyncDataVar_##func.arg3, _asyncDataVar_##func.arg4, _asyncDataVar_##func.arg5, _asyncDataVar_##func.arg6, _asyncDataVar_##func.arg7); \
}

#define NEW_ASYNC_VOID_8(func, type0, type1, type2, type3, type4, type5, type6, type7) \
__ASYNC_TEMPLATE_8(func, type0, type1, type2, type3, type4, type5, type6, type7) \
bool _asyncFlag_##func; \
task _asyncTask_##func() { \
  _asyncFlag_##func = true; \
  _asyncInvoke_##func(); \
  return_t \
} \
__ASYNC_API(; , tHog(); _asyncFlag_##func = false; tStart(_asyncTask_##func, detached); tRelease(); while(!_asyncFlag_##func) EndTimeSlice();, 8, (func, type0, type1, type2, type3, type4, type5, type6, type7)) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
} \
void func##Await(unsigned long unique, unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, routine, id)) sleep(10); \
} \
void func##Await(unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  func##Await(_asyncUnique_##func, timeout, routine, id); \
}

#define NEW_ASYNC_VOID_STATE_8(machine, state, func, type0, type1, type2, type3, type4, type5, type6, type7) __ASYNC_STATE_INTERNAL(machine, state, 8, (func, type0, type1, type2, type3, type4, type5, type6, type7))

#define __ASYNC_TEMPLATE_9(func, type0, type1, type2, type3, type4, type5, type6, type7, type8) \
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
sAsyncData_##func _asyncDataVar_##func; \
void _asyncInvoke_##func() { \
  func(_asyncDataVar_##func.arg0, _asyncDataVar_##func.arg1, _asyncDataVar_##func.arg2, _asyncDataVar_##func.arg3, _asyncDataVar_##func.arg4, _asyncDataVar_##func.arg5, _asyncDataVar_##func.arg6, _asyncDataVar_##func.arg7, _asyncDataVar_##func.arg8); \
}

#define NEW_ASYNC_VOID_9(func, type0, type1, type2, type3, type4, type5, type6, type7, type8) \
__ASYNC_TEMPLATE_9(func, type0, type1, type2, type3, type4, type5, type6, type7, type8) \
bool _asyncFlag_##func; \
task _asyncTask_##func() { \
  _asyncFlag_##func = true; \
  _asyncInvoke_##func(); \
  return_t \
} \
__ASYNC_API(; , tHog(); _asyncFlag_##func = false; tStart(_asyncTask_##func, detached); tRelease(); while(!_asyncFlag_##func) EndTimeSlice();, 9, (func, type0, type1, type2, type3, type4, type5, type6, type7, type8)) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
} \
void func##Await(unsigned long unique, unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, routine, id)) sleep(10); \
} \
void func##Await(unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  func##Await(_asyncUnique_##func, timeout, routine, id); \
}

#define NEW_ASYNC_VOID_STATE_9(machine, state, func, type0, type1, type2, type3, type4, type5, type6, type7, type8) __ASYNC_STATE_INTERNAL(machine, state, 9, (func, type0, type1, type2, type3, type4, type5, type6, type7, type8))

#define __ASYNC_TEMPLATE_10(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9) \
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
sAsyncData_##func _asyncDataVar_##func; \
void _asyncInvoke_##func() { \
  func(_asyncDataVar_##func.arg0, _asyncDataVar_##func.arg1, _asyncDataVar_##func.arg2, _asyncDataVar_##func.arg3, _asyncDataVar_##func.arg4, _asyncDataVar_##func.arg5, _asyncDataVar_##func.arg6, _asyncDataVar_##func.arg7, _asyncDataVar_##func.arg8, _asyncDataVar_##func.arg9); \
}

#define NEW_ASYNC_VOID_10(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9) \
__ASYNC_TEMPLATE_10(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9) \
bool _asyncFlag_##func; \
task _asyncTask_##func() { \
  _asyncFlag_##func = true; \
  _asyncInvoke_##func(); \
  return_t \
} \
__ASYNC_API(; , tHog(); _asyncFlag_##func = false; tStart(_asyncTask_##func, detached); tRelease(); while(!_asyncFlag_##func) EndTimeSlice();, 10, (func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9)) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
} \
void func##Await(unsigned long unique, unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, routine, id)) sleep(10); \
} \
void func##Await(unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  func##Await(_asyncUnique_##func, timeout, routine, id); \
}

#define NEW_ASYNC_VOID_STATE_10(machine, state, func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9) __ASYNC_STATE_INTERNAL(machine, state, 10, (func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9))

#define __ASYNC_TEMPLATE_11(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10) \
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
  type10 arg10; \
} sAsyncData_##func; \
sAsyncData_##func _asyncDataVar_##func; \
void _asyncInvoke_##func() { \
  func(_asyncDataVar_##func.arg0, _asyncDataVar_##func.arg1, _asyncDataVar_##func.arg2, _asyncDataVar_##func.arg3, _asyncDataVar_##func.arg4, _asyncDataVar_##func.arg5, _asyncDataVar_##func.arg6, _asyncDataVar_##func.arg7, _asyncDataVar_##func.arg8, _asyncDataVar_##func.arg9, _asyncDataVar_##func.arg10); \
}

#define NEW_ASYNC_VOID_11(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10) \
__ASYNC_TEMPLATE_11(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10) \
bool _asyncFlag_##func; \
task _asyncTask_##func() { \
  _asyncFlag_##func = true; \
  _asyncInvoke_##func(); \
  return_t \
} \
__ASYNC_API(; , tHog(); _asyncFlag_##func = false; tStart(_asyncTask_##func, detached); tRelease(); while(!_asyncFlag_##func) EndTimeSlice();, 11, (func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10)) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
} \
void func##Await(unsigned long unique, unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, routine, id)) sleep(10); \
} \
void func##Await(unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  func##Await(_asyncUnique_##func, timeout, routine, id); \
}

#define NEW_ASYNC_VOID_STATE_11(machine, state, func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10) __ASYNC_STATE_INTERNAL(machine, state, 11, (func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10))

#define __ASYNC_TEMPLATE_12(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11) \
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
  type10 arg10; \
  type11 arg11; \
} sAsyncData_##func; \
sAsyncData_##func _asyncDataVar_##func; \
void _asyncInvoke_##func() { \
  func(_asyncDataVar_##func.arg0, _asyncDataVar_##func.arg1, _asyncDataVar_##func.arg2, _asyncDataVar_##func.arg3, _asyncDataVar_##func.arg4, _asyncDataVar_##func.arg5, _asyncDataVar_##func.arg6, _asyncDataVar_##func.arg7, _asyncDataVar_##func.arg8, _asyncDataVar_##func.arg9, _asyncDataVar_##func.arg10, _asyncDataVar_##func.arg11); \
}

#define NEW_ASYNC_VOID_12(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11) \
__ASYNC_TEMPLATE_12(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11) \
bool _asyncFlag_##func; \
task _asyncTask_##func() { \
  _asyncFlag_##func = true; \
  _asyncInvoke_##func(); \
  return_t \
} \
__ASYNC_API(; , tHog(); _asyncFlag_##func = false; tStart(_asyncTask_##func, detached); tRelease(); while(!_asyncFlag_##func) EndTimeSlice();, 12, (func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11)) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
} \
void func##Await(unsigned long unique, unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, routine, id)) sleep(10); \
} \
void func##Await(unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  func##Await(_asyncUnique_##func, timeout, routine, id); \
}

#define NEW_ASYNC_VOID_STATE_12(machine, state, func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11) __ASYNC_STATE_INTERNAL(machine, state, 12, (func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11))

#define __ASYNC_TEMPLATE_13(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11, type12) \
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
  type10 arg10; \
  type11 arg11; \
  type12 arg12; \
} sAsyncData_##func; \
sAsyncData_##func _asyncDataVar_##func; \
void _asyncInvoke_##func() { \
  func(_asyncDataVar_##func.arg0, _asyncDataVar_##func.arg1, _asyncDataVar_##func.arg2, _asyncDataVar_##func.arg3, _asyncDataVar_##func.arg4, _asyncDataVar_##func.arg5, _asyncDataVar_##func.arg6, _asyncDataVar_##func.arg7, _asyncDataVar_##func.arg8, _asyncDataVar_##func.arg9, _asyncDataVar_##func.arg10, _asyncDataVar_##func.arg11, _asyncDataVar_##func.arg12); \
}

#define NEW_ASYNC_VOID_13(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11, type12) \
__ASYNC_TEMPLATE_13(func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11, type12) \
bool _asyncFlag_##func; \
task _asyncTask_##func() { \
  _asyncFlag_##func = true; \
  _asyncInvoke_##func(); \
  return_t \
} \
__ASYNC_API(; , tHog(); _asyncFlag_##func = false; tStart(_asyncTask_##func, detached); tRelease(); while(!_asyncFlag_##func) EndTimeSlice();, 13, (func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11, type12)) \
void func##Kill(bool killAll = false) { \
  if (killAll) \
    tStopAll(_asyncTask_##func); \
  else \
    tStop(_asyncTask_##func); \
} \
void func##Await(unsigned long unique, unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  while (RUNNING_STANDALONE(func, unique) && !TimedOut(timeout, routine, id)) sleep(10); \
} \
void func##Await(unsigned long timeout, const unsigned char *routine, unsigned short id) { \
  func##Await(_asyncUnique_##func, timeout, routine, id); \
}

#define NEW_ASYNC_VOID_STATE_13(machine, state, func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11, type12) __ASYNC_STATE_INTERNAL(machine, state, 13, (func, type0, type1, type2, type3, type4, type5, type6, type7, type8, type9, type10, type11, type12))

unsigned long _asyncUnique = 1;

