#include "log.h"

int main(int argc, char **argv)
{
	auto log = new mylog::logger("test");
	log->write("test1");
	log->write("test2");
	log->write("test3");
	log->write("test4");
	log->write("test5");
	log->write("test6");
	log->write("test7");
	log->write("test8");
	log->write("test9");
	log->write("test10");
	log->write("test11");
	log->write("test12");
	log->write("test13");
	delete log;
}