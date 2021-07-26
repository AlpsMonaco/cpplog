#include <fstream>
#include "util.h"
#include <stdio.h>
#include "stdlib.h"

void exit_with_msg(int exit_code, const char *msg)
{
	println(msg);
	exit(exit_code);
}

// 带缓冲读取文件
const char *read_file(const char *file_path, int buf_size)
{
	std::ifstream ifs;
	ifs.open(file_path);

	if (!ifs.is_open())
	{
		return nullptr;
	}

	if (buf_size < 1)
		buf_size = 256;

	int read_size;
	int read_time = 0;
	char *buf = new char[buf_size];
	char *result = nullptr;

	while (!ifs.eof())
	{
		read_time++;
		read_size = ifs.read(buf, buf_size - 1).gcount();
		buf[read_size] = '\0';
		auto tmp_size = buf_size * read_time;
		if (result == nullptr)
		{
			result = new char[tmp_size];
			result[0] = '\0';
		}
		else
		{
			auto tmp = new char[tmp_size];
			pseudolib::strcpy(result, tmp_size, tmp);
			delete[] result;
			result = tmp;
		}

		pseudolib::strcat(result, tmp_size, buf);
	}

	return result;
}

void read_file(const char *file_path, char *dst, int dst_size, int buf_size)
{
	std::ifstream ifs;
	ifs.open(file_path);

	if (!ifs.is_open())
	{
		return;
	}

	if (buf_size < 1)
		buf_size = 256;

	int read_size;
	char *buf = new char[buf_size];

	dst[0] = '\0';
	while (!ifs.eof())
	{
		read_size = ifs.read(buf, buf_size - 1).gcount();
		buf[read_size] = '\0';
		pseudolib::strcat(dst, dst_size, buf);
	}

	delete[] buf;
}

void append_file(const char *file_path, const char *content)
{
	std::ofstream ofs;
	ofs.open(file_path, std::ios::app);
	ofs << content << std::endl;
	ofs.close();
}

namespace pseudolib
{
	// src -> dst
	void strcpy(const char *src, int src_size, char *dst)
	{
		for (int i = 0; i < src_size; i++)
		{
			dst[i] = src[i];
			if (dst[i] == '\0')
				break;
		}
	}

	// dst + src
	void strcat(char *dst, int dst_size, const char *src)
	{
		for (int i = 0; i < dst_size; i++)
		{
			if (dst[i] == '\0')
			{
				int j = 0;
				do
				{
					dst[i] = src[j];
					j++;
					if (src[j] == '\0')
					{
						return;
					}
				} while (++i < dst_size);
			}
		}
	}
}
