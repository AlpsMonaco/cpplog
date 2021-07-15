#pragma once
#include <iostream>

template <typename T>
void print(T t) {
	std::cout << t;
}

template <typename T>
void println(T t) {
	std::cout << t << std::endl;
}


void exit_with_msg(int exit_code, const char* msg);
const char* read_file(const char* file_path, int buf_size = 1024);
void append_file(const char* file_path, const char* content);
const char* win_exec(const char* cmd);
const char* exec(const char* cmd);