float fmod(float x, float y)
{
	int q = (int)(x / y);
	return x - (float)q * y;
}

float sq(float x)
{
	return x * x;
}

int sq(int x)
{
	return x * x;
}

float degToRad(float degrees)
{
	return degrees * PI / 180;
}

float radToDeg(float radians)
{
	return radians * 180 / PI;
}

void stopAllButCurrentTasks()
{
	// DO NOT MAKE A FOR LOOP, WILL NOT WORK
	hogCPU();
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
	releaseCPU();
}

#define STS(id) case id: startTask(id); break;
void startTaskID(word id)
{
	switch (id)
	{
		STS(0)
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
	switch (id)
	{
		STS(0)
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
