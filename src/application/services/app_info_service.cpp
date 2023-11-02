#include "app_info_service.hpp"
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>

namespace application::services
{

AppInfoService::AppInfoService(IAppInfoGateway* appInfoGateway) :
    m_appInfoGateway(appInfoGateway)
{
    connect(m_appInfoGateway, &IAppInfoGateway::newestAppVersionReceived, this,
            &AppInfoService::setNewestVersion);

    connect(m_appInfoGateway, &IAppInfoGateway::downloadingBinariesFinished,
            this, &AppInfoService::processDownloadedBinaryData);

    connect(m_appInfoGateway,
            &IAppInfoGateway::downloadingBinariesProgressChanged, this,
            [this](qint64 received, qint64 total)
            {
                auto progress = static_cast<double>(received) / total;
                emit downloadingBinariesProgressChanged(progress);
            });

    m_appInfoGateway->getNewestAppVersion();
}

QString AppInfoService::getInfo(const QString& key)
{
    if(key == "newestVersion")
        return m_newestVersion;

    QFile infoFile(m_infoFilePath);
    if(!infoFile.open(QIODevice::ReadOnly))
    {
        qWarning() << QString("Failed opening app info file at: %1")
                          .arg(m_infoFilePath);
        return "";
    }

    auto jsonDoc = QJsonDocument::fromJson(infoFile.readAll());
    auto jsonObject = jsonDoc.object();

    auto value = jsonObject[key].toString();
    if(value.isEmpty())
    {
        qWarning()
            << QString("Failed getting invalid app info key: %1").arg(key);
        return "";
    }

    return value;
}

void AppInfoService::updateApplication()
{
    auto binaryPackageName = getBinaryPackageName();
    m_appInfoGateway->downloadBinaries(binaryPackageName);
}

/**
 * This function uses an external .bat script to query Administrator rights,
 * unzip and move the binaries at source to the given destination.
 */
void AppInfoService::unpackAndInstallBinaries(const QString& path)
{
    QDir currentDir(QDir::currentPath());
    QString processPath = currentDir.filePath("unzip_and_move.bat");
    if(!QFile::exists(processPath))
    {
        qWarning() << "Windows update script does not exist";
        return;
    }

    QProcess* process = new QProcess;
    connect(process, &QProcess::errorOccurred, this,
            [process](QProcess::ProcessError error)
            {
                qWarning() << error;
                process->deleteLater();
            });

    QStringList arguments;
    arguments << path;
    process->startDetached(processPath, arguments);

    // App is started again by the script
    stopApplication();
}

QString AppInfoService::getBinaryPackageName()
{
    auto newestVersion = getInfo("newestVersion");
    auto binaryPackageName = "librum-windows-" + newestVersion + "-binaries";

    return binaryPackageName;
}

void AppInfoService::stopApplication()
{
    qApp->quit();
}

void AppInfoService::setNewestVersion(const QString& newestVersion)
{
    m_newestVersion = newestVersion;
    emit newestVersionChanged();
}

void AppInfoService::processDownloadedBinaryData(const QByteArray& data,
                                                 bool success)
{
    if(!success || data.isEmpty())
    {
        qWarning() << "Downloading binary data failed. Can't update.";
        emit applicationUpdateFailed();
        return;
    }

    auto path = saveBinaryDataToFile(QDir::temp(), data);
    if(path.isEmpty())
        return;

    unpackAndInstallBinaries(path);
}

QString AppInfoService::saveBinaryDataToFile(const QDir& destFolder,
                                             const QByteArray& data)
{
    auto binaryPackageName = getBinaryPackageName();
    auto dest = destFolder.filePath(binaryPackageName + ".zip");
    QFile zipFile(dest);
    if(!zipFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Failed saving binary data to file: "
                   << zipFile.errorString();
        return "";
    }

    zipFile.write(data);
    return dest;
}

}  // namespace application::services