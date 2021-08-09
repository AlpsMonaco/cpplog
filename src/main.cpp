#include "log.h"

int main()
{
	mylog::log("info")->log("test");
	std::this_thread::sleep_for(std::chrono::seconds(10));
}