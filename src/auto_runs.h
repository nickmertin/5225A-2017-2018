/* Enumerations */
typedef enum _tAlliance
{
	allianceBlue,
	allianceRed
} tAlliance;

/* Functions */
void selectAuto();
void runAuto();
void driverSkillsStart();
void autoSkills();
<<<<<<< Updated upstream
void autoStationaryCore(bool first, int liftUp, int liftDown, tTurnDir turnDir);
void autoStationaryBlueLeft();
void autoStationaryRedRight();
void autoSideMobileLeft();
void autoSideMobileRight();
=======
void auto20RedLeft();
void auto20RedRight();
void auto20BlueRight();
void auto20BlueLeft();
>>>>>>> Stashed changes
void autoTest();

/* Variables */
int gCurAuto = 0;
tAlliance gAlliance = allianceBlue;
