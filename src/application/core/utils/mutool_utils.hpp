#pragma once
#include <QProcess>
#include <QStandardPaths>
#include <QStringList>

namespace application::core::utils::tools
{

void mergePdfs(QProcess* process, const QString& dest,
               const QList<QString>& filePaths)
{
    QString tool = "mutool";
    QString subCommand = "merge";

    auto args = QStringList { subCommand, "-o", dest } << filePaths;
    process->start(tool, args);
}

}  // namespace application::core::utils::tools