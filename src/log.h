#pragma once
#include <fstream>
#include <string>
#include <map>
#include <time.h>
#include "util.h"

namespace mylog
{
    class logger
    {
    public:
        logger();
        logger(const char *log_name);
        ~logger();
        void write(const char *log);
        void log(const char *log);
        void format_log(char *dst, const char *log);
        void flush_log();
        void set_log_name(const char *log_name);

        // 2006-01-02 15:04:05
        static void current_time_str(char *dst);

        // 2006-01-02
        static void current_date(char *dst);

        /*[2006-01-02 15:04:05] */
        // there's a space char in the end of the log_time_preffix.
        static void log_time_preffix(char *dst);

        static const int log_name_size = 128;
        static const int log_time_size = 20;
        static const int log_date_size = 11;
        static const int log_time_preffix_size = 23;

    protected:
        char log_name[log_name_size] = "log";

        // 4 -> strlen(".log")
        char log_file_name[log_name_size + log_date_size + 4] = "";
        char log_date[log_date_size] = "";
        std::ofstream *log_fd = nullptr;

        void create_log_handle();
        void clear_log_handle();
        bool is_same_date();
        void close_log_handle();
    };

    class logmgr
    {
    private:
        static logmgr *ins;

    public:
        static logmgr *get_ins()
        {
            if (ins == nullptr)
            {
                ins = new logmgr();
            }

            return ins;
        }

        logger *get_logger(const char *log_name);

    protected:
        logmgr();

        std::map<const char *, logger *, strlib::charPtrComparator> m;
    };

    logger *log(const char *log_name);
}