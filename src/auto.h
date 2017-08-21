/* Defines */
#define AUTO_BREAK_POWER 25
#define MOVE_SHOOT_SLEW_RATE 30
#define TURN_DELTA_90 (PI * DRIVE_RADIUS / 2)
#define TURN_DELTA_45 (PI * DRIVE_RADIUS / 4)

/* Functions */
//void driveStraight(float &distance, sbyte power, bool safety, bool harshStop); // Drive straight for a given distance in inches, optionally applying safety or a harsh stop. Updates distance to distance travelled so far each cycle.
//void driveTurn(float leftDelta, float rightDelta, float range, sbyte limit, bool safety, bool harshStop); // Move either side a given distance in inches, optionally applying safety or a harsh stop.
task autoMotorSensorUpdateTask(); // Update motors and sensors during auto
task autoSafetyTask(); // Autonomous drive safety task
//void applyHarshStop();
void scalePower(float &leftPow, float &rightPow, sbyte max = 127);
//void waitForAccelStable(); // Blocks until the acceleration reaches 1 g and the jerk reaches 0 g/s

/* Async Functions */
//NEW_ASYNC_VOID_4(driveStraight, float *, distance, sbyte, power, bool, safety, bool, harshStop, driveStraight(*distance, power, safety, harshStop););
//NEW_ASYNC_VOID_6(driveTurn, float, leftDelta, float, rightDelta, float, range, sbyte, limit, bool, safety, bool, harshStop, driveTurn(leftDelta, rightDelta, range, limit, safety, harshStop););

/* Variables */
bool gAutoSafety = true; // Whether the auto safety should run
unsigned long gAutoTime = 0;
unsigned long gAutoStartTime = 0;
