#include "../src/motors.h"

#include "../src/motors.c"

task main()
{
	sleep(5000);
	for (int i = 0; i < 10000; ++i)
		updateMotors();
	writeDebugStreamLine("%d", nPgmTime - 5000);
}
