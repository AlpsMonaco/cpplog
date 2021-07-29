#include <fstream>
#include "util.h"
#include <stdio.h>
#include "stdlib.h"

void exit_with_msg(int exit_code, const char *msg)
{
	println(msg);
	exit(exit_code);
}

void sleep_ms(int ms)
{
#ifdef _WIN_ENVIROMENT
	Sleep(ms);

#else
	usleep(1000 * ms)
#endif
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
			strlib::strcpy(tmp, tmp_size, result);
			delete[] result;
			result = tmp;
		}

		strlib::strcat(result, tmp_size, buf);
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
		strlib::strcat(dst, dst_size, buf);
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

namespace strlib
{
	void strcpy(char *dst, const char *src)
	{
		int i = 0;
		while (true)
		{
			dst[i] = src[i];
			if (src[i] == '\0')
				return;
			i++;
		}
	}

	// src -> dst
	void strcpy(char *dst, int src_size, const char *src)
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

	void strcat(char *dst, const char *src)
	{
		int i = 0;
		while (dst[i] != '\0')
		{
			i++;
		}

		int j = 0;
		char tmp;
		while (true)
		{
			tmp = src[j];
			dst[i + j] = tmp;
			if (tmp == '\0')
				return;
			j++;
		}
	}

	bool strequ(const char *str1, const char *str2)
	{
		int i = 0;
		while (str1[i] != '\0')
		{
			if (str1[i] != str2[i])
				return false;
			i++;
		}

		return str2[i] == '\0';
	}

	int strlen(const char *src)
	{
		int len;
		for (len = 0; src[len] != '\0'; len++)
		{
		}

		return len;
	}
}
