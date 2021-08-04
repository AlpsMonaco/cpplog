
#include "util.h"
#include "msg_queue/msg_queue.h"

#include "util.h"
#include <map>
#include <thread>

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
	mq->set_max_thread_num(1);
	mq->start();

	void *p;

	for (int i = 0; i < 100; i++)
	{
		char *tmp = new char[20];
		sprintf(tmp, "this is %d", i);
		mq->put_msg(1, (void *)tmp);
		// sleep_ms(i);
	}

	std::this_thread::sleep_for(std::chrono::seconds(10));
	return 0;
}