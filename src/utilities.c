float fmod(float x, float y)
{
	int q = (int)(x / y);
	return x - (float)q * y;
}

float sq(float x)
{
	return x * x;
}

int sq(int x)
{
	return x * x;
}

float degToRad(float degrees)
{
	return degrees * PI / 180;
}

float radToDeg(float radians)
{
	return radians * 180 / PI;
}

void rotate(float& x, float& y, float radians)
{
	float c = cos(radians);
	float s = sin(radians);
	float _x = x * c - y * s;
	float _y = x * s + y * c;
	x = _x;
	y = _y;
}

void rotateDegrees(float& x, float& y, float degrees)
{
	float c = cosDegrees(degrees);
	float s = sinDegrees(degrees);
	float _x = x * c - y * s;
	float _y = x * s + y * c;
	x = _x;
	y = _y;
}

float stdDevFilteredMean(float* values, int count, float nStdDev)
{
	float mean = 0.0;
	for (int i = 0; i < count; ++i)
		mean += values[i];
	mean /= (float)count;
	float variance = 0;
	for (int i = 0; i < count; ++i)
	{
		float tmp = values[i] - mean;
		variance += tmp * tmp;
	}
	float limit = sqrt(variance / (float)count) * nStdDev;
	float total = 0.0;
	int valid = 0;
	for (int i = 0; i < count; ++i)
	{
		float v = values[i];
		if (fabs(v - mean) <= limit)
		{
			total += v;
			++valid;
		}
	}
	return total / (float)valid;
}

void stopAllButCurrentTasks()
{
	for (int i = 1; i < 20; ++i)
		if (i != nCurrentTask) stopTask(i);
}
