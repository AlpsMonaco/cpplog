#include <fstream>
#include "util.h"


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
	int read_size;
	
	while (!ifs.eof()) {
		read_size = ifs.read(buf, buf_size).gcount();
		buf[read_size] = 0;

		if (result == nullptr) {
			int total_size = read_size + 1;
			auto temp_result = new char[total_size];
			strcpy_s(temp_result, total_size, buf);
			result = temp_result;
		}
		else {
			int total_size = strlen(result) + read_size + 1;
			auto temp_result = new char[total_size];
			strcpy_s(temp_result, strlen(result) + 1, result);
			delete(result);
			strcat_s(temp_result, total_size, buf);
			result = temp_result;
		}
	}

	// destruct
	ifs.close();
	delete[] buf;
	return result;
}

void append_file(const char* file_path, const char* content)
{
	std::ofstream ofs;
	ofs.open(file_path, std::ios::app);
	ofs << content << std::endl;
	ofs.close();
}
