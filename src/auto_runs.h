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
void autoSideMobileLeft();
void autoSideMobileRight();
void autoTest();

/* Variables */
int gCurAuto = 0;
tAlliance gAlliance = allianceBlue;
