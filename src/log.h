#pragma once
#include <string>
#include <time.h>
#include "util.h"

enum LOG_LEVEL
{
    DEFAULT,
    ERROR,
    INFO,
    DEBUG
};

namespace mylog
{
    class logger
    {
    public:
        logger(const char *log_name);
        void get_log_name(char *dst);
        static void current_time_str(char *dst, size_t size);
        static const int log_name_size = 128;

    protected:
        char log_name[log_name_size];
    };
}
