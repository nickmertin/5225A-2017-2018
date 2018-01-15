/* Functions */
void timeoutWhileEqual(void *val1, void *val2, unsigned int size, unsigned long timeout);
void timeoutWhileNotEqual(void *val1, void *val2, unsigned int size, unsigned long timeout);
void timeoutWhileTrue(bool *val, unsigned long timeout);
void timeoutWhileFalse(bool *val, unsigned long timeout);


/* Async Functions */
NEW_ASYNC_VOID_4(timeoutWhileEqual, void *, void *, unsigned int, unsigned long);
NEW_ASYNC_VOID_4(timeoutWhileNotEqual, void *, void *, unsigned int, unsigned long);
NEW_ASYNC_VOID_2(timeoutWhileTrue, bool *, unsigned long);
NEW_ASYNC_VOID_2(timeoutWhileFalse, bool *, unsigned long);
