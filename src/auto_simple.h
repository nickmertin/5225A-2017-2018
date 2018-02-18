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

typedef struct _turnNewState
{
	word startPower;
	bool isShort;
	bool isLong;
	bool firstRun;
	float kP_vel;
	float kP_pwr;
	float kI_pwr;
	float kD_pwr;
	float integral;
	float lstErr;
	unsigned long startTime;
	unsigned long lstTime;
} sTurnNewState;

/* Variables */
sVector gTargetLast;

/* Functions */
void moveToTargetSimple(float y, float x, float ys, float xs, byte power, float decelEarly, byte decelPower, float dropEarly = 0, tStopType stopType = stopSoft | stopHarsh, bool slow = true);
void moveToTargetSimple(float y, float x, byte power, float decelEarly, byte decelPower, float dropEarly = 0, tStopType stopType = stopSoft | stopHarsh, bool slow = true);
void moveToTargetDisSimple(float a, float d, float ys, float xs, byte power, float decelEarly, byte decelPower, float dropEarly = 0, tStopType stopType = stopSoft | stopHarsh, bool slow = true);
void moveToTargetDisSimple(float a, float d, byte power, float decelEarly, byte decelPower, float dropEarly = 0, tStopType stopType = stopSoft | stopHarsh, bool slow = true);
void turnToAngleRadSimple(float a, tTurnDir turnDir, byte left, byte right, bool mogo = false);
void turnToAngleSimple(float a, tTurnDir turnDir, byte left, byte right, bool mogo = false);
void turnToTargetSimple(float y, float x, tTurnDir turnDir, byte left, byte right, bool mogo = false, float offset = 0);
void turnSimpleInternalCw(float a, sTurnState& state);
void turnSimpleInternalCcw(float a, sTurnState& state);
//void turnToAngleStupid(float a, tTurnDir turnDir);
//void turnToTargetStupid(float y, float x, tTurnDir turnDir, float offset);
//void turnToAngleCustom(float a, tTurnDir turnDir, byte power, float epsilon);
//void turnToTargetCustom(float y, float x, tTurnDir turnDir, float offset, byte power, float epsilon);

//void turnToAngleNewRad (float a, tTurnDir turnDir);
//void turnToAngleNew(float a, tTurnDir turnDir);
//void turnToTargetNew(float y, float x, tTurnDir turnDir, float offset);
//void turnNewInternal(float a, sTurnNewState& state);

void resetBlueLeft();
void resetBlueRight();

MAKE_ASYNC_ONLY_MACHINE_6(autoSimple, ;,
10, (moveToTargetSimple, float, float, float, float, byte, float, byte, float, tStopType, bool), ;,
10, (moveToTargetDisSimple, float, float, float, float, byte, float, byte, float, tStopType, bool), ;,
5,  (turnToAngleSimple, float, tTurnDir, byte, byte, bool), ;,
7,  (turnToTargetSimple, float, float, tTurnDir, byte, byte, bool, float), ;,
//2, (turnToAngleStupid, float, tTurnDir), ;,
//4, (turnToTargetStupid, float, float, tTurnDir, float), ;,
//4, (turnToAngleCustom, float, tTurnDir, byte, float), ;,
//6, (turnToTargetCustom, float, float, tTurnDir, float, byte, float), ;,
//2, (turnToAngleNew, float, tTurnDir), ;,
//4, (turnToTargetNew, float, float, tTurnDir, float), ;,
0, (resetBlueLeft), ;,
0, (resetBlueRight), ;
)
