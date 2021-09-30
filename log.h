#pragma once
#define ExternPointer void *

#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

using MsgEnum = int;
using Msg = void *;
using MsgCallBack = void (*)(Msg msg);

#define MsgQueueDefaultSize 100
#define MsgQueueDefaultConsumerNum 3

class MsgQueue
{
public:
    MsgQueue();
    MsgQueue(int containerSize);
    MsgQueue(int containerSize, int consumerNum);
    ~MsgQueue();
    void RegisterMsg(MsgEnum msgEnum, MsgCallBack cb);
    void PutMsg(MsgEnum msgEnum, Msg m);
    void SetConsumerNum(int num);

    void Stop();

protected:
    ExternPointer p;
};

#endif

using LogInterface = void (*)(const char *);

// logger::Name("info").Log("something");
// logger::Name("info").Print("something");
namespace logutil
{
    class Logger
    {
    public:
        Logger(const char *logName);
        ~Logger();

        virtual void Log(const char *logContent);
        virtual void Print(const char *logContent);

        // fill current time to dst with format 2006-01-02 15:04:05,dst requires at least 20 bytes(including '\0').
        static void CurrentTime(char *dst);

    protected:
        ExternPointer p;
    };

    struct LoggerShell {
        Logger* l;

        LoggerShell(Logger* l) {
            this->l = l;
        }

       void Log(const char* logContent);
       void Print(const char* logContent);
    };


    LoggerShell LogName(const char* logName);
    Logger* GetLogger(const char* logName);
};