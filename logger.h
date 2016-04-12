#ifndef LOGGER_H
#define LOGGER_H
#include <QFile>
#include <QString>

class Logger
{
public:
    QFile file;
    static Logger& getInstance()
    {
        static Logger instance; // Guaranteed to be destroyed.
                                // Instantiated on first use.
        return instance;
    }
    static void write(QString line);
    Logger(Logger const&)               = delete;
    void operator=(Logger const&)       = delete;
    ~Logger();
private:
    Logger();

};

#endif // LOGGER_H
