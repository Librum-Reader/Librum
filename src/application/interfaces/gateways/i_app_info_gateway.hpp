#pragma once
#include <QObject>
#include <QString>
#include "application_export.hpp"

namespace application
{

/**
 * The AppInfoGateway class acts as a layer of abstraction before the
 * AppInfoAccess class. It maps the data provided by the application to
 * the data type required by the API.
 */
class APPLICATION_EXPORT IAppInfoGateway : public QObject
{
    Q_OBJECT

public:
    virtual ~IAppInfoGateway() noexcept = default;

    virtual void getNewestAppVersion() = 0;

signals:
    void newestAppVersionReceived(const QString& newestAppVersion);
};

}  // namespace application