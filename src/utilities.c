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

int solveCubic(float a, float b, float c, float d, float* sln)
{
	float p = -b / (3 * a);
	float q = p * p * p + (b * c - 3 * a * d) / (6 * a * a);
	float r = c / (3 * a);
	float p2 = p * p;
	float q2 = q * q;
	float r_m_p2 = r - p2;
	float s = sqrt(fabs(q2 + r_m_p2 * r_m_p2 * r_m_p2));
	sln[0] = pow(fabs(q + s), 1.0 / 3.0) * SGN_OF_DIFF(q, -s) + pow(fabs(q - s), 1.0 / 3.0) * SGN_OF_DIFF(q, -s) + p;
	float b_2 = a * sln[0] + b;
	float c_2 = b_2 * sln[0] + c;
	float dscm = b_2 * b_2 - 4 * a * c_2;
	int count = sgn(dscm) + 2;
	if (count == 2)
	{
		sln[1] = -b_2 / (2 * a);
	}
	else if (count == 3)
	{
		float sqrt_dscm = sqrt(dscm);
		float _den = 2 * a;
		sln[1] = (sqrt_dscm - b_2) / _den;
		sln[2] = (-sqrt_dscm - b_2) / _den;
	}
	return count;
}

void stopAllButCurrentTasks()
{
	for (int i = 1; i < 20; ++i)
		if (i != nCurrentTask) stopTask(i);
}
