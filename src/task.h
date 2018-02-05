typedef struct s_taskElement
{
	word next;
	word prev;
	word child;
	word parent;
} taskElement;

taskElement tEls[kNumbOfTasks + 1];

#define return_t { tUnreg(nCurrentTask); return; }

unsigned char _hogLevel = 0;

void tInit();
void tStart(word id, bool detached = false);
void tStop(word id);
void tStopAll(word id);
void tStopRoot();
void tUnreg(word id);
word tGetRoot(word id);
void tHog();
void tRelease();
