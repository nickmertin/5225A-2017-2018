/* Types */
typedef enum _turnAlg
{
	turnRed,
	turnBlue
} tTurnAlg;

/* Functions */
void updateTurnLookup();
sbyte lookupTurn(sbyte joy);

/* Variables */
tTurnAlg gTurnAlg = turnRed;
int gTurnCurvature = 5;
sbyte gTurnLookup[256];
int gTurnCurveLim = 0;

/* Defines */
#define TURN_CURVE_MIN 0
#define TURN_CURVE_MAX 20