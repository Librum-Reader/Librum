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
    void setSettings(const QString& authToken, const QByteArray& data) override;

private:
    QNetworkRequest createRequest(const QUrl& url, const QString& authToken);
    bool checkForErrors(int expectedStatusCode, QNetworkReply* reply);

    QNetworkAccessManager m_networkAccessManager;
    std::unique_ptr<QNetworkReply> m_getSettingsReply = nullptr;
};

}  // namespace infrastructure::persistence