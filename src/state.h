#define PUSH_STATE(machine, state, code) \
{ \
  int machine##Last = machine##State; \
  machine##Set(state); \
  { code } \
  machine##Set(machine##Last); \
}

#define IS_CONFIGURED(machine) (machine##State != -1)
#define NEXT_STATE(new_state) { state = new_state; goto top; }

#define DECLARE_MACHINE(machine, states) \
void machine##Setup(); \
void machine##Set(states state); \
void machine##Set(states state, long arg); \
void machine##Set(states state, long arg, bool detached); \
void machine##Reset(); \
void machine##TimeoutWhile(states state, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true); \
void machine##TimeoutUntil(states state, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true); \

#define MAKE_MACHINE(machine, states, base, handler) \
states machine##State = -1; \
long machine##Arg = 0; \
void machine##Internal(states state) \
{ \
  long &arg = machine##Arg; \
  top: \
  writeDebugStreamLine("%d " #machine " %d -> %d, %x", nPgmTime, machine##State, state, arg); \
  switch (machine##State = state) \
  handler \
} \
NEW_ASYNC_VOID_1(machine##Internal, states); \
void machine##Setup() \
{ \
  if (IS_CONFIGURED(machine)) \
    machine##InternalKill(); \
  machine##Arg = 0; \
  machine##InternalAsync(base); \
  writeDebugStreamLine("Init " #machine " - " #base); \
} \
void machine##Set(states state) \
{ \
  if (nCurrentTask != ASYNC_TASK_NAME(machine##Internal)) \
    machine##InternalKill(); \
  machine##InternalAsync(state); \
} \
void machine##Set(states state, long arg) \
{ \
  if (nCurrentTask != ASYNC_TASK_NAME(machine##Internal)) \
    machine##InternalKill(); \
  machine##Arg = arg; \
  machine##InternalAsync(state); \
} \
void machine##Set(states state, long arg, bool detached) \
{ \
  if (nCurrentTask != ASYNC_TASK_NAME(machine##Internal)) \
    machine##InternalKill(); \
  machine##Arg = arg; \
  machine##InternalAsync(state, detached); \
} \
void machine##Reset() \
{ \
  machine##Set(base, 0); \
} \
void machine##TimeoutWhile(states state, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill) \
{ \
  timeoutWhileEqual(&machine##State, &state, sizeof(states), timeout, routine, id, kill); \
} \
void machine##TimeoutUntil(states state, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill) \
{ \
  timeoutWhileNotEqual(&machine##State, &state, sizeof(states), timeout, routine, id, kill); \
}

#define MAKE_ASYNC_ONLY_MACHINE_1(machine, baseImpl, argc0, argv0, after0) \
typedef enum _asyncStates_##machine { \
  machine##NotRunning, \
  __ASYNC_STATE_NAME_##argc0 argv0  \
} sAsyncStates_##machine; \
DECLARE_MACHINE(machine, sAsyncStates_##machine) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc0 argv0, argc0, argv0) \
MAKE_MACHINE(machine, sAsyncStates_##machine, machine##NotRunning, { \
case machine##NotRunning: \
  { baseImpl } \
  break; \
case __ASYNC_STATE_NAME_##argc0 argv0 : \
  __ASYNC_STATE_INVOKE_##argc0 argv0 \
  { after0 } \
  NEXT_STATE(machine##NotRunning) \
})

#define MAKE_ASYNC_ONLY_MACHINE_2(machine, baseImpl, argc0, argv0, after0, argc1, argv1, after1) \
typedef enum _asyncStates_##machine { \
  machine##NotRunning, \
  __ASYNC_STATE_NAME_##argc0 argv0 , \
  __ASYNC_STATE_NAME_##argc1 argv1  \
} sAsyncStates_##machine; \
DECLARE_MACHINE(machine, sAsyncStates_##machine) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc0 argv0, argc0, argv0) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc1 argv1, argc1, argv1) \
MAKE_MACHINE(machine, sAsyncStates_##machine, machine##NotRunning, { \
case machine##NotRunning: \
  { baseImpl } \
  break; \
case __ASYNC_STATE_NAME_##argc0 argv0 : \
  __ASYNC_STATE_INVOKE_##argc0 argv0 \
  { after0 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc1 argv1 : \
  __ASYNC_STATE_INVOKE_##argc1 argv1 \
  { after1 } \
  NEXT_STATE(machine##NotRunning) \
})

#define MAKE_ASYNC_ONLY_MACHINE_3(machine, baseImpl, argc0, argv0, after0, argc1, argv1, after1, argc2, argv2, after2) \
typedef enum _asyncStates_##machine { \
  machine##NotRunning, \
  __ASYNC_STATE_NAME_##argc0 argv0 , \
  __ASYNC_STATE_NAME_##argc1 argv1 , \
  __ASYNC_STATE_NAME_##argc2 argv2  \
} sAsyncStates_##machine; \
DECLARE_MACHINE(machine, sAsyncStates_##machine) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc0 argv0, argc0, argv0) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc1 argv1, argc1, argv1) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc2 argv2, argc2, argv2) \
MAKE_MACHINE(machine, sAsyncStates_##machine, machine##NotRunning, { \
case machine##NotRunning: \
  { baseImpl } \
  break; \
case __ASYNC_STATE_NAME_##argc0 argv0 : \
  __ASYNC_STATE_INVOKE_##argc0 argv0 \
  { after0 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc1 argv1 : \
  __ASYNC_STATE_INVOKE_##argc1 argv1 \
  { after1 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc2 argv2 : \
  __ASYNC_STATE_INVOKE_##argc2 argv2 \
  { after2 } \
  NEXT_STATE(machine##NotRunning) \
})

#define MAKE_ASYNC_ONLY_MACHINE_4(machine, baseImpl, argc0, argv0, after0, argc1, argv1, after1, argc2, argv2, after2, argc3, argv3, after3) \
typedef enum _asyncStates_##machine { \
  machine##NotRunning, \
  __ASYNC_STATE_NAME_##argc0 argv0 , \
  __ASYNC_STATE_NAME_##argc1 argv1 , \
  __ASYNC_STATE_NAME_##argc2 argv2 , \
  __ASYNC_STATE_NAME_##argc3 argv3  \
} sAsyncStates_##machine; \
DECLARE_MACHINE(machine, sAsyncStates_##machine) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc0 argv0, argc0, argv0) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc1 argv1, argc1, argv1) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc2 argv2, argc2, argv2) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc3 argv3, argc3, argv3) \
MAKE_MACHINE(machine, sAsyncStates_##machine, machine##NotRunning, { \
case machine##NotRunning: \
  { baseImpl } \
  break; \
case __ASYNC_STATE_NAME_##argc0 argv0 : \
  __ASYNC_STATE_INVOKE_##argc0 argv0 \
  { after0 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc1 argv1 : \
  __ASYNC_STATE_INVOKE_##argc1 argv1 \
  { after1 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc2 argv2 : \
  __ASYNC_STATE_INVOKE_##argc2 argv2 \
  { after2 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc3 argv3 : \
  __ASYNC_STATE_INVOKE_##argc3 argv3 \
  { after3 } \
  NEXT_STATE(machine##NotRunning) \
})

#define MAKE_ASYNC_ONLY_MACHINE_5(machine, baseImpl, argc0, argv0, after0, argc1, argv1, after1, argc2, argv2, after2, argc3, argv3, after3, argc4, argv4, after4) \
typedef enum _asyncStates_##machine { \
  machine##NotRunning, \
  __ASYNC_STATE_NAME_##argc0 argv0 , \
  __ASYNC_STATE_NAME_##argc1 argv1 , \
  __ASYNC_STATE_NAME_##argc2 argv2 , \
  __ASYNC_STATE_NAME_##argc3 argv3 , \
  __ASYNC_STATE_NAME_##argc4 argv4  \
} sAsyncStates_##machine; \
DECLARE_MACHINE(machine, sAsyncStates_##machine) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc0 argv0, argc0, argv0) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc1 argv1, argc1, argv1) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc2 argv2, argc2, argv2) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc3 argv3, argc3, argv3) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc4 argv4, argc4, argv4) \
MAKE_MACHINE(machine, sAsyncStates_##machine, machine##NotRunning, { \
case machine##NotRunning: \
  { baseImpl } \
  break; \
case __ASYNC_STATE_NAME_##argc0 argv0 : \
  __ASYNC_STATE_INVOKE_##argc0 argv0 \
  { after0 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc1 argv1 : \
  __ASYNC_STATE_INVOKE_##argc1 argv1 \
  { after1 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc2 argv2 : \
  __ASYNC_STATE_INVOKE_##argc2 argv2 \
  { after2 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc3 argv3 : \
  __ASYNC_STATE_INVOKE_##argc3 argv3 \
  { after3 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc4 argv4 : \
  __ASYNC_STATE_INVOKE_##argc4 argv4 \
  { after4 } \
  NEXT_STATE(machine##NotRunning) \
})

#define MAKE_ASYNC_ONLY_MACHINE_6(machine, baseImpl, argc0, argv0, after0, argc1, argv1, after1, argc2, argv2, after2, argc3, argv3, after3, argc4, argv4, after4, argc5, argv5, after5) \
typedef enum _asyncStates_##machine { \
  machine##NotRunning, \
  __ASYNC_STATE_NAME_##argc0 argv0 , \
  __ASYNC_STATE_NAME_##argc1 argv1 , \
  __ASYNC_STATE_NAME_##argc2 argv2 , \
  __ASYNC_STATE_NAME_##argc3 argv3 , \
  __ASYNC_STATE_NAME_##argc4 argv4 , \
  __ASYNC_STATE_NAME_##argc5 argv5  \
} sAsyncStates_##machine; \
DECLARE_MACHINE(machine, sAsyncStates_##machine) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc0 argv0, argc0, argv0) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc1 argv1, argc1, argv1) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc2 argv2, argc2, argv2) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc3 argv3, argc3, argv3) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc4 argv4, argc4, argv4) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc5 argv5, argc5, argv5) \
MAKE_MACHINE(machine, sAsyncStates_##machine, machine##NotRunning, { \
case machine##NotRunning: \
  { baseImpl } \
  break; \
case __ASYNC_STATE_NAME_##argc0 argv0 : \
  __ASYNC_STATE_INVOKE_##argc0 argv0 \
  { after0 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc1 argv1 : \
  __ASYNC_STATE_INVOKE_##argc1 argv1 \
  { after1 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc2 argv2 : \
  __ASYNC_STATE_INVOKE_##argc2 argv2 \
  { after2 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc3 argv3 : \
  __ASYNC_STATE_INVOKE_##argc3 argv3 \
  { after3 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc4 argv4 : \
  __ASYNC_STATE_INVOKE_##argc4 argv4 \
  { after4 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc5 argv5 : \
  __ASYNC_STATE_INVOKE_##argc5 argv5 \
  { after5 } \
  NEXT_STATE(machine##NotRunning) \
})

#define MAKE_ASYNC_ONLY_MACHINE_7(machine, baseImpl, argc0, argv0, after0, argc1, argv1, after1, argc2, argv2, after2, argc3, argv3, after3, argc4, argv4, after4, argc5, argv5, after5, argc6, argv6, after6) \
typedef enum _asyncStates_##machine { \
  machine##NotRunning, \
  __ASYNC_STATE_NAME_##argc0 argv0 , \
  __ASYNC_STATE_NAME_##argc1 argv1 , \
  __ASYNC_STATE_NAME_##argc2 argv2 , \
  __ASYNC_STATE_NAME_##argc3 argv3 , \
  __ASYNC_STATE_NAME_##argc4 argv4 , \
  __ASYNC_STATE_NAME_##argc5 argv5 , \
  __ASYNC_STATE_NAME_##argc6 argv6  \
} sAsyncStates_##machine; \
DECLARE_MACHINE(machine, sAsyncStates_##machine) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc0 argv0, argc0, argv0) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc1 argv1, argc1, argv1) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc2 argv2, argc2, argv2) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc3 argv3, argc3, argv3) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc4 argv4, argc4, argv4) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc5 argv5, argc5, argv5) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc6 argv6, argc6, argv6) \
MAKE_MACHINE(machine, sAsyncStates_##machine, machine##NotRunning, { \
case machine##NotRunning: \
  { baseImpl } \
  break; \
case __ASYNC_STATE_NAME_##argc0 argv0 : \
  __ASYNC_STATE_INVOKE_##argc0 argv0 \
  { after0 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc1 argv1 : \
  __ASYNC_STATE_INVOKE_##argc1 argv1 \
  { after1 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc2 argv2 : \
  __ASYNC_STATE_INVOKE_##argc2 argv2 \
  { after2 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc3 argv3 : \
  __ASYNC_STATE_INVOKE_##argc3 argv3 \
  { after3 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc4 argv4 : \
  __ASYNC_STATE_INVOKE_##argc4 argv4 \
  { after4 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc5 argv5 : \
  __ASYNC_STATE_INVOKE_##argc5 argv5 \
  { after5 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc6 argv6 : \
  __ASYNC_STATE_INVOKE_##argc6 argv6 \
  { after6 } \
  NEXT_STATE(machine##NotRunning) \
})

#define MAKE_ASYNC_ONLY_MACHINE_8(machine, baseImpl, argc0, argv0, after0, argc1, argv1, after1, argc2, argv2, after2, argc3, argv3, after3, argc4, argv4, after4, argc5, argv5, after5, argc6, argv6, after6, argc7, argv7, after7) \
typedef enum _asyncStates_##machine { \
  machine##NotRunning, \
  __ASYNC_STATE_NAME_##argc0 argv0 , \
  __ASYNC_STATE_NAME_##argc1 argv1 , \
  __ASYNC_STATE_NAME_##argc2 argv2 , \
  __ASYNC_STATE_NAME_##argc3 argv3 , \
  __ASYNC_STATE_NAME_##argc4 argv4 , \
  __ASYNC_STATE_NAME_##argc5 argv5 , \
  __ASYNC_STATE_NAME_##argc6 argv6 , \
  __ASYNC_STATE_NAME_##argc7 argv7  \
} sAsyncStates_##machine; \
DECLARE_MACHINE(machine, sAsyncStates_##machine) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc0 argv0, argc0, argv0) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc1 argv1, argc1, argv1) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc2 argv2, argc2, argv2) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc3 argv3, argc3, argv3) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc4 argv4, argc4, argv4) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc5 argv5, argc5, argv5) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc6 argv6, argc6, argv6) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc7 argv7, argc7, argv7) \
MAKE_MACHINE(machine, sAsyncStates_##machine, machine##NotRunning, { \
case machine##NotRunning: \
  { baseImpl } \
  break; \
case __ASYNC_STATE_NAME_##argc0 argv0 : \
  __ASYNC_STATE_INVOKE_##argc0 argv0 \
  { after0 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc1 argv1 : \
  __ASYNC_STATE_INVOKE_##argc1 argv1 \
  { after1 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc2 argv2 : \
  __ASYNC_STATE_INVOKE_##argc2 argv2 \
  { after2 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc3 argv3 : \
  __ASYNC_STATE_INVOKE_##argc3 argv3 \
  { after3 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc4 argv4 : \
  __ASYNC_STATE_INVOKE_##argc4 argv4 \
  { after4 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc5 argv5 : \
  __ASYNC_STATE_INVOKE_##argc5 argv5 \
  { after5 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc6 argv6 : \
  __ASYNC_STATE_INVOKE_##argc6 argv6 \
  { after6 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc7 argv7 : \
  __ASYNC_STATE_INVOKE_##argc7 argv7 \
  { after7 } \
  NEXT_STATE(machine##NotRunning) \
})

#define MAKE_ASYNC_ONLY_MACHINE_9(machine, baseImpl, argc0, argv0, after0, argc1, argv1, after1, argc2, argv2, after2, argc3, argv3, after3, argc4, argv4, after4, argc5, argv5, after5, argc6, argv6, after6, argc7, argv7, after7, argc8, argv8, after8) \
typedef enum _asyncStates_##machine { \
  machine##NotRunning, \
  __ASYNC_STATE_NAME_##argc0 argv0 , \
  __ASYNC_STATE_NAME_##argc1 argv1 , \
  __ASYNC_STATE_NAME_##argc2 argv2 , \
  __ASYNC_STATE_NAME_##argc3 argv3 , \
  __ASYNC_STATE_NAME_##argc4 argv4 , \
  __ASYNC_STATE_NAME_##argc5 argv5 , \
  __ASYNC_STATE_NAME_##argc6 argv6 , \
  __ASYNC_STATE_NAME_##argc7 argv7 , \
  __ASYNC_STATE_NAME_##argc8 argv8  \
} sAsyncStates_##machine; \
DECLARE_MACHINE(machine, sAsyncStates_##machine) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc0 argv0, argc0, argv0) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc1 argv1, argc1, argv1) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc2 argv2, argc2, argv2) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc3 argv3, argc3, argv3) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc4 argv4, argc4, argv4) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc5 argv5, argc5, argv5) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc6 argv6, argc6, argv6) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc7 argv7, argc7, argv7) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc8 argv8, argc8, argv8) \
MAKE_MACHINE(machine, sAsyncStates_##machine, machine##NotRunning, { \
case machine##NotRunning: \
  { baseImpl } \
  break; \
case __ASYNC_STATE_NAME_##argc0 argv0 : \
  __ASYNC_STATE_INVOKE_##argc0 argv0 \
  { after0 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc1 argv1 : \
  __ASYNC_STATE_INVOKE_##argc1 argv1 \
  { after1 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc2 argv2 : \
  __ASYNC_STATE_INVOKE_##argc2 argv2 \
  { after2 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc3 argv3 : \
  __ASYNC_STATE_INVOKE_##argc3 argv3 \
  { after3 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc4 argv4 : \
  __ASYNC_STATE_INVOKE_##argc4 argv4 \
  { after4 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc5 argv5 : \
  __ASYNC_STATE_INVOKE_##argc5 argv5 \
  { after5 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc6 argv6 : \
  __ASYNC_STATE_INVOKE_##argc6 argv6 \
  { after6 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc7 argv7 : \
  __ASYNC_STATE_INVOKE_##argc7 argv7 \
  { after7 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc8 argv8 : \
  __ASYNC_STATE_INVOKE_##argc8 argv8 \
  { after8 } \
  NEXT_STATE(machine##NotRunning) \
})

#define MAKE_ASYNC_ONLY_MACHINE_10(machine, baseImpl, argc0, argv0, after0, argc1, argv1, after1, argc2, argv2, after2, argc3, argv3, after3, argc4, argv4, after4, argc5, argv5, after5, argc6, argv6, after6, argc7, argv7, after7, argc8, argv8, after8, argc9, argv9, after9) \
typedef enum _asyncStates_##machine { \
  machine##NotRunning, \
  __ASYNC_STATE_NAME_##argc0 argv0 , \
  __ASYNC_STATE_NAME_##argc1 argv1 , \
  __ASYNC_STATE_NAME_##argc2 argv2 , \
  __ASYNC_STATE_NAME_##argc3 argv3 , \
  __ASYNC_STATE_NAME_##argc4 argv4 , \
  __ASYNC_STATE_NAME_##argc5 argv5 , \
  __ASYNC_STATE_NAME_##argc6 argv6 , \
  __ASYNC_STATE_NAME_##argc7 argv7 , \
  __ASYNC_STATE_NAME_##argc8 argv8 , \
  __ASYNC_STATE_NAME_##argc9 argv9  \
} sAsyncStates_##machine; \
DECLARE_MACHINE(machine, sAsyncStates_##machine) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc0 argv0, argc0, argv0) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc1 argv1, argc1, argv1) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc2 argv2, argc2, argv2) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc3 argv3, argc3, argv3) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc4 argv4, argc4, argv4) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc5 argv5, argc5, argv5) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc6 argv6, argc6, argv6) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc7 argv7, argc7, argv7) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc8 argv8, argc8, argv8) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc9 argv9, argc9, argv9) \
MAKE_MACHINE(machine, sAsyncStates_##machine, machine##NotRunning, { \
case machine##NotRunning: \
  { baseImpl } \
  break; \
case __ASYNC_STATE_NAME_##argc0 argv0 : \
  __ASYNC_STATE_INVOKE_##argc0 argv0 \
  { after0 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc1 argv1 : \
  __ASYNC_STATE_INVOKE_##argc1 argv1 \
  { after1 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc2 argv2 : \
  __ASYNC_STATE_INVOKE_##argc2 argv2 \
  { after2 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc3 argv3 : \
  __ASYNC_STATE_INVOKE_##argc3 argv3 \
  { after3 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc4 argv4 : \
  __ASYNC_STATE_INVOKE_##argc4 argv4 \
  { after4 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc5 argv5 : \
  __ASYNC_STATE_INVOKE_##argc5 argv5 \
  { after5 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc6 argv6 : \
  __ASYNC_STATE_INVOKE_##argc6 argv6 \
  { after6 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc7 argv7 : \
  __ASYNC_STATE_INVOKE_##argc7 argv7 \
  { after7 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc8 argv8 : \
  __ASYNC_STATE_INVOKE_##argc8 argv8 \
  { after8 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc9 argv9 : \
  __ASYNC_STATE_INVOKE_##argc9 argv9 \
  { after9 } \
  NEXT_STATE(machine##NotRunning) \
})

#define MAKE_ASYNC_ONLY_MACHINE_11(machine, baseImpl, argc0, argv0, after0, argc1, argv1, after1, argc2, argv2, after2, argc3, argv3, after3, argc4, argv4, after4, argc5, argv5, after5, argc6, argv6, after6, argc7, argv7, after7, argc8, argv8, after8, argc9, argv9, after9, argc10, argv10, after10) \
typedef enum _asyncStates_##machine { \
  machine##NotRunning, \
  __ASYNC_STATE_NAME_##argc0 argv0 , \
  __ASYNC_STATE_NAME_##argc1 argv1 , \
  __ASYNC_STATE_NAME_##argc2 argv2 , \
  __ASYNC_STATE_NAME_##argc3 argv3 , \
  __ASYNC_STATE_NAME_##argc4 argv4 , \
  __ASYNC_STATE_NAME_##argc5 argv5 , \
  __ASYNC_STATE_NAME_##argc6 argv6 , \
  __ASYNC_STATE_NAME_##argc7 argv7 , \
  __ASYNC_STATE_NAME_##argc8 argv8 , \
  __ASYNC_STATE_NAME_##argc9 argv9 , \
  __ASYNC_STATE_NAME_##argc10 argv10  \
} sAsyncStates_##machine; \
DECLARE_MACHINE(machine, sAsyncStates_##machine) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc0 argv0, argc0, argv0) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc1 argv1, argc1, argv1) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc2 argv2, argc2, argv2) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc3 argv3, argc3, argv3) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc4 argv4, argc4, argv4) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc5 argv5, argc5, argv5) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc6 argv6, argc6, argv6) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc7 argv7, argc7, argv7) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc8 argv8, argc8, argv8) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc9 argv9, argc9, argv9) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc10 argv10, argc10, argv10) \
MAKE_MACHINE(machine, sAsyncStates_##machine, machine##NotRunning, { \
case machine##NotRunning: \
  { baseImpl } \
  break; \
case __ASYNC_STATE_NAME_##argc0 argv0 : \
  __ASYNC_STATE_INVOKE_##argc0 argv0 \
  { after0 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc1 argv1 : \
  __ASYNC_STATE_INVOKE_##argc1 argv1 \
  { after1 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc2 argv2 : \
  __ASYNC_STATE_INVOKE_##argc2 argv2 \
  { after2 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc3 argv3 : \
  __ASYNC_STATE_INVOKE_##argc3 argv3 \
  { after3 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc4 argv4 : \
  __ASYNC_STATE_INVOKE_##argc4 argv4 \
  { after4 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc5 argv5 : \
  __ASYNC_STATE_INVOKE_##argc5 argv5 \
  { after5 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc6 argv6 : \
  __ASYNC_STATE_INVOKE_##argc6 argv6 \
  { after6 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc7 argv7 : \
  __ASYNC_STATE_INVOKE_##argc7 argv7 \
  { after7 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc8 argv8 : \
  __ASYNC_STATE_INVOKE_##argc8 argv8 \
  { after8 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc9 argv9 : \
  __ASYNC_STATE_INVOKE_##argc9 argv9 \
  { after9 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc10 argv10 : \
  __ASYNC_STATE_INVOKE_##argc10 argv10 \
  { after10 } \
  NEXT_STATE(machine##NotRunning) \
})

#define MAKE_ASYNC_ONLY_MACHINE_12(machine, baseImpl, argc0, argv0, after0, argc1, argv1, after1, argc2, argv2, after2, argc3, argv3, after3, argc4, argv4, after4, argc5, argv5, after5, argc6, argv6, after6, argc7, argv7, after7, argc8, argv8, after8, argc9, argv9, after9, argc10, argv10, after10, argc11, argv11, after11) \
typedef enum _asyncStates_##machine { \
  machine##NotRunning, \
  __ASYNC_STATE_NAME_##argc0 argv0 , \
  __ASYNC_STATE_NAME_##argc1 argv1 , \
  __ASYNC_STATE_NAME_##argc2 argv2 , \
  __ASYNC_STATE_NAME_##argc3 argv3 , \
  __ASYNC_STATE_NAME_##argc4 argv4 , \
  __ASYNC_STATE_NAME_##argc5 argv5 , \
  __ASYNC_STATE_NAME_##argc6 argv6 , \
  __ASYNC_STATE_NAME_##argc7 argv7 , \
  __ASYNC_STATE_NAME_##argc8 argv8 , \
  __ASYNC_STATE_NAME_##argc9 argv9 , \
  __ASYNC_STATE_NAME_##argc10 argv10 , \
  __ASYNC_STATE_NAME_##argc11 argv11  \
} sAsyncStates_##machine; \
DECLARE_MACHINE(machine, sAsyncStates_##machine) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc0 argv0, argc0, argv0) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc1 argv1, argc1, argv1) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc2 argv2, argc2, argv2) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc3 argv3, argc3, argv3) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc4 argv4, argc4, argv4) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc5 argv5, argc5, argv5) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc6 argv6, argc6, argv6) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc7 argv7, argc7, argv7) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc8 argv8, argc8, argv8) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc9 argv9, argc9, argv9) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc10 argv10, argc10, argv10) \
__ASYNC_STATE_INTERNAL(machine, __ASYNC_STATE_NAME_##argc11 argv11, argc11, argv11) \
MAKE_MACHINE(machine, sAsyncStates_##machine, machine##NotRunning, { \
case machine##NotRunning: \
  { baseImpl } \
  break; \
case __ASYNC_STATE_NAME_##argc0 argv0 : \
  __ASYNC_STATE_INVOKE_##argc0 argv0 \
  { after0 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc1 argv1 : \
  __ASYNC_STATE_INVOKE_##argc1 argv1 \
  { after1 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc2 argv2 : \
  __ASYNC_STATE_INVOKE_##argc2 argv2 \
  { after2 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc3 argv3 : \
  __ASYNC_STATE_INVOKE_##argc3 argv3 \
  { after3 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc4 argv4 : \
  __ASYNC_STATE_INVOKE_##argc4 argv4 \
  { after4 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc5 argv5 : \
  __ASYNC_STATE_INVOKE_##argc5 argv5 \
  { after5 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc6 argv6 : \
  __ASYNC_STATE_INVOKE_##argc6 argv6 \
  { after6 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc7 argv7 : \
  __ASYNC_STATE_INVOKE_##argc7 argv7 \
  { after7 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc8 argv8 : \
  __ASYNC_STATE_INVOKE_##argc8 argv8 \
  { after8 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc9 argv9 : \
  __ASYNC_STATE_INVOKE_##argc9 argv9 \
  { after9 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc10 argv10 : \
  __ASYNC_STATE_INVOKE_##argc10 argv10 \
  { after10 } \
  NEXT_STATE(machine##NotRunning) \
case __ASYNC_STATE_NAME_##argc11 argv11 : \
  __ASYNC_STATE_INVOKE_##argc11 argv11 \
  { after11 } \
  NEXT_STATE(machine##NotRunning) \
})

