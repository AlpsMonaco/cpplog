#include "msg_queue.h"

msg_queue::msg_queue()
{
	new (this) msg_queue(default_queue_size);
}

msg_queue::msg_queue(int queue_size)
{
	new_queue_size(default_queue_size);
}

msg_queue::~msg_queue()
{
	if (this->queue != nullptr)
		delete this->queue;
}

void msg_queue::new_queue_size(int new_size)
{
	msg_payload *new_queue = new msg_payload[new_size];
	if (this->queue != nullptr)
	{
		copy_container(this->queue, this->queue_size, new_queue, new_size);

		delete this->queue;
		this->queue = nullptr;
	}

	this->queue = new_queue;
}

void msg_queue::register_msg(int msg_enum, msgcb cb)
{
	this->cb_map[msg_enum] = cb;
}

msgcb msg_queue::get_cb(int msg_enum)
{
	auto it = this->cb_map.find(msg_enum);
	return it == cb_map.end() ? nullptr : it->second;
}

void msg_queue::put_msg(int msg_enum, void *msg)
{
	msg_payload p = {msg_enum, msg};
	queue[queue_pos++] = p;
}

void msg_queue::producer_method()
{
	while (true)
	{
	}
}

void msg_queue::set_max_thread_num(int num)
{
	this->max_thread_num = num;
}