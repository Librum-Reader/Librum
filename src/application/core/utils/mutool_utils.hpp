#pragma once
#include <QProcess>
#include <QStandardPaths>
#include <QStringList>

namespace application::core::utils::tools
{

void merge(QProcess* process, const QString& dest,
           const QList<QString>& filePaths)
{
    QString tool = "mutool";
    QString subCommand = "merge";

    auto args = QStringList { subCommand, "-o", dest } << filePaths;
    process->start(tool, args);
}

void extract(QProcess* process, const QString& dest, const QString& filePath,
             const QString& separator)
{
    QString tool = "mutool";
    QString subCommand = "clean";

    auto args = QStringList { subCommand, "-g", filePath, dest, separator };
    process->start(tool, args);
}

}  // namespace application::core::utils::tools