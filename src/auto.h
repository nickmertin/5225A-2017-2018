/* Defines */
// The diameter of the tracking wheels in inches
#define WHEEL_DIAMETER_IN 4

// The distance between the tracking wheels and the centre of the robot in inches
#define DRIVE_WIDTH_IN 9 //The robot is 18 inches wide

// The number of tick per rotation of the tracking wheel
#define TICKS_PER_ROTATION 360.0

// Used internally by trackPosition
#define SPIN_TO_IN (WHEEL_DIAMETER_IN * PI / TICKS_PER_ROTATION)

/* Enumerations */
typedef enum _turnDir
{
	cw,
	ccw,
	ch
} tTurnDir;

/* Structures */
typedef struct _pos
{
    float a;
    float y;
    float x;
    int leftLst;
    int rightLst;
} sPos; // Position of the robot

typedef struct _vel
{
	float a;
	float y;
	float x;
	unsigned long lstChecked;
	float lstPosA;
	float lstPosY;
	float lstPosX;
} sVel; // Velocity of the robot

typedef struct _vector
{
	float y;
	float x;
} sVector; // 2D cartesian vector

typedef struct _polar
{
	float magnitude;
	float angle;
} sPolar; // 2D polar vector

typedef struct _line
{
	sVector p1;
	sVector p2;
} sLine;

/* Functions */
void trackPosition(int left, int right, sPos& position); // Update the position of the robot
void resetPosition(sPos& position); // Reset the position
void trackVelocity(sPos position, sVel& velocity); // Update the velocity of the robot
void vectorToPolar(sVector& vector, sPolar& polar); // Convert a cartesian vector to a polar vector
void polarToVector(sPolar& polar, sVector& vector); // Convert a polar vector to a cartesian vector
void transformVelocityToLocal(sVel& global, sVector& local, float angle); // Transform velocity from global to local with a given orientation
void transformVelocityToGlobal(sVel& local, sVector& global, float angle); // Transform velocity from local to global with a given orientation
float getAngleOfLine(sLine line);
float getLengthOfLine(sLine line);
task trackPositionTask();
task autoMotorSensorUpdateTask(); // Update motors and sensors during auto
task autoHitWallTask();
void applyHarshStop();
void resetPositionFullRad(sPos& position, float y, float x, float a);
void resetPositionFull(sPos& position, float y, float x, float a); // Reset the position to a desired value and starts tracking
void moveToTarget(float y, float x, byte power, float delta, float lineEpsilon = 1, float targetEpsilon = 1.5, bool harshStop = true, bool slow = true);
void moveToTarget(float y, float x, float ys, float xs, byte power, float delta, float lineEpsilon = 1, float targetEpsilon = 1.5, bool harshStop = true, bool slow = true);
void turnToAngleRad(float a, tTurnDir turnDir, byte left, byte right, bool harshStop = true, bool slow = true);
void turnToAngle(float a, tTurnDir turnDir, byte left, byte right, bool harshStop = true, bool slow = true);
void turnToTarget(float y, float x, tTurnDir turnDir, byte left, byte right, bool harshStop = true, bool slow = true, float offset = 0);
void turnToTarget(float y, float x, float ys, float xs, tTurnDir turnDir, byte left, byte right, bool harshStop = true, bool slow = true, float offset = 0);
float getTargetAngle(float y, float x, float ys, float xs);
void moveToTargetOrWall(float y, float x, float ys, float xs, byte power, bool harshStop = true, bool slow = true);
void moveToTargetOrWall(float y, float x, byte power, bool harshStop = true, bool slow = true);
float getDistanceFromPoint(sVector point);

/* Async Functions */
NEW_ASYNC_VOID_10(moveToTarget, float, float, float, float, byte, float, float, float, bool, bool)
NEW_ASYNC_VOID_6(turnToAngle, float, tTurnDir, byte, byte, bool, bool)
NEW_ASYNC_VOID_10(turnToTarget, float, float, float, float, tTurnDir, byte, byte, bool, bool, float)
NEW_ASYNC_VOID_7(moveToTargetOrWall, float, float, float, float, byte, bool, bool)

/* Internal Variables */
bool _autoNotHitWall = true;
bool _notReachedTarget = true;
sVector _distanceTarget;
float _distanceFromTarget = 0;

/* Variables */
bool gAutoSafety = true; // Whether the auto safety should run
unsigned long gAutoTime = 0;
sPos gPosition;
sVel gVelocity;
