#pragma once
#include <iostream>

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

template <typename T>
void print(T t)
{
	std::cout << t;
}

template <typename T>
void println(T t)
{
	std::cout << t << std::endl;
}

void exit_with_msg(int exit_code, const char *msg);
const char *read_file(const char *file_path, int buf_size = 1024);
void read_file(const char *file_path, char *dst, int dst_size, int buf_size = 1024);
void append_file(const char *file_path, const char *content);
const char *win_exec(const char *cmd);
// const char* exec(const char* cmd);

namespace strlib
{
	void strcpy(char *dst, int src_size, const char *src);
	void strcpy(char *dst, const char *src);
	void strcat(char *dst, int dst_size, const char *src);
	void strcat(char *dst, const char *src);
	bool strequ(const char *str1, const char *str2);
	int strlen(const char *src);
}