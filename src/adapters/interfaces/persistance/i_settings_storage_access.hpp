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
    virtual void updateSettings(const QString& authToken,
                                const QByteArray& data) = 0;

signals:
    void finishedGettingSettings(const QByteArray& data, bool success);
};

}  // namespace adapters