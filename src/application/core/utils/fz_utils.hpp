#pragma once
#include <mupdf/classes.h>
#include <mupdf/classes2.h>
#include <QImage>
#include <QPointF>
#include <QRectF>
#include "mupdf/fitz/geometry.h"

namespace application::core::utils
{

// Private
namespace
{

void imageCleanupHandler(void* data)
{
    unsigned char* samples = static_cast<unsigned char*>(data);
    delete[] samples;
}

}  // namespace

using FzPointPair = QPair<mupdf::FzPoint, mupdf::FzPoint>;

inline QRectF fzRectToQRectF(const mupdf::FzRect& rect)
{
    return QRectF(rect.x0, rect.y0, rect.x1 - rect.x0, rect.y1 - rect.y0);
}

inline mupdf::FzRect qRectFtoFzRect(const QRectF& rect)
{
    return mupdf::FzRect(rect.topLeft().x(), rect.topLeft().y(),
                         rect.bottomRight().x(), rect.bottomRight().y());
}

// Public
inline QPointF scalePointToCurrentZoom(const QPointF& point, float oldZoom,
                                       float newZoom)
{
    mupdf::FzPoint fzPoint(point.x(), point.y());
    auto oldMatrix = mupdf::FzMatrix();
    oldMatrix.a = oldZoom;
    oldMatrix.d = oldZoom;

    // Normalize point
    auto invMatrix = oldMatrix.fz_invert_matrix();
    fzPoint = fzPoint.fz_transform_point(invMatrix);

    // Apply new zoom
    auto newMatrix = mupdf::FzMatrix();
    newMatrix.a = newZoom;
    newMatrix.d = newZoom;
    fzPoint = fzPoint.fz_transform_point(newMatrix);

    return QPointF(fzPoint.x, fzPoint.y);
}

inline void scaleQRectFToZoom(QRectF& rect, float newZoom)
{
    auto fzRect = qRectFtoFzRect(rect);

    // Apply new zoom
    auto newMatrix = mupdf::FzMatrix();
    newMatrix.a = newZoom;
    newMatrix.d = newZoom;
    auto scaledFzRect = fzRect.fz_transform_rect(newMatrix);

    rect = fzRectToQRectF(scaledFzRect);
}

inline QRectF fzQuadToQRectF(const mupdf::FzQuad& rect)
{
    float width = rect.ur.x - rect.ul.x;
    float height = rect.ll.y - rect.ul.y;
    return QRectF(rect.ul.x, rect.ul.y, width, height);
}

inline QImage qImageFromPixmap(mupdf::FzPixmap pixmap)
{
    int width = pixmap.w();
    int height = pixmap.h();
    int stride = pixmap.stride();
    auto samples = pixmap.samples();

    // Copy samples
    std::size_t length = stride * height;
    unsigned char* destination = new unsigned char[length + 1];
    std::copy(samples, samples + length + 1, destination);

    QImage image(destination, width, height, stride, QImage::Format_RGB888,
                 imageCleanupHandler, destination);
    return image;
}

inline mupdf::FzPoint qPointToFzPoint(const QPointF& qPoint, double scaler = 1)
{
    return mupdf::FzPoint(qPoint.x() * scaler, qPoint.y() * scaler);
}

inline QPointF fzPointToQPoint(const mupdf::FzPoint& fzPoint,
                               double invScaler = 1)
{
    return QPointF(fzPoint.x / invScaler, fzPoint.y / invScaler);
}

/**
 * Restores the point to its original position by applying the inverse
 * transformation matrix to it. This is necessary because mupdf expects
 * arguments passed to be without any zooming or similar applied.
 */
inline void restoreFzPoint(mupdf::FzPoint& point, mupdf::FzMatrix& matrix)
{
    auto invMatrix = matrix.fz_invert_matrix();
    point = point.fz_transform_point(invMatrix);
}

inline QPointF restoreQPoint(const QPointF& point, float zoom)
{
    auto fzPoint = qPointToFzPoint(point);

    mupdf::FzMatrix matrix;
    matrix.a = zoom;
    matrix.d = zoom;

    restoreFzPoint(fzPoint, matrix);
    return fzPointToQPoint(fzPoint);
}

/**
 * Restores a rect to its original position by applying the inverse
 * transformation matrix to it. This is necessary because mupdf expects
 * arguments passed to be without any zooming or similar applied.
 */
inline void restoreQRect(QRectF& rect, float zoom)
{
    auto fzRect = qRectFtoFzRect(rect);

    mupdf::FzMatrix matrix;
    matrix.a = zoom;
    matrix.d = zoom;

    auto invMatrix = matrix.fz_invert_matrix();
    auto restoredFzRect = fzRect.fz_transform_rect(invMatrix);

    rect = fzRectToQRectF(restoredFzRect);
}

inline fz_quad moveQuad(const fz_quad& quad, int x, int y)
{
    return fz_make_quad(quad.ul.x - x, quad.ul.y - y, quad.ur.x - x,
                        quad.ur.y - y, quad.ll.x - x, quad.ll.y - y,
                        quad.lr.x - x, quad.lr.y - y);
}

inline mupdf::FzRect moveRect(const mupdf::FzRect& rect, int x, int y)
{
    return mupdf::fz_make_rect(rect.x0 - x, rect.y0 - y, rect.x1 - x,
                               rect.y1 - y);
}

inline mupdf::FzPoint movePoint(const mupdf::FzPoint& point, int x, int y)
{
    return fz_make_point(point.x + x, point.y + y);
}

}  // namespace application::core::utils
