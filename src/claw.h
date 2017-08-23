/* Defines */
#define CLAW_SERVO_OPEN -70
#define CLAW_SERVO_CLOSED 75

/* Types */
typedef enum _tClawStates {
	clawOpen,
	clawClosed,
} tClawStates;

/* Variables */
sbyte gClawTarget;

/* Functions */
void setClaw(sbyte target);
void clawToTarget();

/* Async Functions */
NEW_ASYNC_VOID(clawToTarget, clawToTarget(););

/* State Machine */
MAKE_MACHINE(claw, tClawStates, clawOpen,
{
	case clawOpen: gClawTarget = CLAW_SERVO_OPEN; clawToTargetAsync(); break;
	case clawClosed: gClawTarget = CLAW_SERVO_CLOSED; clawToTargetAsync(); break;
})
