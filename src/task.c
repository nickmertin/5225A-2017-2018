void tInit()
{
	for (word i = 0; i <= kNumbOfTasks; ++i)
	{
		tEls[i].child = -1;
		tEls[i].prev = -1;
		tEls[i].parent = -1;
		tEls[i].next = -1;
	}
}

void tStart(word id, bool detached)
{
	if (!detached)
	{
		tHog();
		tEls[id].parent = nCurrentTask;
		tEls[id].next = tEls[nCurrentTask].child;
		if (tEls[nCurrentTask].child != -1) tEls[tEls[nCurrentTask].child].prev = id;
		tEls[nCurrentTask].child = id;
		tRelease();
	}
	startTaskID(id);
}

void tStop(word id)
{
	tHog();
	tUnreg(id);
	stopTaskID(id);
	tRelease();
}

void tStopAll(word id, bool notMe)
{
	bool stopCur = false;
	tHog();
	bool updated[kNumbOfTasks];
	memset(updated, 0, kNumbOfTasks);
	while (tEls[id].child != -1 && !updated[tEls[id].child])
	{
		updated[tEls[id].child] = true;
		if (tEls[id].child == nCurrentTask)
		{
			stopCur = !notMe;
			tUnreg(tEls[id].child);
		}
		else tStop(tEls[id].child);
	}
	tStop(id);
	if (stopCur)
	{
		tRelease();
		stopTaskID(nCurrentTask);
	}
	tRelease();
}

void tUnreg(word id)
{
	tHog();
	if (tEls[id].next != -1)
		tEls[tEls[id].next].prev = tEls[id].prev;
	if (tEls[id].prev != -1)
		tEls[tEls[id].prev].next = tEls[id].next;
	if (tEls[id].parent != -1 && tEls[tEls[id].parent].child == id)
		tEls[tEls[id].parent].child = tEls[id].next;

	bool updated[kNumbOfTasks];
	memset(updated, 0, kNumbOfTasks);
	word ce = tEls[id].child, le = -1;
	while (ce != -1 && !updated[ce])
	{
		updated[ce] = true;
		tEls[ce].parent = tEls[id].parent;
		le = ce;
		ce = tEls[ce].next;
	}
	if (le != -1 && tEls[id].parent != -1)
	{
		tEls[le].next = tEls[tEls[id].parent].child;
		tEls[tEls[tEls[id].parent].child].prev = le;
		tEls[tEls[id].parent].child = tEls[id].child;
	}

	tEls[id].child = -1;
	tEls[id].prev = -1;
	tEls[id].parent = -1;
	tEls[id].next = -1;
	tRelease();
}

void tHog()
{
	if (!_hogLevel++)
		hogCPU();
}

void tRelease()
{
	if (_hogLevel && !--_hogLevel)
		releaseCPU();
}
