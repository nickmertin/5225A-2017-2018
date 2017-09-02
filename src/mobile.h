/* Defines */
#define MOBILE_UP_POSITION 2800
#define MOBILE_DOWN_POSITION 500
#define MOBILE_HOLD_UP_POWER 5
#define MOBILE_HOLD_DOWN_POWER -5
#define MOBILE_RAISE_POWER 127
#define MOBILE_LOWER_POWER -127

/* Types */
typedef enum _tMobileStates {
	mobileIdle,
	mobileLowering,
	mobileRaising,
	mobileDown,
	mobileUp
} tMobileStates;

/* Functions */
void setMobile(sbyte value);
void handleMobile();

/* State Machine */
MAKE_MACHINE(mobile, tMobileStates, mobileIdle,
{
	case mobileIdle: setMobile(0); break;
	case mobileLowering: setMobile(MOBILE_LOWER_POWER); break;
	case mobileRaising: setMobile(MOBILE_RAISE_POWER); break;
	case mobileDown: setMobile(MOBILE_HOLD_DOWN_POWER); break;
	case mobileUp: setMobile(MOBILE_HOLD_UP_POWER); break;
})
