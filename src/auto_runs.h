/* Enumerations */
typedef enum _tAlliance
{
	allianceBlue,
	allianceRed
} tAlliance;

/* Functions */
void selectAuto();
void runAuto();
void autoSkills();
void autoStationaryCore(bool first, int liftUp, int liftDown, tTurnDir turnDir);
void autoStationaryBlueLeft();
void autoStationaryRedRight();
void autoTest();

/* Variables */
int gCurAuto = 0;
tAlliance gAlliance = allianceBlue;
