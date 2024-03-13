#include "tools_service.hpp"
#include <QProcess>
#include <QRegularExpression>
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
    auto process = new QProcess;
    QProcess::connect(process, &QProcess::finished, process,
                      &QProcess::deleteLater);

    // Add the book to the library if the merge was successful
    auto dest = getTempFilePath(destName);
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
    if(!extractionSeparationStringIsValid(separator))
    {
        emit extractingPagesFinished(false);
        return;
    }

    auto process = new QProcess;
    QProcess::connect(process, &QProcess::finished, process,
                      &QProcess::deleteLater);

    // Add the book to the library if the extract was successful
    auto dest = getTempFilePath(destName);
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

void ToolsService::convert(const QString& destName,
                                const QString& filePath)
{
    auto process = new QProcess;
    QProcess::connect(process, &QProcess::finished, process,
                      &QProcess::deleteLater);

    // Add the book to the library if the extract was successful
    auto dest = getTempFilePath(destName);
    QProcess::connect(process, &QProcess::finished, this,
                      [this, dest](int exitCode, QProcess::ExitStatus status)
                      {
                          if(status == QProcess::NormalExit && exitCode == 0)
                          {
                              m_libraryService->addBook(dest, true, 0);
                              emit convertingFinished(true);
                              return;
                          }

                          emit convertingFinished(false);
                      });

    utils::tools::convert(process, dest, filePath);
}

bool ToolsService::extractionSeparationStringIsValid(
    const QString& separator) const
{
    static QRegularExpression regex("^[0-9,-]+$");
    return !separator.isEmpty() && regex.match(separator).hasMatch();
}

QString ToolsService::getTempFilePath(const QString& fileName) const
{
    QString destFolder =
        QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0);
    return destFolder + "/" + fileName;
}

}  // namespace application::services