float foo(float x)
{
	return x * x;
}

unsigned long bar(float a, int b, unsigned long c, float d)
{
	EndTimeSlice();
	return (unsigned long) (a * b * c * d)
}

task main()
{
	byte data[13];
	unsigned long result;
	result = bar(foo(12), 1, 2, 3);
}
