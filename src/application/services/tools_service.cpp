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

void ToolsService::merge(const QString& destName,
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
                              emit mergingFinished(true);
                              return;
                          }

                          emit mergingFinished(false);
                      });

    utils::tools::merge(process, dest, filePaths);
}

void ToolsService::extractPages(const QString& destName,
                                const QString& filePath,
                                const QString& separator)
{
    QString destFolder =
        QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0);
    auto dest = destFolder + "/" + destName;

    auto process = new QProcess;
    QProcess::connect(process, &QProcess::finished, process,
                      &QProcess::deleteLater);

    // Add the book to the library if the extract was successful
    QProcess::connect(process, &QProcess::finished, this,
                      [this, dest](int exitCode, QProcess::ExitStatus status)
                      {
                          if(status == QProcess::NormalExit && exitCode == 0)
                          {
                              m_libraryService->addBook(dest, true, 0);
                              emit extractingPagesFinished(true);
                              return;
                          }

                          emit extractingPagesFinished(false);
                      });

    utils::tools::extract(process, dest, filePath, separator);
}

}  // namespace application::services