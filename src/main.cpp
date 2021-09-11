#include "log.h"
#include <thread>

int main(int argc, char **argv)
{
	int i = 0;
	char tempChar1[20];
	char tempChar2[20];
	char tempChar3[20];
	for (;;)
	{
		i++;
		sprintf(tempChar1, "info count %d", i);
		mylog::log("info")->log(tempChar1);

		sprintf(tempChar2, "err count %d", i);
		mylog::log("err")->log(tempChar2);

		sprintf(tempChar3, "test count %d", i);
		mylog::log("test")->log(tempChar3);

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}