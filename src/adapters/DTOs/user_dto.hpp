#pragma once
#include <QObject>
#include <QString>


namespace adapters::dtos
{

struct UserDto
{
    Q_GADGET
    Q_PROPERTY(QString title MEMBER title)
    
public:
    QString title;
};

} // namespace adapters::dtos

Q_DECLARE_METATYPE(adapters::dtos::UserDto)