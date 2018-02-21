/* Drive */
#define JOY_TURN Ch4
#define DZ_TURN 15
#define JOY_THROTTLE Ch3
#define DZ_THROTTLE 15

/* Lift */
#define JOY_LIFT_GAME Ch2
#define JOY_LIFT_SKILLS Ch2Xmtr2
#define JOY_LIFT (gSensor[jmpSkills].value ? JOY_LIFT_SKILLS : JOY_LIFT_GAME)
#define DZ_LIFT 70

/* Arm */
#define JOY_ARM_GAME Ch1
#define JOY_ARM_SKILLS Ch1Xmtr2
#define JOY_ARM (gSensor[jmpSkills].value ? JOY_ARM_SKILLS : JOY_ARM_GAME)
#define DZ_ARM 70

/* Mobile */
#define BTN_MOBILE_TOGGLE Btn6U
#define BTN_MOBILE_MIDDLE Btn6D

/* Macros */
#define BTN_MACRO_ZERO Btn7U
#define BTN_MACRO_STACK Btn8D
#define BTN_MACRO_LOADER Btn7D
#define BTN_MACRO_PREP Btn7R
#define BTN_MACRO_STATIONARY (gSensor[jmpSkills].value ? Btn7D : Btn8R)
#define BTN_MACRO_PRELOAD (gSensor[jmpSkills].value ? Btn8R : Btn7D)
#define BTN_MACRO_PICKUP Btn8L
#define BTN_MACRO_CANCEL Btn8U
#define BTN_MACRO_INC Btn5U
#define BTN_MACRO_DEC Btn5D
