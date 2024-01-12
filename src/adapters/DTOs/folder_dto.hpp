#pragma once
#include <QObject>
#include <QString>
#include "adapters_export.hpp"

namespace adapters::dtos
{

struct ADAPTERS_EXPORT FolderDto
{
    Q_GADGET
    Q_PROPERTY(QString uuid MEMBER uuid)
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QString color MEMBER color)
    Q_PROPERTY(QString icon MEMBER icon)
    Q_PROPERTY(QString description MEMBER description)

public:
    QString uuid;
    QString name;
    QString color;
    QString icon;
    QString description;
};

}  // namespace adapters::dtos

Q_DECLARE_METATYPE(adapters::dtos::FolderDto)