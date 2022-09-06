/*
    SPDX-FileCopyrightText: 2004-05 Enrico Ros <eros.kde@email.it>
    SPDX-FileCopyrightText: 2005 Piotr Szymanski <niedakh@gmail.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_AREA_H_
#define _OKULAR_AREA_H_

#include <math.h>

#include <QColor>
#include <QDebug>
#include <QList>
#include <QPainterPath>
#include <QTransform>

#include "global.h"
#include "okularcore_export.h"

class QPolygonF;
class QRect;

namespace Okular
{
class Annotation;
class Action;
class NormalizedShape;

/**
 * NormalizedPoint is a helper class which stores the coordinates
 * of a normalized point.
 *
 * @par Normalized Coordinate System
 * @parblock
 * Normalized means that the coordinates are always between 0 and 1,
 * unless the point shall be outside of the reference area.
 *
 * The reference area is a rectangle, and all normalized points
 * with coordinates of 0 or 1 describe its edges.
 *
 * This allows to locate things on a reference area without knowing its
 * (current or future) actual size. When the reference area is resized,
 * all things which are described in normalized coordinates keep their
 * proportional position on the area.
 * @endparblock
 *
 * @par Transformation to and from Normalized Coordinates
 * @parblock
 * To transform normalized coordinates to coordinates on the reference area,
 * just multiply them with the size of the reference area.
 *
 * To get normalized coordinates from a point on the reference area,
 * just divide its coordinates with the size of the reference area.
 *
 * Many methods have parameters @c xScale and @c yScale,
 * these are equal to the size of the reference area.
 * @endparblock
 *
 * @par Normalized Coordinate System Applied to Pages
 * @parblock
 * Okular uses a normalized coordinate system mainly to describe
 * positions on pages.
 * This is useful because pages can be shown in different sizes (zoom),
 * but all objects shall keep their proportional position on the page.
 *
 * Okular maps from page to normalized coordinates as follows:
 *  * Left edge of the page: x = 0
 *  * Right edge of the page: x = 1
 *  * Top edge of the page: y = 0
 *  * Bottom edge of the page: y = 1
 * @endparblock
 *
 * @par Example: Draw a Point on a Page
 * @parblock
 * The point is given in normalized coordinates (0.5, 0.3).
 *
 * If you want to draw it on a 800x600 page,
 * just multiply the x coordinate (0.5) with the page width (800),
 * and the y coordinate (0.3) with the page height (600).
 * So, the point will be drawn on the page at (400, 180).
 *
 * That allows you to zoom the page by just multiplying the normalized points with the
 * zoomed page size.
 * @endparblock
 *
 * @par Example: Select Text on a Page using Mouse Events
 * @parblock
 * The position of all glyphs and words is stored in normalized coordinates.
 * (This is what TextPage actually does.)
 * Mouse press and release events are given in page coordinates (400, 180) and (600, 450),
 * while the page has a size of 800x600.
 *
 * If you want to search all text between the mouse click and release event,
 * you need their normalized coordinates.
 * Just divide the x coordinates (400 and 600) by the page width (800),
 * and the y coordinates (180 and 450) by the page height (600).
 * So, you have to search for all glyphs between (0.5, 0.3) and (0.75, 0.75).
 *
 * That allows you to process all glyphs and words without
 * having to keep any of their positions in sync with the page.
 * @endparblock
 *
 * @par Geometric operations
 * @parblock
 * NormalizedPoint supports basic geometric operations.
 *  * You can transform it with a QTransform matrix.
 *  * With the size of the reference area, you can calculate the squared
 *    absolute distance to another NormalizedPoint or a line of two NormalizedPoints.
 *
 * NormalizedRect provides additional geometric operations for rectangles.
 * @endparblock
 *
 * @see NormalizedRect
 */
class OKULARCORE_EXPORT NormalizedPoint
{
public:
    /**
     * Creates a normalized point at (0, 0).
     */
    NormalizedPoint();

    /**
     * Creates a new normalized point with the normalized coordinates (@p x, @p y ).
     */
    NormalizedPoint(double x, double y);

    /**
     * Creates a new normalized point from an absolute point (@p x, @p y)
     * on a reference area of size @p xScale x @p yScale.
     */
    NormalizedPoint(int x, int y, int xScale, int yScale);

    /**
     * @internal
     */
    NormalizedPoint &operator=(const NormalizedPoint &);

    NormalizedPoint(const NormalizedPoint &);
    ~NormalizedPoint() = default;

    /**
     * Transforms the normalized point with the operations defined by @p matrix.
     */
    void transform(const QTransform &matrix);

    /**
     * Returns squared distance to normalized point (@p x, @p y)
     * on a reference area of size @p xScale x @p yScale.
     * @since 0.17 (KDE 4.11)
     */
    double distanceSqr(double x, double y, double xScale, double yScale) const;

    /**
     * Returns squared distance of the normalized point (@p x, @p y)
     * to the line segment from @p start to @p end
     * on a reference area of size @p xScale x @p yScale.
     * @since 0.17 (KDE 4.11)
     */
    static double distanceSqr(double x, double y, double xScale, double yScale, const NormalizedPoint &start, const NormalizedPoint &end);

    /**
     * The normalized x coordinate.
     */
    double x;

    /**
     * The normalized y coordinate.
     */
    double y;
};

/**
 * A NormalizedRect is a rectangle which can be defined by two NormalizedPoints.
 *
 * It describes a rectangular area on a reference area of undefined size.
 * For more information about the normalized coordinate system, see NormalizedPoint.
 *
 * In Okular, NormalizedRect can be used e. g. to describe bounding boxes of TextEntity objects,
 * and the highlight area of text selections.
 *
 * If you need to describe an area which consists of multiple rectangles,
 * you can use RegularAreaRect instead.
 *
 * @see NormalizedPoint, RegularAreaRect, TextEntity
 */
class OKULARCORE_EXPORT NormalizedRect
{
public:
    /**
     * Creates a null normalized rectangle.
     * @see isNull()
     */
    NormalizedRect();

    /**
     * Creates a normalized rectangle with the normalized coordinates
     * @p left, @p top, @p right, @p bottom.
     *
     * If you need the x, y, width and height coordinates use the
     * following formulas:
     *
     * @li x = left
     * @li y = top
     * @li width = right - left
     * @li height = bottom - top
     *
     * @note
     * The coordinates for @p left and @p top should be lower than
     * @p right and @p bottom, respectively.
     * At negative width or height the behaviour of some operations is undefined.
     */
    NormalizedRect(double left, double top, double right, double bottom);

    /**
     * Creates a normalized rectangle from the given @p rectangle
     * on a reference area of size @p xScale x @p yScale.
     *
     * @note
     * The rectangle should have positive width and height.
     * You can use e. g. QRect::normalize() to ensure this.
     * At negative width or height the behaviour of some operations is undefined.
     */
    NormalizedRect(const QRect rectangle, double xScale, double yScale);

    /**
     * @internal
     */
    NormalizedRect(const NormalizedRect &);

    /**
     * @internal
     */
    NormalizedRect &operator=(const NormalizedRect &other);

    ~NormalizedRect() = default;

    /**
     * Build a normalized rect from a QRectF, which already has normalized coordinates.
     */
    static NormalizedRect fromQRectF(const QRectF &rect);

    /**
     * Returns whether this normalized rectangle is a null normalized rect.
     */
    bool isNull() const;

    /**
     * Returns whether the normalized rectangle contains the normalized point
     * (@p x, @p y).
     */
    bool contains(double x, double y) const;

    /**
     * Returns whether the normalized rectangle intersects the @p other normalized
     * rectangle.
     */
    bool intersects(const NormalizedRect &other) const;

    /**
     * This is an overloaded member function, provided for convenience. It behaves essentially
     * like the above function.
     */
    bool intersects(const NormalizedRect *other) const;

    /**
     * Returns whether the normalized rectangle intersects an other normalized
     * rectangle, which is defined by @p left, @p top, @p right and @p bottom.
     */
    bool intersects(double left, double top, double right, double bottom) const;

    /**
     * Returns the rectangle mapped to a reference area of @p xScale x @p yScale.
     */
    QRect geometry(int xScale, int yScale) const;

    /**
     * Same functionality as geometry, but the output is now rounded before typecasting to int
     *
     * @since 0.14 (KDE 4.8)
     */
    QRect roundedGeometry(int xScale, int yScale) const;

    /**
     * Returns the normalized bounding rectangle of the normalized rectangle
     * combined with the @p other normalized rectangle.
     */
    NormalizedRect operator|(const NormalizedRect &other) const;

    /**
     * Sets the normalized rectangle to the normalized bounding rectangle
     * of itself combined with the @p other normalized rectangle.
     */
    NormalizedRect &operator|=(const NormalizedRect &other);

    /**
     * Returns the intersection of this normalized rectangle with the specified
     * @p other. If the rects do not intersect then the result is a null rectangle.
     *
     * @since 0.7 (KDE 4.1)
     */
    NormalizedRect operator&(const NormalizedRect &other) const;

    /**
     * Returns whether the normalized rectangle is equal to the @p other
     * normalized rectangle.
     */
    bool operator==(const NormalizedRect &other) const;

    /**
     * Returns the center of the rectangle
     * @since 0.10 (KDE 4.4)
     */
    NormalizedPoint center() const;

    /**
     * Transforms the normalized rectangle with the operations defined by @p matrix.
     */
    void transform(const QTransform &matrix);

    /**
     * Returns true if the point @p pt is located below the bottom of the rectangle
     * @since 0.14 (KDE 4.8)
     */
    bool isBottom(const NormalizedPoint &pt) const
    {
        return bottom < pt.y;
    }

    /**
     * Returns true if the point @p pt is located above the top of the rectangle
     * @since 0.14 (KDE 4.8)
     */
    bool isTop(const NormalizedPoint &pt) const
    {
        return top > pt.y;
    }

    /**
     * Returns true if the point @p pt is located below the top of the rectangle
     * @since 0.14 (KDE 4.8)
     */
    bool isBottomOrLevel(const NormalizedPoint &pt) const
    {
        return top < pt.y;
    }

    /**
     * Returns true if the point @p pt is located above the bottom of the rectangle
     * @since 0.14 (KDE 4.8)
     */
    bool isTopOrLevel(const NormalizedPoint &pt) const
    {
        return bottom > pt.y;
    }

    /**
     * Returns true if the point @p pt is located to the right of the left edge of the rectangle
     * @since 0.14 (KDE 4.8)
     */
    bool isLeft(const NormalizedPoint &pt) const
    {
        return left < pt.x;
    }

    /**
     * Returns true if the point @p pt is located to the left of the right edge of the rectangle
     * @since 0.14 (KDE 4.8)
     */
    bool isRight(const NormalizedPoint &pt) const
    {
        return right > pt.x;
    }

    /**
     * Returns the squared distance of the normalized point (@p x, @p y)
     * to the closest edge, or 0 if the point is within the rectangle;
     * using a reference area of size @p xScale x @p yScale
     * @since 0.17 (KDE 4.11)
     */
    double distanceSqr(double x, double y, double xScale, double yScale) const
    {
        double distX = 0;
        if (x < left) {
            distX = left - x;
        } else if (x > right) {
            distX = x - right;
        }

        double distY = 0;
        if (top > y) {
            distY = top - y;
        } else if (bottom < y) {
            distY = y - bottom;
        }
        return pow(distX * xScale, 2) + pow(distY * yScale, 2);
    }

    /// @since 1.4
    double width() const
    {
        return right - left;
    }

    /// @since 1.4
    double height() const
    {
        return bottom - top;
    }

    /**
     * The normalized left coordinate.
     */
    double left;

    /**
     * The normalized top coordinate.
     */
    double top;

    /**
     * The normalized right coordinate.
     */
    double right;

    /**
     * The normalized bottom coordinate.
     */
    double bottom;
};
// KDE_DUMMY_QHASH_FUNCTION(NormalizedRect)

/**
 * @short An area with normalized coordinates that contains a reference to an object.
 *
 * These areas ("rects") contain a pointer to a document object
 * (such as a hyperlink, an action, or something like that).
 * The pointer is read and stored as 'void pointer' so cast is
 * performed by accessors based on the value returned by objectType(). Objects
 * are reparented to this class.
 *
 * Type / Class correspondence tab:
 *  - Action    : class Action: description of an action
 *  - Image     : class Image : description of an image (n/a)
 *  - Annotation: class Annotation: description of an annotation
 *
 * For more information about the normalized coordinate system, see NormalizedPoint.
 *
 * @see NormalizedPoint
 */
class OKULARCORE_EXPORT ObjectRect
{
public:
    /**
     * Describes the type of storable object.
     */
    enum ObjectType {
        Action,      ///< An action
        Image,       ///< An image
        OAnnotation, ///< An annotation
        SourceRef    ///< A source reference
    };

    /**
     * Creates a new object rectangle.
     *
     * @param left The left coordinate of the rectangle.
     * @param top The top coordinate of the rectangle.
     * @param right The right coordinate of the rectangle.
     * @param bottom The bottom coordinate of the rectangle.
     * @param ellipse If true the rectangle describes an ellipse.
     * @param type The type of the storable object @see ObjectType.
     * @param object The pointer to the storable object.
     */
    ObjectRect(double left, double top, double right, double bottom, bool ellipse, ObjectType type, void *object);

    /**
     * This is an overloaded member function, provided for convenience.
     */
    ObjectRect(const NormalizedRect &r, bool ellipse, ObjectType type, void *object);

    /**
     * This is an overloaded member function, provided for convenience.
     */
    ObjectRect(const QPolygonF &poly, ObjectType type, void *object);

    /**
     * Destroys the object rectangle.
     */
    virtual ~ObjectRect();

    ObjectRect(const ObjectRect &o) = delete;
    ObjectRect &operator=(const ObjectRect &o) = delete;

    /**
     * Returns the object type of the object rectangle.
     * @see ObjectType
     */
    ObjectType objectType() const;

    /**
     * Returns the storable object of the object rectangle.
     */
    const void *object() const;

    /**
     * Returns the region that is covered by the object rectangle.
     */
    const QPainterPath &region() const;

    /**
     * Returns the bounding rect of the object rectangle for the
     * scaling factor @p xScale and @p yScale.
     */
    virtual QRect boundingRect(double xScale, double yScale) const;

    /**
     * Returns whether the object rectangle contains the point with absolute coordinates
     * (@p x, @p y) at a page size of @p xScale x @p yScale.
     */
    virtual bool contains(double x, double y, double xScale, double yScale) const;

    /**
     * Transforms the object rectangle with the operations defined by @p matrix.
     */
    virtual void transform(const QTransform &matrix);

    /**
     * Returns the squared distance between the object
     * and the point with
     * normalized coordinates (@p x, @p y)
     * at a page size of @p xScale x @p yScale.
     *
     * @since 0.8.2 (KDE 4.2.2)
     */
    // FIXME this should most probably be a virtual method
    double distanceSqr(double x, double y, double xScale, double yScale) const;

protected:
    ObjectType m_objectType;
    void *m_object;
    QPainterPath m_path;
    QPainterPath m_transformedPath;
};

/**
 * This class describes the object rectangle for an annotation.
 */
class OKULARCORE_EXPORT AnnotationObjectRect : public ObjectRect
{
public:
    /**
     * Creates a new annotation object rectangle with the
     * given @p annotation.
     */
    explicit AnnotationObjectRect(Annotation *annotation);

    /**
     * Destroys the annotation object rectangle.
     */
    ~AnnotationObjectRect() override;

    /**
     * Returns the annotation object of the annotation object rectangle.
     */
    Annotation *annotation() const;

    /**
     * Returns the bounding rect of the annotation object rectangle for the
     * scaling factor @p xScale and @p yScale.
     */
    QRect boundingRect(double xScale, double yScale) const override;

    /**
     * Returns whether the annotation object rectangle contains the point @p x, @p y for the
     * scaling factor @p xScale and @p yScale.
     */
    bool contains(double x, double y, double xScale, double yScale) const override;

    /**
     * Transforms the annotation object rectangle with the operations defined by @p matrix.
     */
    void transform(const QTransform &matrix) override;

private:
    Annotation *m_annotation;
};

/**
 * This class describes the object rectangle for a source reference.
 */
class OKULARCORE_EXPORT SourceRefObjectRect : public ObjectRect
{
    friend class ObjectRect;

public:
    /**
     * Creates a new source reference object rectangle.
     *
     * @param point The point of the source reference.
     * @param srcRef The storable source reference object.
     */
    SourceRefObjectRect(const NormalizedPoint &point, void *srcRef);

    /**
     * Returns the bounding rect of the source reference object rectangle for the
     * scaling factor @p xScale and @p yScale.
     */
    QRect boundingRect(double xScale, double yScale) const override;

    /**
     * Returns whether the source reference object rectangle contains the point @p x, @p y for the
     * scaling factor @p xScale and @p yScale.
     */
    bool contains(double x, double y, double xScale, double yScale) const override;

private:
    NormalizedPoint m_point;
};

/**
 * This class is an object rect that doesn't own the given pointer, i.e. won't delete it on destruction
 * @since 1.7
 */
class OKULARCORE_EXPORT NonOwningObjectRect : public ObjectRect
{
public:
    NonOwningObjectRect(double left, double top, double right, double bottom, bool ellipse, ObjectType type, void *object);
    ~NonOwningObjectRect() override;
};

/// @cond PRIVATE
/** @internal */
/** @internal */
template<typename T> T *givePtr(T &t)
{
    return &t;
}

/** @internal */
template<typename T> T &deref(T &t)
{
    return t;
}
/// @endcond

/**
 * @short An area with normalized coordinates, consisting of NormalizedShape objects.
 *
 * This is a template class to describe an area which consists of
 * multiple shapes of the same type, intersecting or non-intersecting.
 * The coordinates are normalized, and can be mapped to a reference area of defined size.
 * For more information about the normalized coordinate system, see NormalizedPoint.
 *
 * Class NormalizedShape \b must have the following functions/operators defined:
 * - bool contains( double, double ), whether it contains the given NormalizedPoint
 * - bool intersects( NormalizedShape )
 * - bool isNull()
 * - Shape geometry( int, int ), which maps to the reference area
 * - operator|=( NormalizedShape ), which unites two NormalizedShape's
 *
 * @see RegularAreaRect, NormalizedPoint
 */
template<class NormalizedShape, class Shape> class RegularArea : public QList<NormalizedShape>
{
public:
    /**
     * Returns whether this area contains the normalized point (@p x, @p y).
     */
    bool contains(double x, double y) const;

    /**
     * Returns whether this area contains a NormalizedShape object that equals @p shape.
     *
     * @note
     * The original NormalizedShape objects can be lost if simplify() was called.
     */
    bool contains(const NormalizedShape &shape) const;

    /**
     * Returns whether this area intersects with the given @p area.
     */
    bool intersects(const RegularArea<NormalizedShape, Shape> *area) const;

    /**
     * Returns whether the regular area intersects with the given @p shape.
     */
    bool intersects(const NormalizedShape &shape) const;

    /**
     * Appends the given @p area to this area.
     */
    void appendArea(const RegularArea<NormalizedShape, Shape> *area);

    /**
     * Appends the given @p shape to this area.
     */
    void appendShape(const NormalizedShape &shape, MergeSide side = MergeAll);

    /**
     * Simplifies this regular area by merging its intersecting subareas.
     * This might change the effective geometry of this area.
     */
    void simplify();

    /**
     * Returns whether the regular area is a null area.
     */
    bool isNull() const;

    /**
     * Returns the subareas of this regular area
     * mapped to a reference area of size @p xScale x @p yScale,
     * then translated by @p dx and @p dy.
     */
    QList<Shape> geometry(int xScale, int yScale, int dx = 0, int dy = 0) const;

    /**
     * Transforms the regular area with the operations defined by @p matrix.
     */
    void transform(const QTransform &matrix);
};

template<class NormalizedShape, class Shape> void RegularArea<NormalizedShape, Shape>::simplify()
{
#ifdef DEBUG_REGULARAREA
    int prev_end = this->count();
#endif
    int end = this->count() - 1, x = 0;
    for (int i = 0; i < end; ++i) {
        if (givePtr((*this)[x])->intersects(deref((*this)[i + 1]))) {
            deref((*this)[x]) |= deref((*this)[i + 1]);
            this->removeAt(i + 1);
            --end;
            --i;
        } else {
            x = i + 1;
        }
    }
#ifdef DEBUG_REGULARAREA
    qCDebug(OkularCoreDebug) << "from" << prev_end << "to" << this->count();
#endif
}

template<class NormalizedShape, class Shape> bool RegularArea<NormalizedShape, Shape>::isNull() const
{
    if (this->isEmpty()) {
        return true;
    }

    typename QList<NormalizedShape>::const_iterator it = this->begin(), itEnd = this->end();
    for (; it != itEnd; ++it) {
        if (!givePtr(*it)->isNull()) {
            return false;
        }
    }

    return true;
}

template<class NormalizedShape, class Shape> bool RegularArea<NormalizedShape, Shape>::intersects(const NormalizedShape &shape) const
{
    if (this->isEmpty()) {
        return false;
    }

    typename QList<NormalizedShape>::const_iterator it = this->begin(), itEnd = this->end();
    for (; it != itEnd; ++it) {
        if (!givePtr(*it)->isNull() && givePtr(*it)->intersects(shape)) {
            return true;
        }
    }

    return false;
}

template<class NormalizedShape, class Shape> bool RegularArea<NormalizedShape, Shape>::intersects(const RegularArea<NormalizedShape, Shape> *area) const
{
    if (this->isEmpty()) {
        return false;
    }

    typename QList<NormalizedShape>::const_iterator it = this->begin(), itEnd = this->end();
    for (; it != itEnd; ++it) {
        typename QList<NormalizedShape>::const_iterator areaIt = area->begin(), areaItEnd = area->end();
        for (; areaIt != areaItEnd; ++areaIt) {
            if (!(*it).isNull() && (*it).intersects(*areaIt)) {
                return true;
            }
        }
    }

    return false;
}

template<class NormalizedShape, class Shape> void RegularArea<NormalizedShape, Shape>::appendArea(const RegularArea<NormalizedShape, Shape> *area)
{
    typename QList<NormalizedShape>::const_iterator areaIt = area->begin(), areaItEnd = area->end();
    for (; areaIt != areaItEnd; ++areaIt) {
        this->append(*areaIt);
    }
}

template<class NormalizedShape, class Shape> void RegularArea<NormalizedShape, Shape>::appendShape(const NormalizedShape &shape, MergeSide side)
{
    int size = this->count();
    // if the list is empty, adds the shape normally
    if (size == 0) {
        this->append(shape);
    } else {
        bool intersection = false;
        NormalizedShape &last = (*this)[size - 1];
#define O_LAST givePtr(last)
#define O_LAST_R O_LAST->right
#define O_LAST_L O_LAST->left
#define O_LAST_T O_LAST->top
#define O_LAST_B O_LAST->bottom
#define O_NEW givePtr(shape)
#define O_NEW_R O_NEW->right
#define O_NEW_L O_NEW->left
#define O_NEW_T O_NEW->top
#define O_NEW_B O_NEW->bottom
        switch (side) {
        case MergeRight:
            intersection = (O_LAST_R >= O_NEW_L) && (O_LAST_L <= O_NEW_R) && ((O_LAST_T <= O_NEW_T && O_LAST_B >= O_NEW_B) || (O_LAST_T >= O_NEW_T && O_LAST_B <= O_NEW_B));
            break;
        case MergeBottom:
            intersection = (O_LAST_B >= O_NEW_T) && (O_LAST_T <= O_NEW_B) && ((O_LAST_R <= O_NEW_R && O_LAST_L >= O_NEW_L) || (O_LAST_R >= O_NEW_R && O_LAST_L <= O_NEW_L));
            break;
        case MergeLeft:
            intersection = (O_LAST_L <= O_NEW_R) && (O_LAST_R >= O_NEW_L) && ((O_LAST_T <= O_NEW_T && O_LAST_B >= O_NEW_B) || (O_LAST_T >= O_NEW_T && O_LAST_B <= O_NEW_B));
            break;
        case MergeTop:
            intersection = (O_LAST_T <= O_NEW_B) && (O_LAST_B >= O_NEW_T) && ((O_LAST_R <= O_NEW_R && O_LAST_L >= O_NEW_L) || (O_LAST_R >= O_NEW_R && O_LAST_L <= O_NEW_L));
            break;
        case MergeAll:
            intersection = O_LAST->intersects(shape);
            break;
        }
#undef O_LAST
#undef O_LAST_R
#undef O_LAST_L
#undef O_LAST_T
#undef O_LAST_B
#undef O_NEW
#undef O_NEW_R
#undef O_NEW_L
#undef O_NEW_T
#undef O_NEW_B
        // if the new shape intersects with the last shape in the list, then
        // merge it with that and delete the shape
        if (intersection) {
            deref((*this)[size - 1]) |= deref(shape);
        } else {
            this->append(shape);
        }
    }
}

template<class NormalizedShape, class Shape> bool RegularArea<NormalizedShape, Shape>::contains(double x, double y) const
{
    if (this->isEmpty()) {
        return false;
    }

    typename QList<NormalizedShape>::const_iterator it = this->begin(), itEnd = this->end();
    for (; it != itEnd; ++it) {
        if ((*it).contains(x, y)) {
            return true;
        }
    }

    return false;
}

template<class NormalizedShape, class Shape> bool RegularArea<NormalizedShape, Shape>::contains(const NormalizedShape &shape) const
{
    if (this->isEmpty()) {
        return false;
    }

    return QList<NormalizedShape>::contains(shape);
}

template<class NormalizedShape, class Shape> QList<Shape> RegularArea<NormalizedShape, Shape>::geometry(int xScale, int yScale, int dx, int dy) const
{
    if (this->isEmpty()) {
        return QList<Shape>();
    }

    QList<Shape> ret;
    Shape t;
    typename QList<NormalizedShape>::const_iterator it = this->begin(), itEnd = this->end();
    for (; it != itEnd; ++it) {
        t = givePtr(*it)->geometry(xScale, yScale);
        t.translate(dx, dy);
        ret.append(t);
    }

    return ret;
}

template<class NormalizedShape, class Shape> void RegularArea<NormalizedShape, Shape>::transform(const QTransform &matrix)
{
    if (this->isEmpty()) {
        return;
    }

    for (int i = 0; i < this->count(); ++i) {
        givePtr((*this)[i])->transform(matrix);
    }
}

/**
 * This is a list of NormalizedRect, to describe an area consisting of
 * multiple rectangles using normalized coordinates.
 *
 * This area can be mapped to a reference area, resulting in a list of QRects.
 * For more information about the normalized coordinate system, see NormalizedPoint.
 *
 * Okular uses this area e. g. to describe a text highlight area,
 * which consists of multiple, intersecting or non-intersecting rectangles.
 *
 * @see NormalizedRect, NormalizedPoint
 */
class OKULARCORE_EXPORT RegularAreaRect : public RegularArea<NormalizedRect, QRect>
{
public:
    RegularAreaRect();
    RegularAreaRect(const RegularAreaRect &rar);
    ~RegularAreaRect();

    RegularAreaRect &operator=(const RegularAreaRect &rar);

private:
    class Private;
    Private *const d;
};

/**
 * This class stores the geometry of a highlighting area in normalized coordinates,
 * together with highlighting specific information.
 */
class HighlightAreaRect : public RegularAreaRect
{
public:
    /**
     * Creates a new highlight area rect with the coordinates of
     * the given @p area.
     */
    explicit HighlightAreaRect(const RegularAreaRect *area = nullptr);

    /**
     * The search ID of the highlight owner.
     */
    int s_id;

    /**
     * The color of the highlight.
     */
    QColor color;
};

uint qHash(const Okular::NormalizedRect &r, uint seed = 0);
}

#ifndef QT_NO_DEBUG_STREAM
/**
 * Debug operator for normalized @p point.
 */
OKULARCORE_EXPORT QDebug operator<<(QDebug str, const Okular::NormalizedPoint &point);

/**
 * Debug operator for normalized @p rect.
 */
OKULARCORE_EXPORT QDebug operator<<(QDebug str, const Okular::NormalizedRect &rect);
#endif

#endif
