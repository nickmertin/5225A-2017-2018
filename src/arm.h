/* Defines */
#define DEG_PER_POT 18.2
#define POT_PER_DEG (1.0 / DEG_PER_POT)
#define BEAM_R 16
#define BEAM_S 16
#define BEAM_MAX_SQ ((BEAM_R + BEAM_S) * (BEAM_R + BEAM_S))
#define CAL_BASE 180
#define A_VERTICAL 2711
#define B_VERTICAL 2898

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
bool armFollowParabola(float a, float b, float c, float targetX, tArmStates nextState);

/* Async Functions */
NEW_ASYNC_2(bool, armToTarget, bool, safety, bool, hold, return armToTarget(safety, hold););
NEW_ASYNC_VOID_5(armFollowParabola, float, a, float, b, float, c, float, targetX, tArmStates, nextState, armFollowParabola(a, b, c, targetX, nextState););

/* State Machine */
MAKE_MACHINE(arm, tArmStates, armMoveHoldToTarget,
{
	case armMoveHoldToTarget: if (gPreserveArmTarget) gPreserveArmTarget = false; else currentArmPosition(gArmTarget); armToTargetAsync(false, true); break;
	case armIdle: setArm(0, 0); break;
})
