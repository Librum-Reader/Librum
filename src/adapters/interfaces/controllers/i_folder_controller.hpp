#pragma once
#include <QObject>
#include "adapters_export.hpp"

namespace adapters
{

/**
 * The FolderController class is exposed to the UI code and thus is the
 * "entry point" to the application's backend for folder organization. It acts
 * as a layer of abstraction which maps the user data to a format usable for the
 * application.
 */
class ADAPTERS_EXPORT IFolderController : public QObject
{
    Q_OBJECT

public:
    virtual ~IFolderController() noexcept = default;
};

}  // namespace adapters