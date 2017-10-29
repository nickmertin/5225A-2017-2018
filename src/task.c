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
		hogCPU();
		tEls[id].parent = nCurrentTask;
		tEls[id].next = tEls[nCurrentTask].child;
		if (tEls[nCurrentTask].child != -1) tEls[tEls[nCurrentTask].child].prev = id;
		tEls[nCurrentTask].child = id;
		releaseCPU();
	}
	startTaskID(id);
}

void tStop(word id)
{
	hogCPU();
	tUnreg(id);
	stopTaskID(id);
	releaseCPU();
}

void tStopAll(word id)
{
	bool stopCur = false;
	hogCPU();
	while (tEls[id].child != -1)
	{
		if (tEls[id].child == nCurrentTask)
		{
			stopCur = true;
			tUnreg(tEls[id].child);
		}
		else tStop(tEls[id].child);
	}
	tStop(id);
	if (stopCur) stopTaskID(nCurrentTask);
	releaseCPU();
}

void tStopRoot()
{
	tStopAll(tGetRoot(nCurrentTask));
}

void tUnreg(word id)
{
	if (tEls[id].next != -1)
		tEls[tEls[id].next].prev = tEls[id].prev;
	if (tEls[id].prev != -1)
		tEls[tEls[id].prev].next = tEls[id].next;
	if (tEls[id].parent != -1 && tEls[tEls[id].parent].child == id)
		tEls[tEls[id].parent].child = tEls[id].next;

	word ce = tEls[id].child, le = -1;
	while (ce != -1)
	{
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
}

word tGetRoot(word id)
{
	while (tEls[id].parent != -1) id = tEls[id].parent;
	return id;
}
