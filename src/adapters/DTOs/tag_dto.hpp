#pragma once
#include <QObject>
#include <QString>


namespace adapters::dtos
{

struct TagDto
{
    Q_GADGET
    Q_PROPERTY(QString name MEMBER name)
    
public:
    QString name;
};

} // namespace adapters::dtos

Q_DECLARE_METATYPE(adapters::dtos::TagDto)