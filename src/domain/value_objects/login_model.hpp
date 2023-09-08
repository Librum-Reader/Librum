#pragma once
#include <QString>
#include "domain_export.hpp"

namespace domain::value_objects
{

class DOMAIN_EXPORT LoginModel
{
public:
    LoginModel(QString email, QString password, bool rememberUser);

    QString getEmail() const;
    QString getPassword() const;
    bool getRememberUser() const;

private:
    QString m_email;
    QString m_password;
    bool m_rememberUser = false;
};

}  // namespace domain::value_objects
