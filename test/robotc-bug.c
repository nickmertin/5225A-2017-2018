#define TEST(x, y) \
if (true) \
{ \
	y \
}

task main()
{
	TEST(, return;)
}
