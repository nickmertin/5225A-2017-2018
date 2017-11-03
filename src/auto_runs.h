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
void autoStationaryCore();
void autoStationaryBlueLeft();
void autoTest();

/* Variables */
int gCurAuto = 0;
tAlliance gAlliance = allianceBlue;
