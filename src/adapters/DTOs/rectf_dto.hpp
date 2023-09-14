#pragma once
#include <QString>
#include <QRect>
#include "adapters_export.hpp"

namespace adapters::dtos
{

struct ADAPTERS_EXPORT RectFDto
{
    QString uuid;
    QRectF rect;
};

}  // namespace adapters::dtos