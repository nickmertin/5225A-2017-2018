/* Enumerations */
typedef enum _stopType
{
	stopNone =		0b00000000,
	stopSoft =		0b00000001,
	stopHarsh =		0b00000010
} tStopType;

typedef enum _mttMode
{
	mttSimple,
	mttProportional,
	mttCascading
} tMttMode;

/* Variables */
sVector gTargetLast;

/* Functions */
void moveToTargetSimple(float y, float x, float ys, float xs, byte power, byte startPower, float maxErrX, float decelEarly, byte decelPower, float dropEarly = 0, tStopType stopType = stopSoft | stopHarsh, tMttMode mode = mttProportional, bool velSafety = false);
void moveToTargetDisSimple(float a, float d, float ys, float xs, byte power, byte startPower, float maxErrX, float decelEarly, byte decelPower, float dropEarly = 0, tStopType stopType = stopSoft | stopHarsh, tMttMode mode = mttProportional, bool velSafety = false);
void turnToAngleNewAlg(float a, tTurnDir turnDir, float fullRatio, byte coastPower, float stopOffsetDeg, bool mogo = false, bool harshStop = true, bool velSafety = false);
void turnToTargetNewAlg(float y, float x, tTurnDir turnDir, float fullRatio, byte coastPower, float stopOffsetDeg, bool mogo = false, bool harshStop = true, float offset = 0, bool velSafety = false);
void sweepTurnToTarget(float y, float x, float a, float r, tTurnDir turnDir, byte power, bool slow = true, bool velSafety = false);

MAKE_ASYNC_ONLY_MACHINE_5(autoSimple, ;,
13, (moveToTargetSimple, float, float, float, float, byte, byte, float, float, byte, float, tStopType, tMttMode, bool), ;,
13, (moveToTargetDisSimple, float, float, float, float, byte, byte, float, float, byte, float, tStopType, tMttMode, bool), ;,
8, (turnToAngleNewAlg, float, tTurnDir, float, byte, float, bool, bool, bool), ;,
10, (turnToTargetNewAlg, float, float, tTurnDir, float, byte, float, bool, bool, float, bool), ;,
8, (sweepTurnToTarget, float, float, float, float, tTurnDir, byte, bool, bool), ;
)
