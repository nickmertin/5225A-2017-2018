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
void auto20RedLeft();
void auto20RedRight();
void auto20BlueRight();
void auto20BlueLeft();
void auto5RedLeft();
void auto5RedRight();
void auto5BlueRight();
void auto5BlueLeft();
void autoStationaryLeft5();
void autoStationaryRight5();
void autoStationaryLeft2();
void autoStationaryRight2();
void autoTest();

/* Variables */
int gCurAuto = 0;
tAlliance gAlliance = allianceBlue;
