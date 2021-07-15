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
	std::ifstream ifs;
	ifs.open(file_path);

	if (!ifs.is_open()) { return nullptr; }

	std::string str_buf;
	if (buf_size < 1) buf_size = 256;

	char* buf = new char[buf_size];
	int read_size;

	using namespace std;
	while (!ifs.eof()) {
		read_size = ifs.read(buf, buf_size-1).gcount();
		buf[read_size] = 0;
		str_buf += buf;
	}

	buf_size = str_buf.size();
	auto temp_char = new char[buf_size];
	temp_char[buf_size] = '\0';
	str_buf.copy(temp_char, buf_size, 0);

	return temp_char;
}

void append_file(const char* file_path, const char* content)
{
	std::ofstream ofs;
	ofs.open(file_path, std::ios::app);
	ofs << content << std::endl;
	ofs.close();
}

const char* win_exec(const char* cmd)
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

	_pclose(fd);
	return result;
}

const char* exec(const char* cmd) {
#ifdef _WIN32 || _WIN64
	return win_exec(cmd);
#endif // _WIN32 || _WIN64
	return nullptr;
}