typedef enum _turnDir
{
	cw,
	ccw
} tTurnDir;

float fmod(float x, float y)
{
	int q = (int)(x / y);
	return x - (float)q * y;
}

float calculateAngle(float cur, float tar, tTurnDir dir)
{
	float curM = fmod(cur, 360);
	float tarM = fmod(tar, 360);

	if (dir == cw)
	{
		if (tarM >= curM) return tarM - curM;
		else return 360 - (curM - tarM);
	}
	else
	{
		if (tarM <= curM) return curM - tarM;
		else return 360 - (tarM - curM);
	}
}

task main()
{
	clearDebugStream();

	for (float i = 350; i < 370.0; i += 0.1)
	{
		writeDebugStreamLine("%.2f %.2f", i, calculateAngle(i, 10, cw));
		sleep(5);
	}
}
