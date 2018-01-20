#pragma config(Sensor, dgtl1,  trackL,         sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  trackR,         sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  trackB,         sensorQuadEncoder)

task main()
{
	while (true)
	{
		clearLCDLine(0);
		clearLCDLine(1);

		displayLCDNumber(0, 0, SensorValue[trackL]);
		displayLCDNumber(0, 8, SensorValue[trackR]);
		displayLCDNumber(1, 0, SensorValue[trackB]);

		if (nLCDButtons)
			SensorValue[trackL] = SensorValue[trackR] = SensorValue[trackB] = 0;

		sleep(20);
	}
}
