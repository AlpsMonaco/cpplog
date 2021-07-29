<<<<<<< HEAD
#include "util.h"
#include "msg_queue.h"
=======
﻿#include "util.h"
#include <map>
#include <thread>

>>>>>>> 4d6dd42917887b6143d8723cf16985164e1b370f

void p(void *s)
{
	println((char *)s);
}

class cTest
{
public:
	cTest(){};
	void publicPrint()
	{
		println("this is public");
	}

	void test()
	{
		std::thread t(&cTest::protectedPrint, this);
		t.detach();
	};

protected:
	void protectedPrint()
	{
		println("this is protected");
	}
};

int main(int argc, char **argv)
{
	// auto a = new cTest();
	// a->test();
	// auto n = new test();
	// n->tetetet();

	msg_queue *mq = new msg_queue();
	mq->register_msg(1, p);
	mq->start();

	char s[] = "hello";
	void *p = s;

	mq->put_msg(1, p);

	std::this_thread::sleep_for(std::chrono::seconds(10));
	return 0;
}