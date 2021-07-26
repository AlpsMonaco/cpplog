#include "log.h"

void mylog::logger::current_time_str(char *dst, size_t size)
{
    if (size < 20)
        return;

    time_t now = time(0);
    tm *ltm = localtime(&now);

    sprintf(dst, "%d-%d-%d %02d:%02d:%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
}

mylog::logger::logger(const char *log_name)
{
    strlib::strcpy(log_name, log_name_size, this->log_name);
}

void mylog::logger::get_log_name(char *dst)
{
    strlib::strcpy(this->log_name, log_name_size, dst);
}