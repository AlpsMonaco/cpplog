#include "log.h"

int main(int argc, char **argv)
{
	auto infoLog = new mylog::logger("info");
	auto errLog = new mylog::logger("err");

	infoLog->log("info1");
	errLog->log("err1");
	infoLog->log("info2");
	errLog->log("err2");

	delete infoLog;
	delete errLog;

	return 0;
}