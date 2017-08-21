/* Defines */
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_WARN
#endif

#define S_LOG { char _logTemp[100]; sprintf(_logTemp,

#if LOG_LEVEL >= LOG_DATA
#define E_LOG_DATA ); writeDebugStreamLine("%d  \t[DATA]   %s", nPgmTime, &_logTemp); }
#else
#define E_LOG_DATA ); }
#endif

#if LOG_LEVEL >= LOG_INFO
#define E_LOG_INFO ); writeDebugStreamLine("%d  \t[INFO]   %s", nPgmTime, &_logTemp); }
#else
#define E_LOG_INFO ); }
#endif

#if LOG_LEVEL >= LOG_WARN
#define E_LOG_WARN ); writeDebugStreamLine("%d  \t[WARN]   %s", nPgmTime, &_logTemp); }
#else
#define E_LOG_WARN ); }
#endif

#if LOG_LEVEL >= LOG_EROR
#define E_LOG_EROR ); writeDebugStreamLine("%d  \t[EROR]  %s", nPgmTime, &_logTemp); }
#else
#define E_LOG_EROR ); }
#endif
