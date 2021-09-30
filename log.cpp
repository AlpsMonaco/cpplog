#include "log.h"
#include <thread>
#include <mutex>
#include <unordered_map>
#include <map>
#include <atomic>
#include <ctime>
#include <stdio.h>
#include <fstream>
#include <io.h>
#include <direct.h>

// win enviroment special config.
#ifdef _WIN32
#pragma warning(disable : 4996)

#endif

#define msgQueueImpl ((MsgQueueImpl *)this->p)

bool IsDirExist(const char *dirPath)
{
    return access(dirPath, 0) == 0;
}

void CreateMultiDir(const char *fullDirPath)
{
    int i = 1;

    char *buf = (char *)malloc(strlen(fullDirPath) + 1);
    while (fullDirPath[i] != '\0')
    {
        if (fullDirPath[i] == '\\' || fullDirPath[i] == '/')
        {
            for (int j = 0; j < i; j++)
            {
                buf[j] = fullDirPath[j];
            }
            buf[i] = '\0';
            if (!IsDirExist(buf))
                mkdir(buf);
        }

        i++;
    }

    mkdir(fullDirPath);
    free(buf);
}

struct MsgPayload
{
    MsgEnum msgEnum;
    Msg msg;

    MsgPayload(MsgEnum msgEnum, Msg msg)
    {
        this->msgEnum = msgEnum;
        this->msg = msg;
    }
};

class MsgQueueImpl
{
public:
    MsgQueueImpl();
    ~MsgQueueImpl();
    MsgQueueImpl(int containerSize);
    MsgQueueImpl(int containerSize, int consumerNum);
    // void Start();
    void Stop();

    void RegisterMsg(MsgEnum msgEnum, MsgCallBack cb);
    void SetConsumerNum(int num);
    void PutMsg(MsgEnum msgEnum, Msg m);
    int GetQueuePosition(int &num);
    void NewConsumer();

protected:
    MsgPayload **container;
    std::unordered_map<MsgEnum, MsgCallBack> *callbackMap;

    int containerSize;
    int consumerNum;

    std::atomic<int> msgCount;
    std::atomic<int> inQueueMsgCount;
    std::atomic<int> msgDoneCount;
    std::atomic<int> msgId;

    std::atomic<int> runningConsumerNum;

    void ConsumerMethod();
};

void MsgQueueImpl::NewConsumer()
{
    this->ConsumerMethod();
}

MsgQueue::~MsgQueue()
{
    delete msgQueueImpl;
}

void MsgQueueImpl::ConsumerMethod()
{
    int loopNum = 0;
    int totalMsgNum = 0;
    int msgDoneNum = 0;
    int msgId = 0;

    for (;;)
    {
        if (loopNum >= 100)
        {
            break;
        }

        totalMsgNum = msgCount;
        msgDoneNum = msgDoneCount;

        if (totalMsgNum - msgDoneNum > 0)
        {
            msgId = inQueueMsgCount++;
            if (msgId > totalMsgNum - 1)
            {
                inQueueMsgCount--;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                loopNum++;
                continue;
            }

            MsgPayload *mp = container[GetQueuePosition(msgId)];
            // if (mp != nullptr)
            // {
            auto it = this->callbackMap->find(mp->msgEnum);
            if (it != this->callbackMap->end())
            {
                it->second(mp->msg);
            }
            msgDoneCount++;
            delete mp;
            // container[GetQueuePosition(msgId)] = nullptr;
            // }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            loopNum++;
            continue;
        }
    }
    runningConsumerNum--;
}

void MsgQueue::Stop()
{
    msgQueueImpl->Stop();
}

void MsgQueueImpl::Stop()
{
    int totalMsgNum = 0;
    int msgDoneNum = 0;

    for (;;)
    {
        totalMsgNum = msgCount;
        msgDoneNum = msgDoneCount;

        if (totalMsgNum - msgDoneNum > 0)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        else
        {
            break;
        }
    }
}

int MsgQueueImpl::GetQueuePosition(int &num)
{
    return num % containerSize;
}

void MsgQueue::SetConsumerNum(int num)
{
    msgQueueImpl->SetConsumerNum(num);
}

void MsgQueueImpl::SetConsumerNum(int num)
{
    this->consumerNum = num;
}

void MsgQueue::PutMsg(MsgEnum msgEnum, Msg m)
{
    msgQueueImpl->PutMsg(msgEnum, m);
}

void MsgQueueImpl::PutMsg(MsgEnum msgEnum, Msg m)
{
    int msgId = this->msgId++;
    int msgCount = 0;
    int msgDoneNum = 0;

    int consumerNum = this->consumerNum;
    if (this->runningConsumerNum < consumerNum)
    {
        runningConsumerNum++;
        std::thread([this]() -> void
                    { this->NewConsumer(); })
            .detach();
    }

    for (;;)
    {
        msgDoneNum = this->msgDoneCount;
        msgCount = this->msgCount;
        if (msgCount - msgDoneNum >= containerSize)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        else
        {
            this->container[GetQueuePosition(msgId)] = new MsgPayload(msgEnum, m);
            this->msgCount++;
            break;
        }
    }
}

void MsgQueueImpl::RegisterMsg(MsgEnum msgEnum, MsgCallBack cb)
{
    (*this->callbackMap)[msgEnum] = cb;
}

void MsgQueue::RegisterMsg(MsgEnum msgEnum, MsgCallBack cb)
{
    msgQueueImpl->RegisterMsg(msgEnum, cb);
}

MsgQueueImpl::MsgQueueImpl()
{
    this->containerSize = MsgQueueDefaultSize;
    this->consumerNum = MsgQueueDefaultConsumerNum;
    this->msgCount = 0;
    this->inQueueMsgCount = 0;
    this->msgDoneCount = 0;
    this->runningConsumerNum = 0;
    this->msgId = 0;

    this->callbackMap = new std::unordered_map<MsgEnum, MsgCallBack>();
    this->container = new MsgPayload *[this->containerSize];
}

MsgQueueImpl::MsgQueueImpl(int containerSize)
{
    this->containerSize = containerSize;
    this->consumerNum = MsgQueueDefaultConsumerNum;
    this->msgCount = 0;
    this->inQueueMsgCount = 0;
    this->msgDoneCount = 0;
    this->runningConsumerNum = 0;
    this->msgId = 0;

    this->callbackMap = new std::unordered_map<MsgEnum, MsgCallBack>();
    this->container = new MsgPayload *[this->containerSize];
}

MsgQueueImpl::MsgQueueImpl(int containerSize, int consumerNum)
{
    this->containerSize = containerSize;
    this->consumerNum = consumerNum;
    this->msgCount = 0;
    this->inQueueMsgCount = 0;
    this->msgDoneCount = 0;
    this->runningConsumerNum = 0;
    this->msgId = 0;

    this->callbackMap = new std::unordered_map<MsgEnum, MsgCallBack>();
    this->container = new MsgPayload *[this->containerSize];
}

MsgQueueImpl::~MsgQueueImpl()
{
    delete[] this->container;
    delete this->callbackMap;
}

MsgQueue::MsgQueue()
{
    this->p = new MsgQueueImpl();
}

MsgQueue::MsgQueue(int containerSize)
{
    this->p = new MsgQueueImpl(containerSize);
}

MsgQueue::MsgQueue(int containerSize, int consumerNum)
{
    this->p = new MsgQueueImpl(containerSize, consumerNum);
}

#define loggerImpl ((LoggerImpl *)this->p)

class LoggerImpl
{
public:
    LoggerImpl(const char *logName)
    {
        this->logName = (char *)malloc(strlen(logName) + 1);
        strcpy(this->logName, logName);
        memset(t, 0, 20);
        memset(ofsDate, 0, 11);
    }

    ~LoggerImpl()
    {
        free(this->logName);
        if (ofs.is_open())
        {
            ofs.close();
        }
    }

    void Log(const char *logContent, bool isPrint = false)
    {
        UpdateTime();
        if (isPrint)
        {
            printf("%s %s\n", t, logContent);
        }

        char date[11];
        CurrentLogDate(date);
        if (!ofs.is_open() || strcmp(this->ofsDate, date))
        {
            CreateLogHandle();
        }

        if (ofs.is_open())
        {
            ofs << t << " " << logContent << std::endl;
        }
    }

protected:
    char *logName;
    char t[20];
    char ofsDate[11];
    std::ofstream ofs;

    void UpdateTime()
    {
        logutil::Logger::CurrentTime(this->t);
    }

    void CurrentLogDate(char *dst)
    {
        const int DateSize = 11;
        for (int i = 0; i < DateSize; i++)
        {
            dst[i] = this->t[i];
        }

        dst[DateSize - 1] = '\0';
    }

    void CreateLogHandle()
    {
        if (ofs.is_open())
            ofs.close();

        char filepath[256];
        char date[11];
        CurrentLogDate(date);

        if (!IsDirExist("log"))
        {
            CreateMultiDir("log");
        }

        sprintf(filepath, "log/%s %s.log", this->logName, date);
        ofs.open(filepath, std::ios::app);
        if (!ofs.is_open())
        {
            printf("open %s error!", filepath);
            return;
        }

        strcpy(this->ofsDate, date);
    }
};

logutil::Logger::Logger(const char *logName)
{
    this->p = new LoggerImpl(logName);
}

logutil::Logger::~Logger()
{
    delete loggerImpl;
}

void logutil::Logger::CurrentTime(char *dst)
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    sprintf(dst, "%d-%02d-%02d %02d:%02d:%02d", 1900 + ltm->tm_year, ltm->tm_mon, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
}

void logutil::Logger::Log(const char *logContent)
{
    loggerImpl->Log(logContent);
}

void logutil::Logger::Print(const char *logContent)
{
    loggerImpl->Log(logContent, true);
}

struct CStrCmp
{
    bool operator()(char const *a, char const *b) const
    {
        return std::strcmp(a, b) < 0;
    }
};

enum LogMsgEnum
{
    Log,
    Print,
};

struct LogMsgPayload
{
    LoggerImpl *p;
    char *logContent;
};

class LoggerWithMgr : public logutil::Logger
{
public:
    LoggerWithMgr(const char *name, MsgQueue *msgQueuePtr) : logutil::Logger(name)
    {
        this->msgQueuePtr = msgQueuePtr;
    }

    void Log(const char *logContent)
    {
        LogMsgPayload *l = (LogMsgPayload *)malloc(sizeof(LogMsgPayload));
        l->logContent = (char *)malloc(strlen(logContent) + 1);
        strcpy(l->logContent, logContent);
        l->p = loggerImpl;

        this->msgQueuePtr->PutMsg(LogMsgEnum::Log, l);
    }

    void Print(const char *logContent)
    {
        LogMsgPayload *l = (LogMsgPayload *)malloc(sizeof(LogMsgPayload));
        l->logContent = (char *)malloc(strlen(logContent) + 1);
        strcpy(l->logContent, logContent);
        l->p = loggerImpl;

        this->msgQueuePtr->PutMsg(LogMsgEnum::Print, l);
    }

private:
    MsgQueue *msgQueuePtr;
};


struct LogManager
{
    std::mutex mu;
    std::map<const char *, logutil::Logger *, CStrCmp> m;
    MsgQueue *msgQueue;
    

    LogManager()
    {
        msgQueue = new MsgQueue();
        msgQueue->SetConsumerNum(1);

        msgQueue->RegisterMsg(LogMsgEnum::Log, [](void *p) -> void
                              {
                                  ((LogMsgPayload *)p)->p->Log(((LogMsgPayload *)p)->logContent);
                                  free(((LogMsgPayload *)p)->logContent);
                                  free((LogMsgPayload *)p);
                              });

        msgQueue->RegisterMsg(LogMsgEnum::Print, [](void *p) -> void
                              {
                                  ((LogMsgPayload *)p)->p->Log(((LogMsgPayload *)p)->logContent, true);
                                  free(((LogMsgPayload *)p)->logContent);
                                  free((LogMsgPayload *)p);
                              });
    }

    logutil::Logger *GetLogger(const char *name)
    {
        mu.lock();
        auto it = m.find(name);
        if (m.end() == it)
        {
            char *tempName = (char *)malloc(strlen(name) + 1);
            strcpy(tempName, name);
            logutil::Logger *p = new LoggerWithMgr(name, this->msgQueue);
            m.emplace(tempName, p);
            mu.unlock();
            return p;
        }
        else
        {
            mu.unlock();
            return it->second;
        }
    }

} LogManager;

logutil::Logger *logutil::GetLogger(const char *name)
{
    return LogManager.GetLogger(name);
}

void logutil::LoggerShell::Log(const char* logContent)
{
    this->l->Log(logContent);
}

void logutil::LoggerShell::Print(const char* logContent)
{
    this->l->Print(logContent);
}

logutil::LoggerShell logutil::LogName(const char* logName)
{
    return LoggerShell(LogManager.GetLogger(logName));
}
