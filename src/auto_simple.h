/* Enumerations */
typedef enum _stopType
{
	stopNone =		0b00000000,
	stopSoft =		0b00000001,
	stopHarsh =		0b00000010
} tStopType;

/* Structures */
typedef struct _turnState
{
	float target;
	float power;
	float left;
	float right;
	float error;
	float lstError;
	float input;
	bool mogo;
	unsigned long time;
	unsigned long lstTime;
	unsigned long nextDebug;
} sTurnState;

/* Variables */
sVector gTargetLast;

/* Functions */
void moveToTargetSimple(float y, float x, float ys, float xs, byte power, float maxErrX, float decelEarly, byte decelPower, float dropEarly = 0, tStopType stopType = stopSoft | stopHarsh, bool slow = true);
void moveToTargetDisSimple(float a, float d, float ys, float xs, byte power, float maxErrX, float decelEarly, byte decelPower, float dropEarly = 0, tStopType stopType = stopSoft | stopHarsh, bool slow = true);
void turnToAngleRadSimple(float a, tTurnDir turnDir, byte left, byte right, bool mogo = false);
void turnToAngleSimple(float a, tTurnDir turnDir, byte left, byte right, bool mogo = false);
void turnToTargetSimple(float y, float x, tTurnDir turnDir, byte left, byte right, bool mogo = false, float offset = 0);
void turnSimpleInternalCw(float a, sTurnState& state);
void turnSimpleInternalCcw(float a, sTurnState& state);
void turnToAngleRadNewAlg(float a, tTurnDir turnDir, bool mogo = false);
void turnToAngleCustom(float a, tTurnDir turnDir, byte power, float epsilon);
void turnToTargetCustom(float y, float x, tTurnDir turnDir, float offset, byte power, float epsilon);

MAKE_ASYNC_ONLY_MACHINE_6(autoSimple, ;,
11, (moveToTargetSimple, float, float, float, float, byte, float, float, byte, float, tStopType, bool), ;,
11, (moveToTargetDisSimple, float, float, float, float, byte, float, float, byte, float, tStopType, bool), ;,
5,  (turnToAngleSimple, float, tTurnDir, byte, byte, bool), ;,
7,  (turnToTargetSimple, float, float, tTurnDir, byte, byte, bool, float), ;,
4, (turnToAngleCustom, float, tTurnDir, byte, float), ;,
6, (turnToTargetCustom, float, float, tTurnDir, float, byte, float), ;
)
