/* Types */
typedef struct _sCycleData {
	unsigned long period; // The target length of a cycle
	unsigned long startTime; // The program time that the cycle started
	unsigned long count; // The number of cycles that have finished
	unsigned long time; // The length of time the last cycle took
	string name; // The name of the cycle
} sCycleData;

/* Functions */
void initCycle(sCycleData& data, unsigned long period, const string name); // Do any operations required to start a cycle
void endCycle(sCycleData& data); // Do any operations required to end a cycle
