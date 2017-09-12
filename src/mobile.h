/* Defines */
#define MOBILE_UP_POSITION 2500
#define MOBILE_DOWN_POSITION 400
#define MOBILE_HALFWAY_POSITION 1500
#define MOBILE_HOLD_UP_POWER 5
#define MOBILE_HOLD_DOWN_POWER -12
#define MOBILE_RAISE_POWER 127
#define MOBILE_LOWER_POWER -127

/* Types */
typedef enum _tMobileStates
{
	mobileIdle,
	mobileLowering,
	mobileRaising,
	mobileDown,
	mobileUp,
	mobileToHalfway
} tMobileStates;

/* Functions */
void setMobile(word value);
void handleMobile();
void holdMobileToTarget(int target);

/* Async Functions */
NEW_ASYNC_VOID_1(holdMobileToTarget, int, target, holdMobileToTarget(target););

/* State Machine */
MAKE_MACHINE(mobile, tMobileStates, mobileUp,
{
	case mobileIdle: setMobile(0); break;
	case mobileLowering: setMobile(MOBILE_LOWER_POWER); break;
	case mobileRaising: setMobile(MOBILE_RAISE_POWER); break;
	case mobileDown: setMobile(MOBILE_HOLD_DOWN_POWER); break;
	case mobileUp: setMobile(MOBILE_HOLD_UP_POWER); break;
	case mobileToHalfway: holdMobileToTargetAsync(MOBILE_HALFWAY_POSITION); break;
})
