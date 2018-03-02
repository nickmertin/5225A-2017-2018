/* Functions */
void timeoutWhileEqual(void *val1, void *val2, unsigned int size, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true);
void timeoutWhileNotEqual(void *val1, void *val2, unsigned int size, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true);
void timeoutWhileLessThanS(short *val, short threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true);
void timeoutWhileGreaterThanS(short *val, short threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true);
void timeoutWhileLessThanL(tSensors sen, long threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill);
void timeoutWhileGreaterThanL(long *val, long threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true);
void timeoutWhileLessThanF(float *val, float threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true);
void timeoutWhileGreaterThanF(float *val, float threshold, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true);
void timeoutWhileTrue(bool *val, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true);
void timeoutWhileFalse(bool *val, unsigned long timeout, const unsigned char *routine, unsigned short id, bool kill = true);
