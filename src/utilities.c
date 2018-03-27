float fmod(float x, float y)
{
	int q = floor(x / y);
	return x - (float)q * y;
}

float degToRad(float degrees)
{
	return degrees * PI / 180;
}

float radToDeg(float radians)
{
	return radians * 180 / PI;
}

float nearAngle(float angle, float reference)
{
	return round((reference - angle) / (2 * PI)) * (2 * PI) + angle;
}

void stopAllButCurrentTasks()
{
	// DO NOT MAKE A FOR LOOP, WILL NOT WORK
	tHog();
	STOP_TASK_NOT_CUR(1);
	STOP_TASK_NOT_CUR(2);
	STOP_TASK_NOT_CUR(3);
	STOP_TASK_NOT_CUR(4);
	STOP_TASK_NOT_CUR(5);
	STOP_TASK_NOT_CUR(6);
	STOP_TASK_NOT_CUR(7);
	STOP_TASK_NOT_CUR(8);
	STOP_TASK_NOT_CUR(9);
	STOP_TASK_NOT_CUR(10);
	STOP_TASK_NOT_CUR(11);
	STOP_TASK_NOT_CUR(12);
	STOP_TASK_NOT_CUR(13);
	STOP_TASK_NOT_CUR(14);
	STOP_TASK_NOT_CUR(15);
	STOP_TASK_NOT_CUR(16);
	STOP_TASK_NOT_CUR(17);
	STOP_TASK_NOT_CUR(18);
	STOP_TASK_NOT_CUR(19);
	STOP_TASK_NOT_CUR(20);
	tRelease();
}

#define STS(id) case id: startTask(id); break;
void startTaskID(word id)
{
	if (id == nCurrentTask)
		_hogLevel = 0;
	switch (id)
	{
		case 0:
			writeDebugStreamLine("WARNING, MAIN TASK BEING STARTED AGAIN, ABORTING");
			break;
		STS(1)
		STS(2)
		STS(3)
		STS(4)
		STS(5)
		STS(6)
		STS(7)
		STS(8)
		STS(9)
		STS(10)
		STS(11)
		STS(12)
		STS(13)
		STS(14)
		STS(15)
		STS(16)
		STS(17)
		STS(18)
		STS(19)
		STS(20)
	}
}
#undef STS

#define STS(id) case id: stopTask(id); break;
void stopTaskID(word id)
{
	if (id == nCurrentTask)
		_hogLevel = 0;
	switch (id)
	{
		case 0:
			writeDebugStreamLine("WARNING, MAIN TASK BEING STOPPED, ABORTING");
			break;
		STS(1)
		STS(2)
		STS(3)
		STS(4)
		STS(5)
		STS(6)
		STS(7)
		STS(8)
		STS(9)
		STS(10)
		STS(11)
		STS(12)
		STS(13)
		STS(14)
		STS(15)
		STS(16)
		STS(17)
		STS(18)
		STS(19)
		STS(20)
	}
}
#undef STS
