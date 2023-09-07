#pragma once
#include <mupdf/classes.h>
#include <QImage>
#include <QPointF>
#include <QRectF>

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

// Public
QPointF scalePointToCurrentZoom(const QPointF& point, float oldZoom,
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

QRectF fzQuadToQRectF(const mupdf::FzQuad& rect)
{
    float width = rect.ur.x - rect.ul.x;
    float height = rect.ll.y - rect.ul.y;
    return QRectF(rect.ul.x, rect.ul.y, width, height);
}

QImage qImageFromPixmap(mupdf::FzPixmap pixmap)
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

}  // namespace application::core::utils