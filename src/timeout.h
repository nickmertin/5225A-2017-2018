/* Functions */
void timeoutWhileEqual(void *val1, void *val2, unsigned int size, unsigned long timeout, const string description);
void timeoutWhileNotEqual(void *val1, void *val2, unsigned int size, unsigned long timeout, const string description);
void timeoutWhileLessThanS(short *val, short threshold, unsigned long timeout, const string description);
void timeoutWhileGreaterThanS(short *val, short threshold, unsigned long timeout, const string description);
void timeoutWhileLessThanL(long *val, long threshold, unsigned long timeout, const string description);
void timeoutWhileGreaterThanL(long *val, long threshold, unsigned long timeout, const string description);
void timeoutWhileLessThanF(float *val, float threshold, unsigned long timeout, const string description);
void timeoutWhileGreaterThanF(float *val, float threshold, unsigned long timeout, const string description);
void timeoutWhileTrue(bool *val, unsigned long timeout, const string description);
void timeoutWhileFalse(bool *val, unsigned long timeout, const string description);
