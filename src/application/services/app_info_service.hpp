#pragma once
#include <QDir>
#include <QObject>
#include "i_app_info_service.hpp"
#include "i_app_info_gateway.hpp"

namespace application::services
{

class AppInfoService : public IAppInfoService
{
    Q_OBJECT

public:
    AppInfoService(IAppInfoGateway* appInfoGateway);

    QString getInfo(const QString &key) override;
    void updateApplication() override;

private slots:
    void setNewestVersion(const QString& newestVersion);
    void processDownloadedBinaryData(const QByteArray& data, bool success);

private:
    QString saveBinaryDataToFile(const QDir& destFolder, const QByteArray& data);
    void unpackAndInstallBinaries(const QString& path);
    QString getBinaryPackageName();
    void stopApplication();

    IAppInfoGateway* m_appInfoGateway;
    QString m_infoFilePath = ":/app_info.json";
    QString m_newestVersion = "";
};

}  // namespace application::services