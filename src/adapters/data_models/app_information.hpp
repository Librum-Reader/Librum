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
    Q_PROPERTY(QString newsWebsite READ getNewsWebsite CONSTANT)
    Q_PROPERTY(QString companyEmail READ getCompanyEmail CONSTANT)
    Q_PROPERTY(QString githubLink READ getGithubLink CONSTANT)

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

    QString getGithubLink()
    {
        return m_githubLink;
    }

    QString getNewsWebsite()
    {
        return m_newsWebsite;
    }


private:
    QString m_currentVersion { "v0.4.6" };
    QString m_newestVersion { "v0.4.6" };
    QString m_currentQtVersion { "v5.15.8" };
    QString m_companyName { "Librum-Reader" };
    QString m_productName { "Librum" };
    QString m_companyWebsite { "https://librumreader.com" };
    QString m_newsWebsite { "https://librumreader.com/news" };
    QString m_companyEmail { "contact@librumreader.com" };
    QString m_githubLink { "https://github.com/Librum-Reader/Librum" };
};

}  // namespace adapters::data_models