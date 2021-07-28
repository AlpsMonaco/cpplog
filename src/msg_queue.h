#pragma once
#include "util.h"

typedef void (*msgcb)(void *);

class msg_queue
{
public:
	msg_queue();
	msg_queue(int cb_size, int queue_size);
	~msg_queue();

	int get_cb_size();
	int get_queue_size();

	static const int default_cb_size = 10;
	static const int default_queue_size = 100;
	static const int default_consumer_num = 3;

protected:
	int cb_size = 0;
	int queue_size = 0;
	msgcb *cb_list = nullptr;
	void **queue = nullptr;

	void new_cb_list(int size);
	void new_queue_size(int size);
};
