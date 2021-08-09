#pragma once
#include <iostream>

#ifdef _WIN32
#define _WIN_ENVIROMENT 1
#include <Windows.h>
#else
#include <unistd.h>
#include <cstring>
#endif

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
void sleep_ms(int ms);

namespace strlib
{
	void strcpy(char *dst, int src_size, const char *src);
	void strcpy(char *dst, const char *src);
	void strcat(char *dst, int dst_size, const char *src);
	void strcat(char *dst, const char *src);
	bool strequ(const char *str1, const char *str2);
	int strlen(const char *src);

	struct charPtrComparator
	{
		bool operator()(const char *a, const char *b) const
		{
			return strcmp(a, b) < 0;
		}
	};

}