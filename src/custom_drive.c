void updateDriveLookup()
{
	for (int x = -127; x <= 127; ++x)
	{
		float w;
		switch (gDriveAlg)
		{
			case driveRed:
			{
				float weight = exp(-0.1 * gDriveCurvature);
				w = (weight + exp(0.1 * (abs(x) - 127)) * (1 - weight));
				break;
			}
			case driveBlue:
				w = exp(0.001 * (gDriveCurvature * (abs(x) - 127)));
				break;
		}
		gDriveLookup[(ubyte)x] = round(w * x);
	}
}

sbyte lookupDrive(sbyte joy)
{
	return gDriveLookup[(ubyte)joy];
}
