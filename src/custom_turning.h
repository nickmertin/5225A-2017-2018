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
int gTurnCurvature = 7;
sbyte gTurnLookup[256];
int gTurnCurveLim = 15;
bool gTurnIgnoreJumper = false;

/* Defines */
#define TURN_CURVE_MIN 0
#define TURN_CURVE_MAX 20
