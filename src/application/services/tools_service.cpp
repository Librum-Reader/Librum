#include "tools_service.hpp"
#include <QProcess>
#include <QStandardPaths>
#include <QStringList>
#include "mutool_utils.hpp"

namespace application::services
{

using namespace core;

ToolsService::ToolsService(ILibraryService* libraryService) :
    m_libraryService(libraryService)
{
}

void ToolsService::mergePdfs(const QString& destName,
                             const QList<QString>& filePaths)
{
    QString destFolder =
        QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0);
    auto dest = destFolder + "/" + destName;

    auto process = new QProcess;
    QProcess::connect(process, &QProcess::finished, process,
                      &QProcess::deleteLater);

    // Add the book to the library if the merge was successful
    QProcess::connect(process, &QProcess::finished, this,
                      [this, dest](int exitCode, QProcess::ExitStatus status)
                      {
                          if(status == QProcess::NormalExit && exitCode == 0)
                          {
                              m_libraryService->addBook(dest, true, 0);
                              emit mergingPdfsFinished(true);
                              return;
                          }

                          emit mergingPdfsFinished(false);
                      });

    utils::tools::mergePdfs(process, dest, filePaths);
}

}  // namespace application::services