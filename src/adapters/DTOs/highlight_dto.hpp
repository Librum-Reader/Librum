#pragma once
#include <QList>
#include <QRectF>
#include <QString>
#include "adapters_export.hpp"
#include "rectf_dto.hpp"

namespace adapters::dtos
{

struct ADAPTERS_EXPORT HighlightDto
{
    QString uuid;
    int pageNumber;
    QString color;
    QList<RectFDto> rects;
};

}  // namespace adapters::dtos