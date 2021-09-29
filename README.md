# cpplog

## A simple async log written in c++.Integrated with MsgQueue.

### Introduction
A simple log library,controlled with MsgQueue.


### Usage
```
#include "log.h"
#include <thread>

int main(int argc,char** argv)
{
	//Assume current date is 2021-08-10.
	
	logutil::Name("debug")->Print("Print() will both print to stdout and log to debug 2021-08-10.log file");
	logutil::Name("debug")->Log("Log() will only log to debug 2021-08-10.log file without print to stdout");

	std::this_thread::sleep_for(std::chrono::seconds(10));

	// also use:
	auto l = new logutil::Logger("test");
	l->Print("something");
	l->Log("something");
	delete l;

}

```


### Document
logutil::Name("log") is thread safe,dispatched by a msg queue in LogManager.Check log.cpp for more detail.
But remember logutil::Logger::Log is not thread safe;