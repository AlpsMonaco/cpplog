#include <iostream>
#include "util.h"
#include "demo.h"

void test(int& a){
	a = 1;
}


int main()
{
	const char* a = nullptr;
	char b[20] = "hello";

	a = b;
	println(a);
	b[1] = 'c';
	println(a);

}


