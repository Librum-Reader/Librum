#pragma once
#include <QObject>
#include <QString>

namespace application
{

/**
 * The UserStorageGateway class acts as a layer of abstraction before the
 * SettingsStorageAccess class. It maps the data provided by the application to
 * the data type required for user storage API requests.
 */
class ISettingsStorageGateway : public QObject
{
    Q_OBJECT

public:
    virtual ~ISettingsStorageGateway() noexcept = default;

    virtual void getSettings(const QString& authToken) = 0;
    virtual void updateSettings(const QString& authToken,
                                const QByteArray& data) = 0;

signals:
    void finishedGettingSettings(const QByteArray& data, bool success);
};

}  // namespace application