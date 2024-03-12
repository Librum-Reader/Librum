#pragma once
#include "i_library_service.hpp"
#include "i_tools_service.hpp"

namespace application::services
{

class APPLICATION_EXPORT ToolsService : public IToolsService
{
    Q_OBJECT

public:
    ToolsService(ILibraryService* libraryService);

    void merge(const QString& destName,
               const QList<QString>& filePaths) override;
    void extractPages(const QString& destName, const QString& filePath,
                      const QString& separator) override;

private:
    bool extractionSeparationStringIsValid(const QString& separator) const;
    QString getTempFilePath(const QString& fileName) const;

    ILibraryService* m_libraryService;
};

}  // namespace application::services