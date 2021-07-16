#include "util.h"
#include "log.h"

int main(int argc, char const *argv[])
{
	/* code */
	char a[20];
	self::log::current_time_str(a, 20);
	println(a);
}
