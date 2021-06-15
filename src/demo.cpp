#include <iostream>
#include <fstream>
#include "util.h"
#include "demo.h"
#include <thread>
#include <vector>

using namespace std;

namespace demo {
	const char* demo_file_path = "test.txt";

	void write_file() {
	ofstream ofs;
	ofs.open(demo_file_path,ios::app);
	ofs << "this data is written by operator overload <<"<<endl;
	
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
				std::thread([i](){this_thread::sleep_for(chrono::seconds(i)); std::cout << i << std::endl; }));
		}

		for (unsigned int i = 0; i < v.size(); i++) {
			println(v[i].get_id());
			v[i].join();
		}
	}

	void test() {
		auto v =new std::vector<int>;
		v->push_back(1);
		delete(v);
	}
}

