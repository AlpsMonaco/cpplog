#pragma once
#include <fstream>
#include <string>
#include <time.h>
#include "util.h"

namespace mylog
{
    class logger
    {
    public:
        logger(const char *log_name);
        ~logger();
        void get_log_name(char *dst);
        void write(const char *log);

        static void current_time_str(char *dst, size_t size);
        static void current_time_str(char *dst);
        static void current_date(char *dst);
        static void log_time_preffix(char *dst);

        static const int log_name_size = 128;
        static const int log_date_size = 11;
        static const int log_time_size = 20;
        static const int log_time_preffix_size = 23;

    protected:
        char log_name[log_name_size];
        char log_file_name[log_name_size + log_date_size];
        char log_date[log_date_size];
        std::ofstream *log_fd;

        void create_log_handle();
        void clear_log_handle();
        bool is_same_date();
        void close_log_handle();
        void flush_log();
        };
}
