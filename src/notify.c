byte waitOn(sNotifier& notifier, unsigned long timeout)
{
	notifier.set = false;
	notifier.waiter = nCurrentTask;
	notifier.waitee = -1;
	while (!notifier.set && nPgmTime < timeout)
	{
		if (notifier.semaphore)
		{
			semaphoreLock(*notifier.semaphore, MIN(timeout - nPgmTime, 0x7FFF));
			semaphoreUnlock(*notifier.semaphore);
		}
		else sleep(10);
	}
	return notifier.set ? notifier.waitee : -1;
}

bool lock(sNotifier& notifier, TSemaphore *semaphore)
{
	if (notifier.semaphore || !semaphore) return false;
	notifier.semaphore = semaphore;
	if (semaphore->nLockCount == 0 || bDoesTaskOwnSemaphore((*semaphore)))
	{
		semaphoreLock(*semaphore);
		return true;
	}
	return false;
}

byte notify(sNotifier& notifier)
{
	notifier.waitee = nCurrentTask;
	notifier.set = true;
	if (notifier.semaphore && bDoesTaskOwnSemaphore((*notifier.semaphore))) semaphoreUnlock(*notifier.semaphore);
	return notifier.waiter;
}
