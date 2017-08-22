/* Defines */
#define DEG_PER_POT 18.2
#define BEAM_R 16
#define BEAM_S 16
#define CAL_BASE 180
#define A_VERTICAL 2668
#define B_VERTICAL 3051

/* Types */
typedef struct _sArmPos {
	float x;
	float y;
} sArmPos;

typedef enum _tArmStates {
	armMoveHoldToTarget,
	armIdle,
} tArmStates;

/* Variables */
sArmPos gArmTarget;
bool gPreserveArmTarget = false;

/* Functions */
void calculatePosition(sArmPos& position, float a, float b);
float calculateTargetA(float x, float y);
float calculateTargetB(float x, float y);
float potiToDeg(short poti, short cal);
float degToPoti(float deg, short cal);
void currentArmPosition(sArmPos& position);
void setArm(sbyte powerA, sbyte powerB);
void setArmF(float powerA, float powerB);
bool armToTarget(bool safety, bool hold);

/* Async Functions */
NEW_ASYNC_2(bool, armToTarget, bool, safety, bool, hold, return armToTarget(safety, hold););

/* State Machine */
MAKE_MACHINE(arm, tArmStates, armMoveHoldToTarget,
{
	case armMoveHoldToTarget: if (gPreserveArmTarget) gPreserveArmTarget = false; else currentArmPosition(gArmTarget); armToTargetAsync(false, true); break;
	case armIdle: setArm(0, 0); break;
})
