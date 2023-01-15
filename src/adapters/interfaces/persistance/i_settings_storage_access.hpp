#pragma once
#include <QObject>
#include <QString>

namespace adapters
{

/**
 * The SettingsStorageAccess class makes the API calls to the settings storage
 * server.
 */
class ISettingsStorageAccess : public QObject
{
    Q_OBJECT

public:
    virtual ~ISettingsStorageAccess() noexcept = default;

    virtual void getSettings(const QString& authToken) = 0;
    virtual void setSettings(const QString& authToken,
                             const QByteArray& data) = 0;

signals:
    void settingsReady(const QByteArray& data);
    void gettingUserFailed();
};

}  // namespace adapters