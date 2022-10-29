#pragma once
#include <QObject>
#include <QString>

namespace adapters::dtos
{

struct UserDto
{
    Q_GADGET
    Q_PROPERTY(QString firstName MEMBER firstName)
    Q_PROPERTY(QString lastName MEMBER lastName)
    Q_PROPERTY(QString email MEMBER email)

public:
    QString firstName;
    QString lastName;
    QString email;
};

}  // namespace adapters::dtos

Q_DECLARE_METATYPE(adapters::dtos::UserDto)