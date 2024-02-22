#pragma once
#include <QString>
#include "domain_export.hpp"

namespace domain::value_objects
{

class DOMAIN_EXPORT RegisterModel
{
public:
    RegisterModel(QString name, QString email, QString password,
                  bool keepUpdated);

    QString getName() const;
    QString getEmail() const;
    QString getPassword() const;
    bool getKeepUpdated() const;

private:
    QString m_name;
    QString m_email;
    QString m_password;
    const bool m_keepUpdated;
};

}  // namespace domain::value_objects
