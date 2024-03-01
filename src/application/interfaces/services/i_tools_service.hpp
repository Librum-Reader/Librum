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

    virtual void mergePdfs(const QString& firstPath,
                           const QString& secondPath) = 0;
};

}  // namespace application