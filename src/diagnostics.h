/* Types */
typedef enum _lcdScreen
{
	lcdMain,
	lcdAutoCones,
	lcdAutoStart,
	lcdBattery,
	lcdTracking,
	lcdJumpCount,
	lcdLightSensors,
	lcdLiftTest,
	lcdSkillsTest,
	lcdTurnAlg,
	lcdTurnCurve,
	lcdTurnCurveLim,
	lcdDriveAlg,
	lcdDriveCurve,
	kNumbLcdScreens
} tLcdScreen;

typedef enum _lcdButtons
{
	btnNone = 0b000,
	btnLeft = 0b001,
	btnCenter = 0b010,
	btnRight = 0b100
} tLcdButtons;

/* Functions */
void handleLcd();

/* Variables */
tLcdScreen gLcdScreen = lcdMain;
tLcdButtons gLastLcdButtons = btnNone;

/* Defines */
#define LCD_RISING(button) ((buttons & button) && !(gLastLcdButtons & button))
