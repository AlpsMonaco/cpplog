#pragma once
#include <string>
#include <time.h>

enum LOG_LEVEL
{
    DEFAULT,
    ERROR,
    INFO,
    DEBUG
};

namespace self
{
    class log
    {
    public:
        log(const char *log_name)
        {
            this->log_name = log_name;
        }

        log(std::string log_name)
        {
            this->log_name = log_name;
        }

        std::string get_log_name();
        static void current_time_str(char *dst, size_t size);

    protected:
        std::string log_name;
    };
}
