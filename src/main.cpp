#include "log.h"

int main(int argc, char **argv)
{
	mylog::logger infoLog;
	infoLog.log("hello world");
}