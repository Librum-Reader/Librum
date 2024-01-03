#pragma once
#include <QObject>
#include <QString>
#include "application_export.hpp"

namespace application
{

/**
 *  The FolderService handles all Folder operations, including storage etc.
 */
class APPLICATION_EXPORT IFolderService : public QObject
{
    Q_OBJECT

public:
    virtual ~IFolderService() noexcept = default;
};

}  // namespace application
