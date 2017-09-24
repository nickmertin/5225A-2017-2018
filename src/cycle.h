/* Defines */
#define CYCLE_MAX_TIME 10

/* Functions */
void startCycle(); // Do any operations required to start a cycle
void endCycle(); // Do any operations required to end a cycle

/* Variables */
unsigned long gCycleStartTime = 0; // The program time that the cycle started
unsigned long gCycleCount = 0; // The number of cycles that have finished
unsigned long gCycleTime = 0; // The length of time the last cycle took
