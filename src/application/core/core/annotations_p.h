/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_ANNOTATIONS_P_H
#define OKULAR_ANNOTATIONS_P_H

#include "annotations.h"
#include "area.h"

// qt/kde includes
#include <QColor>
#include <QDateTime>
#include <QString>
#include <QVariant>

class QTransform;

namespace Okular
{
class PagePrivate;

class AnnotationPrivate
{
public:
    AnnotationPrivate();

    OKULARCORE_EXPORT static AnnotationPrivate* get(Annotation* a);

    virtual ~AnnotationPrivate();

    AnnotationPrivate(const AnnotationPrivate&) = delete;
    AnnotationPrivate& operator=(const AnnotationPrivate&) = delete;

    /**
     * Transforms the annotation coordinates with the transformation
     * defined by @p matrix.
     */
    void annotationTransform(const QTransform& matrix);

    virtual void transform(const QTransform& matrix);
    virtual void baseTransform(const QTransform& matrix);
    virtual void resetTransformation();
    virtual void translate(const NormalizedPoint& coord);
    virtual void adjust(const NormalizedPoint& deltaCoord1,
                        const NormalizedPoint& deltaCoord2);
    virtual bool openDialogAfterCreation() const;
    virtual void setAnnotationProperties(const QDomNode& node);
    virtual bool canBeResized() const;
    virtual AnnotationPrivate* getNewAnnotationPrivate() = 0;

    /**
     * Determines the distance of the closest point of the annotation to the
     * given point @p x @p y @p xScale @p yScale
     * @since 0.17
     */
    virtual double distanceSqr(double x, double y, double xScale,
                               double yScale) const;

    PagePrivate* m_page;

    QString m_author;
    QString m_contents;
    QString m_uniqueName;
    QDateTime m_modifyDate;
    QDateTime m_creationDate;

    int m_flags;
    NormalizedRect m_boundary;
    NormalizedRect m_transformedBoundary;

    Okular::Annotation::Style m_style;
    Okular::Annotation::Window m_window;
    QList<Okular::Annotation::Revision> m_revisions;

    Annotation::DisposeDataFunction m_disposeFunc;
    QVariant m_nativeId;
};

}  // namespace Okular

#endif
