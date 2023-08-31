#pragma once
#include <QObject>
#include <QString>
#include "adapters_export.hpp"
#include "login_dto.hpp"
#include "register_dto.hpp"

namespace adapters
{

/**
 * The AppInfo class makes the API calls to the server to query metainformation
 * about Librum.
 */
class ADAPTERS_EXPORT IAppInfoAccess : public QObject
{
    Q_OBJECT

public:
    virtual ~IAppInfoAccess() noexcept = default;

    virtual void getNewestAppVersion() = 0;

signals:
    void newestAppVersionReceived(const QString& version);
};

}  // namespace adapters
