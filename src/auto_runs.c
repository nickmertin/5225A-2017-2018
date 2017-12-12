void runAuto()
{
  resetPositionFull(gPosition, 18, 42, 45);
  //moveToTarget(30, 67, gPosition.y, gPosition.x, 127, 1.5, 1, 1, true, true);
	moveToTargetAsync(44.3, 65.3, gPosition.y, gPosition.x, 127, 1.5, 1, 1, true, true);
	unsigned long driveTimeout = nPgmTime + 3500;
	setArm (127, 265);
	moveToTargetAwait(driveTimeout);
	setArm (-80, 245);
	closeClaw (false);
	sleep (200);

	moveToTarget(36, 56, -127, 1.5, 1, 1, true, true); //back up
	turnToAngle(135, cw, 60, -60, true, true);
	moveToTarget(31, 75, 127, 5, 1, 1, true, true);
	turnToAngle(50, ccw, 60, 60, true, true);
	//moveToTarget(40, 75, 127, 5, 1, 1, true, true);
	//moveToTarget(40, 74, 31, 60, 127, 4, 1, 1, true, true);
}
