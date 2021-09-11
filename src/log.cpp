#include "log.h"
#include "time.h"
#include "msg_queue.h"
#include <fstream>
#include <iostream>
#pragma warning(disable : 4996)

#define LogFileHandlePtr ((std::ofstream *)p)
#define LogFileHandle *(std::ofstream *)p

template <typename T>
void Println(T t)
{
    std::cout << t << std::endl;
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

    sprintf(dst, "%d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
}

void mylog::logger::set_log_name(const char *log_name)
{
    ::strcpy(this->log_name, log_name);
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

mylog::logger::logger()
{

    log_name = (char *)malloc(LogNameSize);
    log_date = (char *)malloc(LogDateSize);
    log_file_name = (char *)malloc(LogNameSize + LogDateSize);

    this->p = nullptr;
    ::strcpy(this->log_name, "log");
    strcpy(this->log_date, "0000-00-00");
}

mylog::logger::logger(const char *log_name)
{
    this->log_name = (char *)malloc(LogNameSize);
    log_date = (char *)malloc(LogDateSize);
    log_file_name = (char *)malloc(LogNameSize + LogDateSize);

    this->p = nullptr;
    ::strcpy(this->log_name, log_name);
    strcpy(this->log_date, "0000-00-00");
}

mylog::logger::~logger()
{
    free(log_name);
    free(log_date);
    free(log_file_name);
    clear_log_handle();
}

void mylog::logger::create_log_handle()
{
    current_date(log_date);

    ::strcpy(log_file_name, this->log_name);
    ::strcat(log_file_name, " ");
    ::strcat(log_file_name, log_date);
    ::strcat(log_file_name, ".log");

    std::ofstream *fd = new std::ofstream(log_file_name, std::ios::app);
    if (!fd->is_open())
    {
        clear_log_handle();
        delete fd;
        return;
    }

    this->p = fd;
}

bool mylog::logger::is_same_date()
{
    char *TmpDate = (char *)malloc(LogDateSize);

    current_date(TmpDate);
    bool IsDateEqu = [](const char *p1, const char *p2) -> bool
    {
        for (int i = 0; i < LogDateSize; i++)
        {
            if (p1[i] != p2[i])
                return false;
        }
        return true;
    }(TmpDate, log_date);
    free(TmpDate);
    return IsDateEqu;
}

void mylog::logger::clear_log_handle()
{
    ::strcpy(log_file_name, "");
    ::strcpy(log_date, "");
    close_log_handle();
}

void mylog::logger::write(const char *log)
{
    if (!is_same_date())
    {
        clear_log_handle();
        create_log_handle();
    }

    auto content = new char[LogTimePreffixSize + ::strlen(log)];
    log_time_preffix(content);
    ::strcat(content, log);

    LogFileHandle << content << std::endl;
    delete[] content;
}

// log and print
void mylog::logger::log(const char *log)
{
    if (!is_same_date())
    {
        clear_log_handle();
        create_log_handle();
    }

    auto content = new char[LogTimePreffixSize + ::strlen(log)];
    log_time_preffix(content);
    ::strcat(content, log);

    LogFileHandle << content << std::endl;
    std::cout << content << std::endl;

    delete[] content;
}

void mylog::logger::format_log(char *dst, const char *log)
{
    log_time_preffix(dst);
    ::strcat(dst, log);
}

void mylog::logger::close_log_handle()
{

    if (this->p != nullptr)
    {
        if (LogFileHandlePtr->is_open())
        {
            LogFileHandlePtr->close();
        }

        delete LogFileHandlePtr;
        this->p = nullptr;
    }
}

void mylog::logger::flush_log()
{
    if (this->p != nullptr)
    {
        if (LogFileHandlePtr->is_open())
            LogFileHandlePtr->flush();
    }
}

#define MsgQueuePtr ((msg_queue *)p)

mylog::logmgr::logmgr()
{
    this->p = new msg_queue(10000);
    MsgQueuePtr->set_max_thread_num(1);
    MsgQueuePtr->register_msg(this->msg_enum_log, [](void *param) -> void
                              {
                                  auto lp = (log_payload *)param;
                                  lp->lg->log(lp->content);
                                  delete[] lp->content;
                                  delete lp;
                              });

    MsgQueuePtr->register_msg(this->msg_enum_write, [](void *param) -> void
                              {
                                  auto lp = (log_payload *)param;
                                  lp->lg->write(lp->content);
                                  delete[] lp->content;
                                  delete lp;
                              });

    MsgQueuePtr->register_msg(this->msg_enum_flush, [](void *param) -> void
                              { ((mylog::logmgr *)param)->flush_all(); });

    std::thread([this]() -> void
                {
                    for (;;)
                    {
                        std::this_thread::sleep_for(std::chrono::seconds(300));
                        this->put_msg(this->msg_enum_flush, (void *)this);
                    }
                })
        .detach();

    MsgQueuePtr->start();
}

void mylog::logmgr::flush_all()
{
    auto mPtr = &mylog::logmgr::get_ins()->m;
    auto it = mPtr->begin();
    while (it != mPtr->end())
    {
        it->second->flush_log();
        it++;
    }
}

mylog::logger *mylog::logmgr::get_logger(const char *log_name)
{
    auto it = this->m.find(log_name);
    if (it != m.end())
    {
        return it->second;
    }

    mylog::logger *lgPtr = new mylog::logger(log_name);
    char *temp_log_name = new char[LogNameSize];
    ::strcpy(temp_log_name, log_name);

    m[temp_log_name] = lgPtr;
    return lgPtr;
}

mylog::log_payload *mylog::log(const char *log_name)
{
    return new mylog::log_payload(log_name);
}

mylog::logmgr *mylog::logmgr::ins = nullptr;

void mylog::logmgr::put_msg(int msg_enum, void *param)
{
    MsgQueuePtr->put_msg(msg_enum, param);
}