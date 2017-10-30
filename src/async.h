#define NEW_ASYNC_VOID_0(func) \
task asyncTask_##func() { \
  func(); \
  return_t; \
} \
void func##Async() { \
  tStart(asyncTask_##func); \
} \
void func##Await(unsigned long timeout) { \
  while (tEls[asyncTask_##func].parent != -1 && !TimedOut(timeout)) sleep(10); \
} \
void func##Kill() { \
  tStopAll(asyncTask_##func); \
}

#define NEW_ASYNC_VOID_1(func, type0) \
typedef struct _asyncData_##func { \
  type0 arg0; \
} sAsyncData_##func; \
sAsyncData_##func gAsyncData_##func; \
task asyncTask_##func() { \
  func(gAsyncData_##func.arg0); \
  return_t; \
} \
void func##Async(type0 arg0) { \
  gAsyncData_##func.arg0 = arg0; \
  tStart(asyncTask_##func); \
} \
void func##Await(unsigned long timeout) { \
  while (tEls[asyncTask_##func].parent != -1 && !TimedOut(timeout)) sleep(10); \
} \
void func##Kill() { \
  tStopAll(asyncTask_##func); \
}

#define NEW_ASYNC_VOID_2(func, type0, type1) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
} sAsyncData_##func; \
sAsyncData_##func gAsyncData_##func; \
task asyncTask_##func() { \
  func(gAsyncData_##func.arg0, gAsyncData_##func.arg1); \
  return_t; \
} \
void func##Async(type0 arg0, type1 arg1) { \
  gAsyncData_##func.arg0 = arg0; \
  gAsyncData_##func.arg1 = arg1; \
  tStart(asyncTask_##func); \
} \
void func##Await(unsigned long timeout) { \
  while (tEls[asyncTask_##func].parent != -1 && !TimedOut(timeout)) sleep(10); \
} \
void func##Kill() { \
  tStopAll(asyncTask_##func); \
}

#define NEW_ASYNC_VOID_3(func, type0, type1, type2) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
} sAsyncData_##func; \
sAsyncData_##func gAsyncData_##func; \
task asyncTask_##func() { \
  func(gAsyncData_##func.arg0, gAsyncData_##func.arg1, gAsyncData_##func.arg2); \
  return_t; \
} \
void func##Async(type0 arg0, type1 arg1, type2 arg2) { \
  gAsyncData_##func.arg0 = arg0; \
  gAsyncData_##func.arg1 = arg1; \
  gAsyncData_##func.arg2 = arg2; \
  tStart(asyncTask_##func); \
} \
void func##Await(unsigned long timeout) { \
  while (tEls[asyncTask_##func].parent != -1 && !TimedOut(timeout)) sleep(10); \
} \
void func##Kill() { \
  tStopAll(asyncTask_##func); \
}

#define NEW_ASYNC_VOID_4(func, type0, type1, type2, type3) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
} sAsyncData_##func; \
sAsyncData_##func gAsyncData_##func; \
task asyncTask_##func() { \
  func(gAsyncData_##func.arg0, gAsyncData_##func.arg1, gAsyncData_##func.arg2, gAsyncData_##func.arg3); \
  return_t; \
} \
void func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3) { \
  gAsyncData_##func.arg0 = arg0; \
  gAsyncData_##func.arg1 = arg1; \
  gAsyncData_##func.arg2 = arg2; \
  gAsyncData_##func.arg3 = arg3; \
  tStart(asyncTask_##func); \
} \
void func##Await(unsigned long timeout) { \
  while (tEls[asyncTask_##func].parent != -1 && !TimedOut(timeout)) sleep(10); \
} \
void func##Kill() { \
  tStopAll(asyncTask_##func); \
}

#define NEW_ASYNC_VOID_5(func, type0, type1, type2, type3, type4) \
typedef struct _asyncData_##func { \
  type0 arg0; \
  type1 arg1; \
  type2 arg2; \
  type3 arg3; \
  type4 arg4; \
} sAsyncData_##func; \
sAsyncData_##func gAsyncData_##func; \
task asyncTask_##func() { \
  func(gAsyncData_##func.arg0, gAsyncData_##func.arg1, gAsyncData_##func.arg2, gAsyncData_##func.arg3, gAsyncData_##func.arg4); \
  return_t; \
} \
void func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4) { \
  gAsyncData_##func.arg0 = arg0; \
  gAsyncData_##func.arg1 = arg1; \
  gAsyncData_##func.arg2 = arg2; \
  gAsyncData_##func.arg3 = arg3; \
  gAsyncData_##func.arg4 = arg4; \
  tStart(asyncTask_##func); \
} \
void func##Await(unsigned long timeout) { \
  while (tEls[asyncTask_##func].parent != -1 && !TimedOut(timeout)) sleep(10); \
} \
void func##Kill() { \
  tStopAll(asyncTask_##func); \
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
sAsyncData_##func gAsyncData_##func; \
task asyncTask_##func() { \
  func(gAsyncData_##func.arg0, gAsyncData_##func.arg1, gAsyncData_##func.arg2, gAsyncData_##func.arg3, gAsyncData_##func.arg4, gAsyncData_##func.arg5); \
  return_t; \
} \
void func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5) { \
  gAsyncData_##func.arg0 = arg0; \
  gAsyncData_##func.arg1 = arg1; \
  gAsyncData_##func.arg2 = arg2; \
  gAsyncData_##func.arg3 = arg3; \
  gAsyncData_##func.arg4 = arg4; \
  gAsyncData_##func.arg5 = arg5; \
  tStart(asyncTask_##func); \
} \
void func##Await(unsigned long timeout) { \
  while (tEls[asyncTask_##func].parent != -1 && !TimedOut(timeout)) sleep(10); \
} \
void func##Kill() { \
  tStopAll(asyncTask_##func); \
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
sAsyncData_##func gAsyncData_##func; \
task asyncTask_##func() { \
  func(gAsyncData_##func.arg0, gAsyncData_##func.arg1, gAsyncData_##func.arg2, gAsyncData_##func.arg3, gAsyncData_##func.arg4, gAsyncData_##func.arg5, gAsyncData_##func.arg6); \
  return_t; \
} \
void func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6) { \
  gAsyncData_##func.arg0 = arg0; \
  gAsyncData_##func.arg1 = arg1; \
  gAsyncData_##func.arg2 = arg2; \
  gAsyncData_##func.arg3 = arg3; \
  gAsyncData_##func.arg4 = arg4; \
  gAsyncData_##func.arg5 = arg5; \
  gAsyncData_##func.arg6 = arg6; \
  tStart(asyncTask_##func); \
} \
void func##Await(unsigned long timeout) { \
  while (tEls[asyncTask_##func].parent != -1 && !TimedOut(timeout)) sleep(10); \
} \
void func##Kill() { \
  tStopAll(asyncTask_##func); \
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
sAsyncData_##func gAsyncData_##func; \
task asyncTask_##func() { \
  func(gAsyncData_##func.arg0, gAsyncData_##func.arg1, gAsyncData_##func.arg2, gAsyncData_##func.arg3, gAsyncData_##func.arg4, gAsyncData_##func.arg5, gAsyncData_##func.arg6, gAsyncData_##func.arg7); \
  return_t; \
} \
void func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7) { \
  gAsyncData_##func.arg0 = arg0; \
  gAsyncData_##func.arg1 = arg1; \
  gAsyncData_##func.arg2 = arg2; \
  gAsyncData_##func.arg3 = arg3; \
  gAsyncData_##func.arg4 = arg4; \
  gAsyncData_##func.arg5 = arg5; \
  gAsyncData_##func.arg6 = arg6; \
  gAsyncData_##func.arg7 = arg7; \
  tStart(asyncTask_##func); \
} \
void func##Await(unsigned long timeout) { \
  while (tEls[asyncTask_##func].parent != -1 && !TimedOut(timeout)) sleep(10); \
} \
void func##Kill() { \
  tStopAll(asyncTask_##func); \
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
sAsyncData_##func gAsyncData_##func; \
task asyncTask_##func() { \
  func(gAsyncData_##func.arg0, gAsyncData_##func.arg1, gAsyncData_##func.arg2, gAsyncData_##func.arg3, gAsyncData_##func.arg4, gAsyncData_##func.arg5, gAsyncData_##func.arg6, gAsyncData_##func.arg7, gAsyncData_##func.arg8); \
  return_t; \
} \
void func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7, type8 arg8) { \
  gAsyncData_##func.arg0 = arg0; \
  gAsyncData_##func.arg1 = arg1; \
  gAsyncData_##func.arg2 = arg2; \
  gAsyncData_##func.arg3 = arg3; \
  gAsyncData_##func.arg4 = arg4; \
  gAsyncData_##func.arg5 = arg5; \
  gAsyncData_##func.arg6 = arg6; \
  gAsyncData_##func.arg7 = arg7; \
  gAsyncData_##func.arg8 = arg8; \
  tStart(asyncTask_##func); \
} \
void func##Await(unsigned long timeout) { \
  while (tEls[asyncTask_##func].parent != -1 && !TimedOut(timeout)) sleep(10); \
} \
void func##Kill() { \
  tStopAll(asyncTask_##func); \
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
sAsyncData_##func gAsyncData_##func; \
task asyncTask_##func() { \
  func(gAsyncData_##func.arg0, gAsyncData_##func.arg1, gAsyncData_##func.arg2, gAsyncData_##func.arg3, gAsyncData_##func.arg4, gAsyncData_##func.arg5, gAsyncData_##func.arg6, gAsyncData_##func.arg7, gAsyncData_##func.arg8, gAsyncData_##func.arg9); \
  return_t; \
} \
void func##Async(type0 arg0, type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6, type7 arg7, type8 arg8, type9 arg9) { \
  gAsyncData_##func.arg0 = arg0; \
  gAsyncData_##func.arg1 = arg1; \
  gAsyncData_##func.arg2 = arg2; \
  gAsyncData_##func.arg3 = arg3; \
  gAsyncData_##func.arg4 = arg4; \
  gAsyncData_##func.arg5 = arg5; \
  gAsyncData_##func.arg6 = arg6; \
  gAsyncData_##func.arg7 = arg7; \
  gAsyncData_##func.arg8 = arg8; \
  gAsyncData_##func.arg9 = arg9; \
  tStart(asyncTask_##func); \
} \
void func##Await(unsigned long timeout) { \
  while (tEls[asyncTask_##func].parent != -1 && !TimedOut(timeout)) sleep(10); \
} \
void func##Kill() { \
  tStopAll(asyncTask_##func); \
}

