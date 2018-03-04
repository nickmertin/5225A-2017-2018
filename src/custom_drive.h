/* Types */
typedef enum _driveAlg
{
	driveRed,
	driveBlue
} tDriveAlg;

/* Functions */
void updateDriveLookup();
sbyte lookupDrive(sbyte joy);

/* Variables */
tDriveAlg gDriveAlg = driveRed;
int gDriveCurvature = 0;
sbyte gDriveLookup[256];
int gDriveCurveLim = 0;

/* Defines */
#define DRIVE_CURVE_MIN 0
#define DRIVE_CURVE_MAX 20