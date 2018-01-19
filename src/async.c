void asyncInit()
{
	for (int i = 0; i < TASK_POOL_SIZE; ++i)
		gAsyncTaskData[i].id = NULL;
}

void await(byte index, unsigned long timeout, const string description)
{
	if (index < TASK_POOL_SIZE)
		waitOn(gAsyncTaskData[index].notifier, timeout, description);
}

void kill(byte index)
{
#if TASK_POOL_SIZE > 0
	if (index < TASK_POOL_SIZE)
	{
		tStop(threadPoolTask0 + index);
		notify(gAsyncTaskData[index].notifier);
	}
#endif
}

byte _startAsync(byte *id, void *data)
{
#if TASK_POOL_SIZE > 0
	for (int i = 0; i < TASK_POOL_SIZE; ++i)
	{
		if (tEls[threadPoolTask0 + i].parent == -1)
		{
			gAsyncTaskData[i].id = id;
			gAsyncTaskData[i].data = data;
			reset(gAsyncTaskData[i].notifier);
			tStart(threadPoolTask0 + i);
			waitOn(gAsyncTaskData[i].notifier, nPgmTime + 100, "Start async task");
			reset(gAsyncTaskData[i].notifier);
			return i;
		}
	}
#endif
	return 255;
}
