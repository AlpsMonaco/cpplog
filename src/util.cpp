#include <fstream>
#include "util.h"
#include <stdio.h>
#include "stdlib.h"


void exit_with_msg(int exit_code, const char* msg) {
	println(msg);
	exit(exit_code);
}

// 带缓冲读取文件
const char* read_file(const char* file_path,int buf_size)
{
	const char* result = nullptr;

	std::ifstream ifs;
	ifs.open(file_path);

	if (!ifs.is_open()) { return result; }
	char* buf = new char[buf_size + 1];
	int read_size = 1;
	int get_count;
	
	while (!ifs.eof()) {
		get_count = ifs.read(buf, buf_size).gcount();
		buf[get_count] = 0;
		read_size += get_count;

		auto temp_result = new char[read_size];
		if (result != nullptr) {
			strcpy_s(temp_result, read_size, result);
			delete(result);
			strcat_s(temp_result, read_size, buf);
		}
		else {
			strcpy_s(temp_result, read_size, buf);
		}
		result = temp_result;
	}

	// destruct
	ifs.close();
	delete[] buf;
	buf = nullptr;
	return result;
}

void append_file(const char* file_path, const char* content)
{
	std::ofstream ofs;
	ofs.open(file_path, std::ios::app);
	ofs << content << std::endl;
	ofs.close();
}

const char* exec(const char* cmd)
{
	const char* result = nullptr;

	const int buf_size = 255;
	char buf[buf_size];
	int count = 1;
	
	auto fd = _popen(cmd, "r");
	if (!fd) return result;
	
	while (fgets(buf, buf_size, fd) != NULL) {
		count += strlen(buf);
		auto temp_result = new char[count];

		if (result != nullptr) {
			strcpy_s(temp_result, count, result);
			delete(result);
			strcat_s(temp_result, count, buf);
		}else {
			strcpy_s(temp_result, count, buf);
		}

		result = temp_result;
	}

	return result;
}
