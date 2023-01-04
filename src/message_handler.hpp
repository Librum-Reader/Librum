#pragma once
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QString>

namespace logging
{

void logMessage(const QString& logLine, bool writeToStdOut);

void messageHandler(QtMsgType type, const QMessageLogContext& context,
                    const QString& msg)
{
    // Redirect qml messages to the console
    QString fileName(context.file);
    if(fileName.endsWith(".qml"))
    {
        qDebug() << "Qml: " << msg;
        return;
    }

    bool writeToStdOut = false;
    QString logLine;
    switch(type)
    {
    case QtInfoMsg:
        logLine = QString("Info: %1").arg(msg);
        break;
    case QtDebugMsg:
        logLine = QString("Debug: %1").arg(msg);
        writeToStdOut = true;
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

    logMessage(logLine, writeToStdOut);
}

void logMessage(const QString& logLine, bool writeToStdOut)
{
    QFile file("librum_log.txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Append) && !writeToStdOut)
    {
        QTextStream logStream(&file);

        QDateTime current = QDateTime::currentDateTime();
        QString dateString = current.toString("dd.MM.yyyy - hh.mm.ss");
        logStream << "(" << dateString << "): " << logLine << Qt::endl
                  << Qt::endl;
    }
    else
    {
        QTextStream out(stdout);
        out << "Librum message: " << logLine << Qt::endl;
    }
}

}  // namespace logging