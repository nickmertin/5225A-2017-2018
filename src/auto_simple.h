/* Enumerations */
typedef enum _stopType
{
	stopNone =		0b00000000,
	stopSoft =		0b00000001,
	stopHarsh =		0b00000010
} tStopType;

/* Variables */
sVector gTargetLast;

/* Functions */
void moveToTargetSimple(float y, float x, float ys, float xs, byte power, float dropEarly = 0, tStopType stopType = stopSoft | stopHarsh, bool slow = true);
void moveToTargetSimple(float y, float x, byte power, float dropEarly = 0, tStopType stopType = stopSoft | stopHarsh, bool slow = true);
void moveToTargetDisSimple(float a, float d, float ys, float xs, byte power, float dropEarly = 0, tStopType stopType = stopSoft | stopHarsh, bool slow = true);
void moveToTargetDisSimple(float a, float d, byte power, float dropEarly = 0, tStopType stopType = stopSoft | stopHarsh, bool slow = true);

void turnToAngleRadSimple(float a, tTurnDir turnDir, byte left, byte right);
void turnToAngleSimple(float a, tTurnDir turnDir, byte left, byte right);

///* Async Functions */
//NEW_ASYNC_VOID_10(moveToTargetSimple, float, float, float, float, byte, float, tStopType, bool)
//NEW_ASYNC_VOID_6(turnToAngleSimple, float, tTurnDir, byte, byte)
