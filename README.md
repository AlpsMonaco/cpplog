# cpplog

## usage
simply use:
```
#include "log.h"

int main(int argc, char **argv)
{
	int i = 0;
	char tempChar[20];
	for (;;)
	{
		i++;
		sprintf(tempChar, "info count %d", i);
		mylog::log("info")->log(tempChar);

		sprintf(tempChar, "err count %d", i);
		mylog::log("err")->log(tempChar);

		sprintf(tempChar, "test count %d", i);
		mylog::log("test")->log(tempChar);

		std::this_thread::sleep_for(std::chrono::seconds(60));
	}
}

```
