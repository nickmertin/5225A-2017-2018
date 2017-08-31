/* Defines */
#define LIFT_HOLD_POWER 5
#define LIFT_LOWER_POWER -127
#define LIFT_RAISE_POWER 127

/* Types */
typedef enum _tLiftStates
{
	liftIdle,
	liftHolding,
	liftLowering,
	liftRaising
} tLiftStates;

/* Functions */
void setLift(sbyte power);
void handleLift();

/* State Machine */
MAKE_MACHINE(lift, tLiftStates, liftHolding,
{
	case liftIdle: setLift(0); break;
	case liftHolding: setLift(LIFT_HOLD_POWER); break;
	case liftLowering: setLift(LIFT_LOWER_POWER); break;
	case liftRaising: setLift(LIFT_RAISE_POWER); break;
})
