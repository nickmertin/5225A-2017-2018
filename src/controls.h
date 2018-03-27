/* Drive */
#define JOY_TURN Ch4
#define DZ_TURN 15
#define JOY_THROTTLE Ch3
#define DZ_THROTTLE 15

/* Lift */
#define JOY_LIFT_DRIVER Ch2
#define JOY_LIFT_PARTNER Ch2Xmtr2
#define DZ_LIFT 70

/* Arm */
#define JOY_ARM_DRIVER Ch1
#define JOY_ARM_PARTNER Ch1Xmtr2
#define DZ_ARM 70

/* Mobile */
#define BTN_MOBILE_TOGGLE Btn6U
#define BTN_MOBILE_MIDDLE Btn6D

/* Macros */
#define BTN_MACRO_ZERO Btn7U
#define BTN_MACRO_ZERO_PARTNER Btn7UXmtr2
#define BTN_MACRO_STACK Btn8D
#define BTN_MACRO_LOADER Btn7D
#define BTN_MACRO_PREP Btn7R
#define BTN_MACRO_WALL Btn7L
#define BTN_MACRO_STATIONARY Btn8R
#define BTN_MACRO_PRELOAD (gSensor[jmpSkills].value ? Btn8R : Btn7D)
#define BTN_MACRO_PICKUP (gSensor[jmpSkills].value ? Btn7L : Btn8L)
#define BTN_MACRO_CANCEL Btn8U
#define BTN_MACRO_INC Btn5U
#define BTN_MACRO_DEC Btn5D

//Skills only Macros
#define BTN_MACRO_TILT Btn8L
#define BTN_MACRO_STACK_NO_PU Btn8R
