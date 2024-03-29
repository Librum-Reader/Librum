#pragma once
#include <QObject>
#include "application_export.hpp"

namespace application
{

/**
 *  The ToolsService handles all the tool operations.
 */
class APPLICATION_EXPORT IToolsService : public QObject
{
    Q_OBJECT

public:
    virtual ~IToolsService() noexcept = default;

    virtual void merge(const QString& destName,
                       const QList<QString>& filePaths) = 0;
    virtual void extractPages(const QString& destName, const QString& filePath,
                              const QString& separator) = 0;
    virtual void imageToPdf(const QString& destName,
                            const QString& filePath) = 0;

signals:
    void mergingFinished(bool success);
    void extractingPagesFinished(bool success);
    void convertingFinished(bool success);
};

}  // namespace application