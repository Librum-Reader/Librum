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

    Q_INVOKABLE virtual void mergePdfs(const QString& destName,
                                       const QList<QString>& filePaths) = 0;

signals:
    void mergingPdfsFinished(bool sucess);
};

}  // namespace adapters
