#pragma once
#include <QObject>
#include <string_view>

namespace application
{

class AppInformation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentVersion READ getCurrentVersion CONSTANT)
    Q_PROPERTY(QString newestVersion READ getNewestVersion CONSTANT)
    Q_PROPERTY(QString currentQtVersion READ getCurrentQtVersion CONSTANT)
    Q_PROPERTY(QString companyName READ getCompanyName CONSTANT)
    Q_PROPERTY(QString companyWebsite READ getCompanyWebsite CONSTANT)
    Q_PROPERTY(QString companyEmail READ getCompanyEmail CONSTANT)
    
public:
    static QString getCurrentVersion() { return QString::fromStdString(m_currentVersion.data()); }
    static QString getNewestVersion() { return QString::fromStdString(m_newestVersion.data()); }
    static QString getCurrentQtVersion() { return QString::fromStdString(m_currentQtVersion.data()); }
    static QString getCompanyName() { return QString::fromStdString(m_companyName.data()); }
    static QString getCompanyWebsite() { return QString::fromStdString(m_companyWebsite.data()); }
    static QString getCompanyEmail() { return QString::fromStdString(m_companyEmail.data()); }
    
    
private:
    constexpr static std::string_view m_currentVersion { "v0.0.1" };
    constexpr static std::string_view m_newestVersion { "v0.0.1" };
    constexpr static std::string_view m_currentQtVersion { "v6.3.0" };
    constexpr static std::string_view m_companyName { "Etovex" };
    constexpr static std::string_view m_companyWebsite { "https://librumreader.netlify.app/" };
    constexpr static std::string_view m_companyEmail { "support@etovex.com" };
};

}