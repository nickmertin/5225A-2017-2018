void updateTurnLookup()
{
	for (int x = -127; x <= 127; ++x)
	{
		float w;
		switch (gTurnAlg)
		{
			case turnRed:
			{
				float weight = exp(-0.1 * gTurnCurvature);
				w = (weight + exp(0.1 * (abs(x) - 127)) * (1 - weight));
				break;
			}
			case turnBlue:
				w = exp(0.001 * (gTurnCurvature * (abs(x) - 127)));
				break;
		}
		gTurnLookup[(ubyte)x] = round(w * x);
	}
}

sbyte lookupTurn(sbyte joy)
{
	return gTurnLookup[(ubyte)joy];
}
