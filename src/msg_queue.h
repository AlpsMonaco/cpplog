#pragma once
#include "util.h"
#include <map>
#include <thread>
#include <atomic>

typedef void (*msgcb)(void *);

struct msg_payload
{
	int msg_enum;
	void *msg;
};

class msg_queue
{
public:
	msg_queue();
	msg_queue(int queue_size);
	~msg_queue();

	int get_queue_size();
	void start();
	void stop();
	void register_msg(int msg_enum, msgcb cb);
	bool put_msg(int msg_enum, void *msg);
	void set_max_thread_num(int num);
	int get_unfinished_num();

	static const int default_queue_size = 100;
	static const int default_max_thread_num = 5;

protected:
	int queue_size = 0;
	int queue_pos = 0;
	int max_thread_num = default_max_thread_num;
	std::atomic_int queue_handled_pos = 0;
	std::atomic_int current_thread_num = 0;
	std::atomic_int unfinished_num = 0;

	msg_payload *queue = nullptr;
	std::map<int, msgcb> cb_map;

	void new_queue_size(int size);
	msgcb get_cb(int msg_enum);
	void producer();
	void producer_method();
	void consumer_method();
	int get_next_queue_pos();
	int get_next_handle_pos();
	int get_current_handled_pos();
};
