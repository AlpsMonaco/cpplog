#pragma once
#include <map>
#include <thread>
#include <atomic>

// use this when size of dst is larger than src surely.
template <typename T>
void copy_container(T src, int src_size, T dst)
{
	for (int i = 0; i < src_size; i++)
	{
		dst[i] = src[i];
	}
}

// copy all src to dst when size of dst is bigger than src.
// copy only size of dst from src to dst when size of dst is smaller than src.
template <typename T>
void copy_container(T src, int src_size, T dst, int dst_size)
{
	int size = dst_size > src_size ? src_size : dst_size;
	copy_container(src, size, dst);
}

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
	void put_msg(int msg_enum, void *msg);
	void set_max_thread_num(int num);

	static const int default_queue_size = 100;
	static const int default_max_thread_num = 5;

protected:
	int queue_size;
	int max_thread_num;

	std::atomic_int queue_pos;
	std::atomic_int queue_handled_pos;
	std::atomic_int current_thread_num;

	msg_payload *queue;
	std::map<int, msgcb> cb_map;

	void new_queue_size(int size);
	msgcb get_cb(int msg_enum);
	void producer_method();
	void consumer_method(int pos);
	int get_pos();
	int get_handled_pos();
	int get_unfinished_num();
	int get_next_queue_pos();
	int get_next_handle_pos();
};
