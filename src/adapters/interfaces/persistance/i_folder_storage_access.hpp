#pragma once
#include <QJsonObject>
#include <QObject>
#include <QString>
#include "adapters_export.hpp"

namespace adapters
{

/**
 * The FolderStorageAccess class makes the API calls to the API of the used
 * Folder to store folders, etc. from it.
 */
class ADAPTERS_EXPORT IFolderStorageAccess : public QObject
{
    Q_OBJECT

public:
    virtual ~IFolderStorageAccess() noexcept = default;
};

}  // namespace adapters
