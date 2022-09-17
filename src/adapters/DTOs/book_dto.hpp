#pragma once
#include <QObject>
#include <QString>
#include <QList>
#include <QByteArray>
#include "tag_dto.hpp"


namespace adapters::dtos
{

struct BookDto
{
    Q_GADGET
    Q_PROPERTY(QString title MEMBER title)
    Q_PROPERTY(QString filePath MEMBER filePath)
    
public:
    QString title;
    QString filePath;
    QByteArray cover;
    QList<TagDto> tags;
};

} // namespace adapters::dtos

Q_DECLARE_METATYPE(adapters::dtos::BookDto)