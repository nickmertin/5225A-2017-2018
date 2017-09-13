/* Defines */
#define INTAKE_IN_POWER 127
#define INTAKE_OUT_POWER -127

/* Types */
typedef enum _tIntakeStates
{
	intakeIdle,
	intakeIn,
	intakeOut
} tIntakeStates;

/* Functions */
void setIntake(word power);
void handleIntake();

/* State Machine */
MAKE_MACHINE(intake, tIntakeStates, intakeIdle,
{
	case intakeIdle: setIntake(0); break;
	case intakeIn: setIntake(INTAKE_IN_POWER); break;
	case intakeOut: setIntake(INTAKE_OUT_POWER); break;
})
