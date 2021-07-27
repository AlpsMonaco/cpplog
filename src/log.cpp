#include "log.h"
#pragma warning(disable : 4996)

void mylog::logger::current_time_str(char *dst, size_t size)
{
    if (size < 20)
        return;

    time_t now = time(0);
    tm *ltm = localtime(&now);

    sprintf(dst, "%d-%d-%d %02d:%02d:%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
}

void mylog::logger::current_time_str(char *dst)
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    sprintf(dst, "%d-%d-%d %02d:%02d:%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
}

void mylog::logger::current_date(char *dst)
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    sprintf(dst, "%d-%d-%d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
}

// write dst with log time preffix
// dst requires at least size of 23(including \0)
// [2006-01-02 15:04:05]
void mylog::logger::log_time_preffix(char *dst)
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    sprintf(dst, "[%d-%d-%d %02d:%02d:%02d] ", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
}

mylog::logger::logger(const char *log_name)
{
    strlib::strcpy(this->log_name, log_name_size, log_name);
}

mylog::logger::~logger()
{
    clear_log_handle();
}

void mylog::logger::get_log_name(char *dst)
{
    strlib::strcpy(dst, this->log_name);
}

void mylog::logger::create_log_handle()
{
    current_date(log_date);

    strlib::strcpy(log_file_name, this->log_name);
    strlib::strcat(log_file_name, " ");
    strlib::strcat(log_file_name, log_date);
    strlib::strcat(log_file_name, ".log");

    std::ofstream *fd = new std::ofstream(log_file_name, std::ios::app);
    if (!fd->is_open())
    {
        clear_log_handle();
        return;
    }

    log_fd = fd;
}

bool mylog::logger::is_same_date()
{
    char tmp_date[log_date_size];
    return strlib::strequ(tmp_date, log_date);
}

void mylog::logger::clear_log_handle()
{
    strlib::strcpy(log_file_name, "");
    strlib::strcpy(log_date, "");
    close_log_handle();
}

void mylog::logger::write(const char *log)
{
    if (!is_same_date())
    {
        clear_log_handle();
        create_log_handle();
    }

    auto content = new char[log_time_preffix_size + strlib::strlen(log)];
    log_time_preffix(content);
    strlib::strcat(content, log);

    *log_fd << content << std::endl;
    delete[] content;
}

void mylog::logger::close_log_handle()
{
    if (this->log_fd != nullptr)
    {
        if (this->log_fd->is_open())
        {
            this->log_fd->close();
        }
    }
}

void mylog::logger::flush_log()
{
    if (this->log_fd != nullptr)
    {
        if (this->log_fd->is_open())
            log_fd->flush();
    }
}