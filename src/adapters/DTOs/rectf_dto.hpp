#pragma once
#include <QRect>
#include <QString>
#include "adapters_export.hpp"

namespace adapters::dtos
{

struct ADAPTERS_EXPORT RectFDto
{
    QString uuid;
    QRectF rect;
};

}  // namespace adapters::dtos