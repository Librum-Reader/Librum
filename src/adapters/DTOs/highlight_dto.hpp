#pragma once
#include <QList>
#include <QRectF>
#include <QString>
#include "adapters_export.hpp"

namespace adapters::dtos
{

struct ADAPTERS_EXPORT HighlightDto
{
    QString uuid;
    int pageNumber;
    QString color;
    QList<QRectF> rects;
};

}  // namespace adapters::dtos