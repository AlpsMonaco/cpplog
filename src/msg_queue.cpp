#include "msg_queue.h"

msg_queue::msg_queue()
{
	new (this) msg_queue(default_queue_size);
}

msg_queue::msg_queue(int queue_size)
{
	this->queue_size = 0;
	this->queue_pos = 0;
	this->max_thread_num = default_max_thread_num;
	this->queue_handled_pos = 0;
	this->current_thread_num = 0;
	this->unfinished_num = 0;
	this->queue = nullptr;
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
	this->queue_size = new_size;
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

// put_msg put msg to container;
// return false when current unfinished tasks num is larget than max_thread_num + queue_size;
bool msg_queue::put_msg(int msg_enum, void *msg)
{
	if (get_unfinished_num() >= (max_thread_num + queue_size))
	{
		return false;
	}

	unfinished_num++;
	msg_payload p = {msg_enum, msg};
	queue[get_next_queue_pos()] = p;
	return true;
}

void msg_queue::producer_method()
{
	while (true)
	{
		if (get_unfinished_num() > 0)
		{
			if (current_thread_num < max_thread_num)
			{
				current_thread_num++;
				std::thread(&msg_queue::consumer_method, this).detach();
			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
}

void msg_queue::consumer_method()
{
	int pos = get_next_handle_pos();
	msg_payload p = this->queue[pos];
	msgcb f = get_cb(p.msg_enum);
	if (f != nullptr)
	{
		f(p.msg);
	}

	unfinished_num--;
	current_thread_num--;
}

void msg_queue::set_max_thread_num(int num)
{
	this->max_thread_num = num;
}

int msg_queue::get_next_queue_pos()
{
	int pos = queue_pos % queue_size;
	queue_pos++;
	return pos;
}

int msg_queue::get_next_handle_pos()
{
	int pos = queue_handled_pos++;
	return pos % queue_size;
}

int msg_queue::get_unfinished_num()
{
	int num = this->unfinished_num;
	return num;
}

void msg_queue::start()
{
	std::thread(&msg_queue::producer_method, this).detach();
}