#pragma once
#include <QList>
#include <QObject>
#include <QString>
#include "adapters_export.hpp"

namespace adapters
{

/**
 * The ToolsController class is exposed to the UI code and thus is the
 * "entry point" to the application's backend for tool operations.
 */
class ADAPTERS_EXPORT IToolsController : public QObject
{
    Q_OBJECT

public:
    virtual ~IToolsController() noexcept = default;

    Q_INVOKABLE virtual void merge(const QString& destName,
                                   const QList<QString>& filePaths) = 0;
    Q_INVOKABLE virtual void extractPages(const QString& destName,
                                          const QString& filePath,
                                          const QString& separatorString) = 0;
    Q_INVOKABLE virtual void imageToPdf(const QString& destName,
                                        const QString& filePath) = 0;

signals:
    void mergingFinished(bool sucess);
    void extractingPagesFinished(bool sucess);
    void convertingFinished(bool success);
};

}  // namespace adapters
