#pragma once
#include <QObject>

namespace adapters::models
{

class AppInformation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentVersion READ getCurrentVersionAsQString CONSTANT)
    Q_PROPERTY(QString newestVersion READ getNewestVersionAsQString CONSTANT)
    Q_PROPERTY(QString currentQtVersion READ getCurrentQtVersionAsQString CONSTANT)
    Q_PROPERTY(QString companyName READ getCompanyNameAsQString CONSTANT)
    Q_PROPERTY(QString companyWebsite READ getCompanyWebsiteAsQString CONSTANT)
    Q_PROPERTY(QString companyEmail READ getCompanyEmailAsQString CONSTANT)
    
public slots:
    QString getCurrentVersionAsQString() { return m_currentVersion; }
    QString getNewestVersionAsQString() { return m_newestVersion; }
    QString getCurrentQtVersionAsQString() { return m_currentQtVersion; }
    QString getCompanyNameAsQString() { return m_companyName; }
    QString getCompanyWebsiteAsQString() { return m_companyWebsite; }
    QString getCompanyEmailAsQString() { return m_companyEmail; }
    
    
private:
    QString m_currentVersion { "v0.0.1" };
    QString m_newestVersion { "v0.0.1" };
    QString m_currentQtVersion { "v6.3.1" };
    QString m_companyName { "Etovex" };
    QString m_companyWebsite { "https://librumreader.netlify.app/" };
    QString m_companyEmail { "support@etovex.com" };
};

} // namespace adapters::models