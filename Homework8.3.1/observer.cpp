#include <iostream>
#include <fstream>
#include <vector>

class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

class MessagePrint : public LogCommand
{
public:
    explicit MessagePrint (std::string messagePrint) : messagePrint_(messagePrint) {}
    void print(const std::string& message) override
    {
        std::cout << message << std::endl;
    }
protected:
    std::string messagePrint_;
};

class MessageOut : public LogCommand
{
public:
    explicit MessageOut(std::string messageOut, std::string path) : messageOut_(messageOut), path_(path) {}
    void print(const std::string& message) override
    {
        std::ofstream out;
        out.open(path_);
        if (out.is_open())
        {
            out << messageOut_ << std::endl;
        }
        out.close();
    }
protected:
    std::string messageOut_;
    std::string path_;
};

class Alert
{
public:
    void warning(const std::string& message) const
    {
        std::cout << message << std::endl;
    }
    void error(const std::string& message) const
    {
        std::ofstream out;
        out.open("error.txt");
        if (out.is_open())
        {
            out << message << std::endl;
        }
        out.close();
    }
    void fatalError(const std::string& message) const
    {
        std::cout << message << std::endl;
        std::ofstream out;
        out.open("error.txt");
        if (out.is_open())
        {
            out << message << std::endl;
        }
        out.close();
    }
};

class Observer 
{
public:
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

class Observed 
{
public:
    void AddObserver(std::weak_ptr<Observer> observer) 
    {
        observers_.push_back(observer);
    }

    void onWarning() 
    {
        for (auto observer : observers_) 
        {
            if(auto strong_ptr = observer.lock()) {
                strong_ptr->onWarning(message_);
            }
        }
    }
    void onError() 
    {
        for (auto observer : observers_)
        {
            if (auto strong_ptr = observer.lock()) {
                strong_ptr->onError(message_);
            }
        }
    }
    void onFatalError() 
    {
        for (auto observer : observers_)
        {
            if (auto strong_ptr = observer.lock()) {
                strong_ptr->onFatalError(message_);
            }
        }
    }
private:
    std::vector<std::weak_ptr<Observer>> observers_;
    std::string& message_;
};

struct LogMessage {
public:
    std::string message = "";
    int id = -1;
};

class Handler
{
public:
    virtual ~Handler() {}
    virtual Handler* SetNext(Handler* handler) = 0;
    virtual LogMessage Handle(LogMessage logMessage) = 0;
};

class BaseHandler : public Handler
{
protected:
    Handler* next_handler;
public:
    BaseHandler() : next_handler(nullptr) {}
    Handler* SetNext(Handler* handler) override
    {
        next_handler = handler;
        return handler;
    }
    LogMessage Handle(LogMessage logMessage) override
    {
        if (next_handler)
        {
            return next_handler->Handle(logMessage);
        }
        return {};
    }
};

class FatalError : public BaseHandler
{
public:
    LogMessage Handle(LogMessage logMessage) override
    {
        if (logMessage.id == 1)
        {
            std::cout << logMessage.message << std::endl;
            std::ofstream out;
            out.open("error.txt");
            if (out.is_open())
            {
                out << logMessage.message << std::endl;
            }
            out.close();
            return logMessage;
        }
        return BaseHandler::Handle(logMessage);
    }
};

class Error : public BaseHandler
{
public:
    LogMessage Handle(LogMessage logMessage) override
    {
        if (logMessage.id == 2)
        {
            std::ofstream out;
            out.open("error.txt");
            if (out.is_open())
            {
                out << logMessage.message << std::endl;
            }
            out.close();
            return logMessage;
        }
        return BaseHandler::Handle(logMessage);
    }
};

class Warning : public BaseHandler
{
public:
    LogMessage Handle(LogMessage logMessage) override
    {
        if (logMessage.id == 3)
        {
            std::cout << logMessage.message << std::endl;
            return logMessage;
        }
        return BaseHandler::Handle(logMessage);
    }
};

class UnknownError : public BaseHandler
{
public:
    LogMessage Handle(LogMessage logMessage) override
    {
        if (logMessage.id == 4)
        {
            std::cout << "Unknow Error" << std::endl;
            return logMessage;
        }
        return BaseHandler::Handle(logMessage);
    }
};