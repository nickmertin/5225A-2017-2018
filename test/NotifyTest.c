#include "../src/utilities.h"
#include "../src/notify.h"

#include "../src/utilities.c"
#include "../src/notify.c"

sNotifier notifier;
TSemaphore *semaphore = NULL;

task secondary()
{
	writeDebugStreamLine("Secondary start %d", nPgmTime);
	lock(notifier, semaphore);
	sleep(111);
	notify(notifier);
	writeDebugStreamLine("Secondary end %d", nPgmTime);
}

task main()
{
	clearDebugStream();
	writeDebugStreamLine("Code start %d", nPgmTime);
	startTask(secondary);
	waitOn(notifier, nPgmTime + 1000);
	writeDebugStreamLine("Notified %d", nPgmTime);
	TSemaphore _sem;
	semaphoreInitialize(_sem);
	semaphore = &_sem;
	startTask(secondary);
	waitOn(notifier, nPgmTime + 1000);
	writeDebugStreamLine("Notified %d", nPgmTime);
}
