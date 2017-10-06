task test2()
{
}

task test()
{
	sleep(1000);
	writeDebugStreamLine("%d", getTaskPriority(test2));
	startTask(test2);
	sleep(100);
	writeDebugStreamLine("%d", getTaskPriority(test2));
	stopAllTasks();
	while(true);
}

task main()
{
	writeDebugStreamLine("%d", getTaskPriority(test));
	startTask(test);
	sleep(100);
	writeDebugStreamLine("%d", getTaskPriority(test));
	sleep(2000);
	stopAllTasks();
}
