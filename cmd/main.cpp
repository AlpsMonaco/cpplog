#include "../log.h"
#include <thread>

int main(int argc, char **argv)
{
	printf("With LogManager mode \n");

	int count = 0;
	char *buf = (char *)malloc(128);

	for (;;)
	{
		count++;
		sprintf(buf, "this is test round %d", count);
		logutil::Name("test")->Print(buf);
		sprintf(buf, "this is info round %d", count);
		logutil::Name("info")->Print(buf);
		sprintf(buf, "this is error round %d", count);
		logutil::Name("error")->Print(buf);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		if (count == 10000000)
		{
			break;
		}
	}

	free(buf);
}