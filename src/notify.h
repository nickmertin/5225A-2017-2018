/* Types */
typedef struct _sNotifier {
	bool set;
	byte waiter;
	byte waitee;
	TSemaphore *semaphore;
} sNotifier;

/* Functions */
void reset(sNotifier& notifier);
byte waitOn(sNotifier& notifier, unsigned long timeout, const string description);
bool lock(sNotifier& notifier, TSemaphore *semaphore);
byte notify(sNotifier& notifier);
