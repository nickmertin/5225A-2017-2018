/* Functions */
void timeoutWhileEqual(void *val1, void *val2, unsigned int size, unsigned long timeout, const unsigned char *routine, unsigned short id);
void timeoutWhileNotEqual(void *val1, void *val2, unsigned int size, unsigned long timeout, const unsigned char *routine, unsigned short id);
void timeoutWhileLessThanS(short *val, short threshold, unsigned long timeout, const unsigned char *routine, unsigned short id);
void timeoutWhileGreaterThanS(short *val, short threshold, unsigned long timeout, const unsigned char *routine, unsigned short id);
void timeoutWhileLessThanL(long *val, long threshold, unsigned long timeout, const unsigned char *routine, unsigned short id);
void timeoutWhileGreaterThanL(long *val, long threshold, unsigned long timeout, const unsigned char *routine, unsigned short id);
void timeoutWhileLessThanF(float *val, float threshold, unsigned long timeout, const unsigned char *routine, unsigned short id);
void timeoutWhileGreaterThanF(float *val, float threshold, unsigned long timeout, const unsigned char *routine, unsigned short id);
void timeoutWhileTrue(bool *val, unsigned long timeout, const unsigned char *routine, unsigned short id);
void timeoutWhileFalse(bool *val, unsigned long timeout, const unsigned char *routine, unsigned short id);
