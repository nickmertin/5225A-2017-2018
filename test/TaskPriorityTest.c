task test()
{
	while(true);
}

task main()
{
	writeDebugStreamLine("%d", getTaskPriority(test));
	startTask(test);
	sleep(100);
	writeDebugStreamLine("%d", getTaskPriority(test));
	stopAllTasks();
}
