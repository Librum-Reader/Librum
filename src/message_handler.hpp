#pragma once
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QString>
#include <QUrl>

namespace logging
{

void logMessageToStdout(const QString& logLine);
void logMessageToFile(const QString& logMessage);
QString getMsgTypeAsString(const QtMsgType& type);
QString getFunctionNameFromSignature(const QString& signature);

void messageHandler(QtMsgType type, const QMessageLogContext& context,
                    const QString& msg)
{
    // Redirect qml and debug messages to stdout
    QString fileName(context.file);
    if(fileName.endsWith(".qml") || type == QtDebugMsg)
    {
        logMessageToStdout("Librum: " + msg);
        return;
    }

    QString messageType = getMsgTypeAsString(type);
    QString functionName = getFunctionNameFromSignature(context.function);

    QString logMessage =
        QString("%1 | %2 | %3 | %4 in '%5' at line: %6 | %7")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"),
                 QDateTime::currentDateTime().toString("HH:mm:ss.zzz"))
            .arg(messageType, -9)
            .arg(QUrl(context.file).fileName(), -18)
            .arg(functionName, -10)
            .arg(context.line)
            .arg(msg);

    logMessageToFile(logMessage);
}

QString getFunctionNameFromSignature(const QString& signature)
{
    int functionNameStart = signature.lastIndexOf(':') + 1;
    QString functionName = signature.mid(functionNameStart);

    return functionName;
}

QString getMsgTypeAsString(const QtMsgType& type)
{
    switch(type)
    {
    case QtInfoMsg:
        return "Info";
    case QtDebugMsg:
        return "Debug";
    case QtWarningMsg:
        return "Warning";
    case QtCriticalMsg:
        return "Critical";
    case QtFatalMsg:
        return "Fatal";
        break;
    }

    return "Unknown";
}

void logMessageToStdout(const QString& logLine)
{
    QTextStream out(stdout);
    out << logLine << Qt::endl;
}

void logMessageToFile(const QString& logMessage)
{
    QDir destDir(
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if(!destDir.exists())
        destDir.mkpath(".");

    auto logFile = destDir.filePath("librum_log.txt");
    QFile file(logFile);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Append))
        logMessageToStdout(logMessage);  // Fallback

    QTextStream logStream(&file);
    logStream << logMessage << Qt::endl << Qt::endl;
}

}  // namespace logging