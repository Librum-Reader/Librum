#pragma once
#include <QObject>
#include <QString>
#include "application_export.hpp"

namespace application
{

/**
 *  The IAppInfoService handles the app information.
 */
class APPLICATION_EXPORT IAppInfoService : public QObject
{
    Q_OBJECT

public:
    virtual ~IAppInfoService() noexcept = default;

    virtual QString getInfo(const QString& key) = 0;
    virtual void getNewestAppVersion() = 0;

signals:
    void newestAppVersionReceived(const QString& newestAppVersion);
};

}  // namespace application