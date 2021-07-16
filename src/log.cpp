#include "log.h"

std::string self::log::get_log_name()
{
    return log_name;
}

void self::log::current_time_str(char *dst, size_t size)
{
    if (size < 20)
        return;

    time_t now = time(0);
    tm *ltm = localtime(&now);

    sprintf(dst, "%d-%d-%d %02d:%02d:%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
}