#pragma once
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QString>

namespace logging
{

void logMessageToStdout(const QString& logLine);
void logMessageToFile(const QString& logLine);

void messageHandler(QtMsgType type, const QMessageLogContext& context,
                    const QString& msg)
{
    // Redirect qml messages to stdout
    QString fileName(context.file);
    if(fileName.endsWith(".qml"))
    {
        logMessageToStdout("Qml: " + msg);
        return;
    }

    QString logLine;
    switch(type)
    {
    case QtInfoMsg:
        logLine = QString("Info: %1").arg(msg);
        break;
    case QtDebugMsg:
        logLine = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        logLine = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        logLine = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        logLine = QString("Fatal: %1").arg(msg);
        break;
    }

    logMessageToFile(logLine);
}

void logMessageToStdout(const QString& logLine)
{
    QTextStream out(stdout);
    out << logLine << Qt::endl;
}

void logMessageToFile(const QString& logLine)
{
    QFile file("librum_log.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Append))
        logMessageToStdout(logLine);  // Fallback

    QTextStream logStream(&file);
    QDateTime current = QDateTime::currentDateTime();
    QString dateString = current.toString("dd.MM.yyyy - hh.mm.ss");
    logStream << "(" << dateString << "): " << logLine << Qt::endl << Qt::endl;
}

}  // namespace logging