#include <iostream>
#include "util.h"
using namespace std;

/* 
msg_queue.register(func_ptr,args...)
msg_queue.do(func_ptr,args...)
*/

struct table
{
	int id;
	float score;
};

void a(table *t)
{
	cout << t->id << endl;
	cout << t->score << endl;
}

void b(void *param)
{
	auto t = (table *)param;
	a(t);
}

class msg_oprate
{
public:
	// accept params.
	virtual void accept(void *param_ptr) = 0;
	// perform.
	virtual void perform() = 0;
};

class print_table : msg_oprate
{
public:
	table *t;
	void accept(void *param_ptr)
	{
		this->t = (table *)param_ptr;
	}

	void perform()
	{
		a(t);
	}
};

int main(int argc, char **argv)
{
	const int size_1 = 20;
	char s1[size_1] = "hello world";

	const int size_2 = 20;
	char s2[size_2] = "hello world2";

	char *tmp = new char[50];
	copy_container(s1, size_1, tmp, 50);

	println(tmp);
}
