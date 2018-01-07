void runAuto()
{
	//1 cone on mogo
	resetPositionFull(gPosition, 18, 42, 45);
	//moveToTarget(30, 67, gPosition.y, gPosition.x, 127, 1.5, 1, 1, true, true);
	moveToTargetAsync(44.3, 65.3, gPosition.y, gPosition.x, 127, 1.5, 1, 1, true, true);
	unsigned long driveTimeout = nPgmTime + 3500;
	liftToPos (1); //power 127
	moveToTargetAwait(driveTimeout);
	liftToPos (0, 130); //power -80, position -245
	openClaw (true);
	sleep (600);

	//2 put mogo in scoring zone
	moveToTargetSimple (49, 69, 127, 0, stopSoft | stopHarsh, true);
	liftToPos (-1);
	turnToAngleSimple (340, cw, 100, 100);
	int distance = gSensor[driveLeft].value + 500;
	driveTimeout = nPgmTime + 2700;
	writeDebugStreamLine ("distance is %d", distance);
	setDrive (90, 90);
	while (gSensor[driveLeft].value < distance && nPgmTime < driveTimeout) sleep(10);
	setDrive (0,0);
	writeDebugStreamLine ("drove to %d", gSensor[leftEnc].value);
	liftToPos (1, -60);

	//3 shake and back up from mogo
	resetPositionFull(gPosition, 10, 60, 180);
	for (byte x = 0; x < 2; x++)
	{
		//right shake
		distance = gSensor[rightEnc].value - 80;
		setDrive (0, -127);
		while (gSensor[rightEnc].value > distance) sleep(10);
		setDrive(0,0);
		//left shake
		distance = gSensor[leftEnc].value - 80;
		setDrive (-127, 0);
		while (gSensor[leftEnc].value > distance) sleep(10);
		setDrive(0,0);
	}
	//back up
	moveToTargetSimple (40, 60, -80, 0, stopSoft | stopHarsh, true);

	//4 go to mogo
	turnToAngle(0, ccw, 127, 127, true, true);


}
