#define TEST(name, args) TEST_##name args
#define TEST_PRINT(format, arg) writeDebugStreamLine(format, arg)

task main()
{
	TEST(PRINT, ("%s", "hello"));
}
