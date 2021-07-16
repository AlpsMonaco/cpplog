#include <iostream>
#include <fstream>
#include "util.h"
#include "demo.h"
#include <thread>
#include <vector>
#include <ctime>
#include <windows.h>

using namespace std;

namespace demo
{
	const char *demo_file_path = "test.txt";

	void write_file()
	{
		ofstream ofs;
		ofs.open(demo_file_path, ios::app);
		ofs << "this data is written by operator overload <<" << endl;

		const char *content = "this data is written by write method\n";
		ofs.write(content, strlen(content));
		ofs.close();
	}

	void read_file()
	{
		println("implement on util.cpp,do not provide demo anymore");
	}

	void multithread()
	{
		std::vector<std::thread> v;
		for (int i = 0; i < 10; i++)
		{
			v.push_back(
				std::thread([i]()
							{
								this_thread::sleep_for(chrono::seconds(i));
								std::cout << i << std::endl;
							}));
		}

		for (unsigned int i = 0; i < v.size(); i++)
		{
			println(v[i].get_id());
			v[i].join();
		}
	}

	void time_demo()
	{
		// 基于当前系统的当前日期/时间
		time_t now = time(0);

		cout << "1970 到目前经过秒数:" << now << endl;

		tm *ltm = localtime(&now);

		// 输出 tm 结构的各个组成部分
		cout << "年: " << 1900 + ltm->tm_year << endl;
		cout << "月: " << 1 + ltm->tm_mon << endl;
		cout << "日: " << ltm->tm_mday << endl;
		cout << "时间: " << ltm->tm_hour << ":";
		cout << ltm->tm_min << ":";
		cout << ltm->tm_sec << endl;
	}

}