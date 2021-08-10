# cpplog

## A simple async log written in c++.Integrated with msg_queue.

### Introduction
A lite self-used log lib.Require msg_queue lib which is a submodule in /src(https://github.com/AlpsMonaco/msg_queue)  


### Usage
```
#include "log.h"

int main(int argc,char** argv)
{
	//Assume current date is 2021-08-10.
	
	mylog::log("debug")->log("log() will both print to stdout and log to debug 2021-08-10.log file");
	mylog::log("debug")->write("write() will only log to debug 2021-08-10.log file without print to stdout");

	std::this_thread::sleep_for(std::chrono::seconds(10));
}

```


### Document
This lib uses mylog::logmgr to handle log operations globally.  
The construct function of mylog::logmgr is protected,could only be constructed by mylog::logmgr::get_ins().  
It is a singleton and lazy loads so normally users do not need to call any methods of mylog::logmgr.  
