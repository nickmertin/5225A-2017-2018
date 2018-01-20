#pragma config(Sensor, dgtl1,  trackL,         sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  trackR,         sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  trackB,         sensorQuadEncoder)
#pragma config(Motor,  port3,           driveL1,       tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           driveL2,       tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           driveR2,       tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           driveR1,       tmotorVex393TurboSpeed_MC29, openLoop)

task main()
{
	clearDebugStream();
	motor[driveL1] = motor[driveL2] = -127;
	motor[driveR1] = motor[driveR2] = 127;
	sleep(1000);
	int L = SensorValue[trackL];
	int R = SensorValue[trackR];
	int B = SensorValue[trackB];
	sleep(100);
	int tL = (int)((SensorValue[trackL] - L) * 0.95);
	int tR = (int)((SensorValue[trackR] - R) * 0.95);
	int tB = (int)((SensorValue[trackB] - B) * 0.95);
	while (true)
	{
		int _L = SensorValue[trackL];
		int _R = SensorValue[trackR];
		int _B = SensorValue[trackB];
		if ((_L - L) * sgn(tL) < tL)
			writeDebugStreamLine("L %d %d", _L - L, tL);
		if ((_R - R) * sgn(tR) < tR)
			writeDebugStreamLine("R %d %d", _R - R, tR);
		if ((_B - B) * sgn(tB) < tB)
			writeDebugStreamLine("B %d %d", _B - B, tB);
		L = _L;
		R = _R;
		B = _B;
		sleep(100);
	}
}
