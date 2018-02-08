void reset(sNotifier& notifier)
{
	notifier.set = false;
	notifier.waiter = notifier.waitee = -1;
	notifier.semaphore = NULL;
}

byte waitOn(sNotifier& notifier, unsigned long timeout, const string description)
{
	//notifier.set = false;
	notifier.waiter = nCurrentTask;
	notifier.waitee = -1;
	while (!notifier.set && !TimedOut(timeout, description))
	{
		if (notifier.semaphore)
		{
			semaphoreLock(*notifier.semaphore, MIN(timeout - nPgmTime, 0x7FFF));
			semaphoreUnlock(*notifier.semaphore);
		}
		else sleep(10);
	}
	if (notifier.set)
		return notifier.waitee;
	writeDebugStreamLine("Notifier on task %d timed out", nCurrentTask);
	return -1;
}

bool lock(sNotifier& notifier, TSemaphore *semaphore)
{
	tHog();
	if (notifier.semaphore || !semaphore) return false;
	if (semaphore->nLockCount == 0)
	{
		semaphoreLock(*semaphore);
		notifier.semaphore = semaphore;
		tRelease();
		return true;
	}
	tRelease();
	return false;
}

byte notify(sNotifier& notifier)
{
	notifier.waitee = nCurrentTask;
	notifier.set = true;
	if (notifier.semaphore && bDoesTaskOwnSemaphore((*notifier.semaphore))) semaphoreUnlock(*notifier.semaphore);
	return notifier.waiter;
}
