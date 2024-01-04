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

public:
    QString uuid;
    QString name;
};

}  // namespace adapters::dtos

Q_DECLARE_METATYPE(adapters::dtos::FolderDto)