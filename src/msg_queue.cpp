#include "msg_queue.h"

msg_queue::msg_queue()
{
	new_cb_list(default_cb_size);
	new_queue_size(default_queue_size);
}

msg_queue::msg_queue(int cb_size, int queue_size)
{
	new_cb_list(default_cb_size);
	new_queue_size(default_queue_size);
}

msg_queue::~msg_queue()
{
	if (this->cb_list != nullptr)
		delete this->cb_list;
	if (this->queue != nullptr)
		delete this->queue;
}

void msg_queue::new_cb_list(int new_size)
{
	msgcb *tmp_cb_list = new msgcb[new_size];
	if (this->cb_list != nullptr)
	{
		copy_container(this->cb_list, this->cb_size, tmp_cb_list, new_size);

		delete this->cb_list;
		this->cb_list = nullptr;
	}

	this->cb_list = tmp_cb_list;
}

void msg_queue::new_queue_size(int new_size)
{
	void **new_queue = new void *[new_size];
	if (this->queue != nullptr)
	{
		copy_container(this->queue, this->cb_size, new_queue, new_size);

		delete this->queue;
		this->queue = nullptr;
	}

	this->queue = new_queue;
}
