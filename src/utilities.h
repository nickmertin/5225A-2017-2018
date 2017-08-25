/* Defines */
// Limit a variable to a value
#define LIM_TO_VAL(input, val) abs(input) > val ? val * sgn(input) : input

// Limit a variable to a value and set that variable to the result
#define LIM_TO_VAL_SET(input, val) input = LIM_TO_VAL(input, val)

// The length of an array
#define ARR_LEN(array) (sizeof(array) / sizeof(array[0]))

// Swaps any integral type
#define SWAP(x, y) { x = x ^ y; y = x ^ y; x = x ^ y; }

// Checks the bit in a bitmap
#define CHK_BIT(bit, map) ((map & bit) == bit)

#define MIN(x, y) (x < y ? x : y)

#define MAX(x, y) (x > y ? x : y)

#define NORMAL_RAD(a) (fmod(a + PI, PI * 2) - PI)

#define SGN_OF_DIFF(x, y) ((x > y) - (x < y))

/* Functions */
float fmod(float x, float y); // Floating point mod operation
float sq(float x); // Square a value
int sq(int x); // Square a value
float degToRad(float degrees); // Convert degrees to radians
float radToDeg(float radians); // Convert radians to degrees
void rotate(float& x, float& y, float radians); // Rotate a vector by an angle
void rotateDegrees(float& x, float& y, float degrees); // Rotate a vector by an angle
float stdDevFilteredMean(float* values, int count, float nStdDev); // Get the average of the values that are within nStdDev standard deviations
void stopAllButCurrentTasks();
