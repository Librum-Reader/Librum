#pragma once
#include <QObject>
#include "application_export.hpp"

namespace application
{

/**
 * The FolderGateway class acts as a layer of abstraction before the
 * FolderAccess class. It maps the data provided by the application to
 * the data type required for Folder API requests.
 */
class APPLICATION_EXPORT IFolderStorageGateway : public QObject
{
    Q_OBJECT

public:
    virtual ~IFolderStorageGateway() noexcept = default;
};

}  // namespace application
