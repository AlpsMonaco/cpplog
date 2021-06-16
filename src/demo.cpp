#include <iostream>
#include <fstream>
#include "util.h"
#include "demo.h"
#include <thread>
#include <vector>
#include <ctime>
#include <windows.h>

using namespace std;

namespace demo {
	const char* demo_file_path = "test.txt";

	void write_file() {
		ofstream ofs;
		ofs.open(demo_file_path, ios::app);
		ofs << "this data is written by operator overload <<" << endl;

		const char* content = "this data is written by write method\n";
		ofs.write(content, strlen(content));
		ofs.close();
	}

	void read_file() {
		println("implement on util.cpp,do not provide demo anymore");
	}


	void multithread() {
		std::vector<std::thread> v;
		for (int i = 0; i < 10; i++) {
			v.push_back(
				std::thread([i]() {this_thread::sleep_for(chrono::seconds(i)); std::cout << i << std::endl; }));
		}

		for (unsigned int i = 0; i < v.size(); i++) {
			println(v[i].get_id());
			v[i].join();
		}
	}

	void time_demo()
	{
		auto now = time(0);
		const int buf_size = 20;
		char buf[buf_size];

		tm t;
		localtime_s(&t, &now);
		strftime(buf, buf_size, "%Y-%m-%d %T", &t);
		println(buf);
		Sleep(2000);
		strftime(buf, buf_size, "%Y-%m-%d %T", &t);
		println(buf);
	}

}