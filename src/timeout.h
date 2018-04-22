/* Types */
typedef enum _timeoutVelSourceType
{
	velNone,
	velSensor,
	velPtr,
	velLocalY,
	velTurn
} tTimeoutVelSourceType;

/* Defines */
#define VEL_NONE velNone, 0
#define VEL_SENSOR(sen) velSensor, (unsigned long)(sen)
#define VEL_POINTER(ptr) velPtr, (unsigned long)(ptr)
#define VEL_LOCAL_Y velLocalY, 0
#define VEL_TURN velTurn, 0

/* Functions */
void timeoutWhileEqual(void *val1, void *val2, unsigned int size, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true);
void timeoutWhileNotEqual(void *val1, void *val2, unsigned int size, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true);
void timeoutWhileLessThanL(tTimeoutVelSourceType velSourceType, unsigned long velSourceData, float vel, long *val, long threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true, bool correctSign = true);
void timeoutWhileGreaterThanL(tTimeoutVelSourceType velSourceType, unsigned long velSourceData, float vel, long *val, long threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true, bool correctSign = true);
void timeoutWhileLessThanF(tTimeoutVelSourceType velSourceType, unsigned long velSourceData, float vel, float *val, float threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true, bool correctSign = true);
void timeoutWhileGreaterThanF(tTimeoutVelSourceType velSourceType, unsigned long velSourceData, float vel, float *val, float threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true, bool correctSign = true);
void timeoutWhileTrue(bool *val, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true);
void timeoutWhileFalse(bool *val, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true);
