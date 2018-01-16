/* Functions */
void timeoutWhileEqual(void *val1, void *val2, unsigned int size, unsigned long timeout);
void timeoutWhileNotEqual(void *val1, void *val2, unsigned int size, unsigned long timeout);
void timeoutWhileLessThanS(short *val, short threshold, unsigned long timeout);
void timeoutWhileGreaterThanS(short *val, short threshold, unsigned long timeout);
void timeoutWhileLessThanL(long *val, long threshold, unsigned long timeout);
void timeoutWhileGreaterThanL(long *val, long threshold, unsigned long timeout);
void timeoutWhileLessThanF(float *val, float threshold, unsigned long timeout);
void timeoutWhileGreaterThanF(float *val, float threshold, unsigned long timeout);
void timeoutWhileTrue(bool *val, unsigned long timeout);
void timeoutWhileFalse(bool *val, unsigned long timeout);


/* Async Functions */
NEW_ASYNC_VOID_4(timeoutWhileEqual, void *, void *, unsigned int, unsigned long);
NEW_ASYNC_VOID_4(timeoutWhileNotEqual, void *, void *, unsigned int, unsigned long);
NEW_ASYNC_VOID_3(timeoutWhileLessThanS, short *, short, unsigned long);
NEW_ASYNC_VOID_3(timeoutWhileGreaterThanS, short *, short, unsigned long);
NEW_ASYNC_VOID_3(timeoutWhileLessThanL, long *, long, unsigned long);
NEW_ASYNC_VOID_3(timeoutWhileGreaterThanL, long *, long, unsigned long);
NEW_ASYNC_VOID_3(timeoutWhileLessThanF, float *, float, unsigned long);
NEW_ASYNC_VOID_3(timeoutWhileGreaterThanF, float *, float, unsigned long);
NEW_ASYNC_VOID_2(timeoutWhileTrue, bool *, unsigned long);
NEW_ASYNC_VOID_2(timeoutWhileFalse, bool *, unsigned long);
