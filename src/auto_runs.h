/* Enumerations */
typedef enum _tAlliance
{
	allianceBlue,
	allianceRed
} tAlliance;

/* Functions */
void selectAuto();
void runAuto();
void autoSkills(int segment = -1);
void killAuto(unsigned long timeout);

#if SKILLS_ROUTE == 0

void autoBlock();
void auto20Left(int cones);
void auto20Right(int cones);
void auto5Left(int cones);
void auto5Right(int cones);
void autoSBLeft(bool secondCone, bool boomKapow);
void autoSBRight(bool secondCone, bool boomKapow);
void autoSideBlockLeft(int cones);
void autoSideBlockRight(int cones);

#elif SKILLS_ROUTE < 0

void autoTest();

#endif

NEW_ASYNC_VOID_1(killAuto, unsigned long);

/* Variables */
int gCurAuto = 0;
tAlliance gAlliance = allianceBlue;
bool gAutoLocked = false;
int gAutoCones = 3;
bool gAutoStartSkills = false;

/* Defines */
#if SKILLS_ROUTE == 0
#define AUTO_OPTIONS_COUNT 11
#elif SKILLS_ROUTE == 1
#define AUTO_OPTIONS_COUNT 1
#define SKILLS_1_SAFE (gAlliance == allianceRed)
#else
#define AUTO_OPTIONS_COUNT 0
#endif
