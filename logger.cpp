#include "logger.h"
#include <QTextStream>
#include <QDebug>
void Logger::write(QString line)
{
    Logger &logger = Logger::getInstance();

    if(!logger.file.isOpen()) {
        return;
    }
    QTextStream out(&logger.file);
    out << line << "\n";
}

Logger::~Logger()
{
    if(file.isOpen()) file.close();
}

Logger::Logger()
{
    file.setFileName("debug.log");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        qDebug() << "Error, could not open logfile log.txt";
    }
}
