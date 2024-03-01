#include "tools_service.hpp"
#include <QProcess>
#include <QStandardPaths>
#include <QStringList>

namespace application::services
{

void ToolsService::mergePdfs(const QString& firstPath,
                             const QString& secondPath)
{
    auto mergeProcess = new QProcess;
    connect(mergeProcess, &QProcess::finished, mergeProcess,
            &QProcess::deleteLater);

    QString tool = "mutool";
    QString subCommand = "merge";
    QString destFolder =
        QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
            .first();
    QString destName = "merged.pdf";
    auto dest = destFolder + "/" + destName;

    mergeProcess->start(
        tool, QStringList { subCommand, "-o", dest, firstPath, secondPath });
}

}  // namespace application::services