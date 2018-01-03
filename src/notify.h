/* Types */
typedef struct _sNotifier {
	bool set;
	byte waiter;
	byte waitee;
	TSemaphore *semaphore;
} sNotifier;

/* Functions */
byte waitOn(sNotifier& notifier, unsigned long timeout, const string description);
bool lock(sNotifier& notifier, TSemaphore *semaphore);
byte notify(sNotifier& notifier);
