void runAuto()
{
	//resetPositionFull(gPosition, 19.5, 44.5, 45);
	//moveToTarget(50, 50, 19.5, 44.5,127, 2 1, 1.5, true, true);
	setDrive(127,127)
	while (gSensor[leftEnc].value < 300) sleep(10);
	setDrive(0, 0);
}
