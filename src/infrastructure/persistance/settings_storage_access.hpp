#pragma once
#include <QByteArray>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <memory>
#include "i_settings_storage_access.hpp"

namespace infrastructure::persistence
{

class SettingsStorageAccess : public adapters::ISettingsStorageAccess
{
    Q_OBJECT

public:
    void getSettings(const QString& authToken) override;
    void updateSettings(const QString& authToken,
                        const QByteArray& data) override;

private:
    QNetworkAccessManager m_networkAccessManager;
    std::unique_ptr<QNetworkReply> m_getSettingsReply = nullptr;
};

}  // namespace infrastructure::persistence