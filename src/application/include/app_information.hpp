#pragma once
#include <QObject>
#include <string_view>

namespace application
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
    
public:
    constexpr static std::string_view getCurrentVersion() { return m_currentVersion; }
    constexpr static std::string_view getNewestVersion() { return m_newestVersion; }
    constexpr static std::string_view getCurrentQtVersion() { return m_currentQtVersion; }
    constexpr static std::string_view getCompanyName() { return m_companyName; }
    constexpr static std::string_view getCompanyWebsite() { return m_companyWebsite; }
    constexpr static std::string_view getCompanyEmail() { return m_companyEmail; }
    
    static QString getCurrentVersionAsQString() { return QString::fromStdString(m_currentVersion.data()); }
    static QString getNewestVersionAsQString() { return QString::fromStdString(m_newestVersion.data()); }
    static QString getCurrentQtVersionAsQString() { return QString::fromStdString(m_currentQtVersion.data()); }
    static QString getCompanyNameAsQString() { return QString::fromStdString(m_companyName.data()); }
    static QString getCompanyWebsiteAsQString() { return QString::fromStdString(m_companyWebsite.data()); }
    static QString getCompanyEmailAsQString() { return QString::fromStdString(m_companyEmail.data()); }
    
    
private:
    constexpr static std::string_view m_currentVersion { "v0.0.1" };
    constexpr static std::string_view m_newestVersion { "v0.0.1" };
    constexpr static std::string_view m_currentQtVersion { "v6.3.0" };
    constexpr static std::string_view m_companyName { "Etovex" };
    constexpr static std::string_view m_companyWebsite { "https://librumreader.netlify.app/" };
    constexpr static std::string_view m_companyEmail { "support@etovex.com" };
};

}