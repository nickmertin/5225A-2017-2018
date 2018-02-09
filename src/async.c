void asyncInit()
{
	for (int i = 0; i < TASK_POOL_SIZE; ++i)
		gAsyncTaskData[i].id = NULL;
}

void await(unsigned long unique, unsigned long timeout, const string description)
{
#if TASK_POOL_SIZE > 0
	tHog();
	byte i;
	for (i = 0; i < TASK_POOL_SIZE; ++i)
		if (gAsyncTaskData[i].unique == unique && tEls[threadPoolTask0 + i].parent != -1)
			goto found;
	tRelease();
	return;
found:
	tRelease();
	_awaitAsync(i, timeout, description);
#endif
}

void _awaitAsync(byte index, unsigned long timeout, const string description)
{
	if (index < TASK_POOL_SIZE)
		waitOn(gAsyncTaskData[index].notifier, timeout, description);
}

void _killAsync(byte index)
{
#if TASK_POOL_SIZE > 0
	if (index < TASK_POOL_SIZE)
	{
		tStop(threadPoolTask0 + index);
		notify(gAsyncTaskData[index].notifier);
	}
#endif
}

unsigned long _startAsync(byte *id, void *data)
{
#if TASK_POOL_SIZE > 0
	tHog();
	unsigned long unique = _asyncUnique++;
	for (int i = 0; i < TASK_POOL_SIZE; ++i)
	{
		if (tEls[threadPoolTask0 + i].parent == -1)
		{
			gAsyncTaskData[i].id = id;
			gAsyncTaskData[i].data = data;
			gAsyncTaskData[i].unique = unique;
			reset(gAsyncTaskData[i].notifier);
			tStart(threadPoolTask0 + i);
			tRelease();
			waitOn(gAsyncTaskData[i].notifier, nPgmTime + 100, "Start async task");
			reset(gAsyncTaskData[i].notifier);
			return unique;
		}
	}
	tRelease();
#endif
	writeDebugStreamLine("NO FREE TASK");
	return 0;
}
