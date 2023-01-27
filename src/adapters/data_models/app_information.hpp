#pragma once
#include <QObject>

namespace adapters::data_models
{

class AppInformation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentVersion READ getCurrentVersion CONSTANT)
    Q_PROPERTY(QString newestVersion READ getNewestVersion CONSTANT)
    Q_PROPERTY(QString currentQtVersion READ getCurrentQtVersion CONSTANT)
    Q_PROPERTY(QString productName READ getProductName CONSTANT)
    Q_PROPERTY(QString companyName READ getCompanyName CONSTANT)
    Q_PROPERTY(QString companyWebsite READ getCompanyWebsite CONSTANT)
    Q_PROPERTY(QString companyEmail READ getCompanyEmail CONSTANT)
    Q_PROPERTY(QString helpEmail READ getHelpEmail CONSTANT)

public slots:

    QString getCurrentVersion()
    {
        return m_currentVersion;
    }

    QString getNewestVersion()
    {
        return m_newestVersion;
    }

    QString getCurrentQtVersion()
    {
        return m_currentQtVersion;
    }

    QString getProductName()
    {
        return m_productName;
    }

    QString getCompanyName()
    {
        return m_companyName;
    }

    QString getCompanyWebsite()
    {
        return m_companyWebsite;
    }

    QString getCompanyEmail()
    {
        return m_companyEmail;
    }

    QString getHelpEmail()
    {
        return m_helpEmail;
    }


private:
    QString m_currentVersion { "v0.2" };
    QString m_newestVersion { "v0.2" };
    QString m_currentQtVersion { "v5.15.7" };
    QString m_companyName { "Etovex" };
    QString m_productName { "Librum" };
    QString m_companyWebsite { "https://librum-reader.com" };
    QString m_companyEmail { "librum_reader@protonmail.com" };
    QString m_helpEmail { "librum_help@protonmail.com" };
};

}  // namespace adapters::data_models