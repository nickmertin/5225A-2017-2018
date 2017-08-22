/* Defines */
#define DEG_PER_POT 18.2
#define BEAM_R 16
#define BEAM_S 16
#define CAL_BASE 180
#define A_VERTICAL 2668
#define B_VERTICAL 3051

/* Types */
typedef struct _sArmPos {
	float x;
	float y;
} sArmPos;

void calculatePosition(sArmPos& position, float a, float b);
float calculateTargetA(float x, float y);
float calculateTargetB(float x, float y);
float potiToDeg(short poti, short cal);
float degToPoti(float deg, short cal);
void setArm(sbyte powerA, sbyte powerB);
void setArmF(float powerA, float powerB);
bool armToTarget(sArmPos& target, bool safety, bool hold);
