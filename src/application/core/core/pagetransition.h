/*
    SPDX-FileCopyrightText: 2005 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_PAGETRANSITION_H_
#define _OKULAR_PAGETRANSITION_H_

#include "okularcore_export.h"

namespace Okular
{
/**
 * @short Information object for the transition effect of a page.
 *
 * This class encapsulates the information about the effect of
 * a page transition. It supports mainly the transition effects as
 * defined in PDF specification 1.6.
 */
class OKULARCORE_EXPORT PageTransition
{
public:
    /**
     * Describes the type of transition effect.
     */
    enum Type
    {
        Replace,
        Split,
        Blinds,
        Box,
        Wipe,
        Dissolve,
        Glitter,
        Fly,
        Push,
        Cover,
        Uncover,
        Fade
    };

    /**
     * Describes the alignment that is applied to the @ref Type
     * of transition effect.
     */
    enum Alignment
    {
        Horizontal,
        Vertical
    };

    /**
     * Describes the direction that is applied to the @ref Type
     * of transition effect.
     */
    enum Direction
    {
        Inward,
        Outward
    };

    /**
     * Creates a new page transition of the given @p type.
     *
     * If no type is given, the normal @ref Replace transition is used.
     */
    explicit PageTransition(Type type = Replace);

    /**
     * Creates a new page transition from an @p other.
     */
    PageTransition(const PageTransition& other);
    PageTransition& operator=(const PageTransition& other);

    /**
     * Destroys the page transition.
     */
    ~PageTransition();

    /**
     * Returns the type of the transition.
     */
    Type type() const;

    /**
     * Returns the duration of the transition in seconds.
     */
    double duration() const;

    /**
     * Returns the alignment of the transition.
     */
    Alignment alignment() const;

    /**
     * Returns the direction of motion of the transition.
     */
    Direction direction() const;

    /**
     * Returns the angle of rotation of the transition.
     */
    int angle() const;

    /**
     * Returns the starting or ending scale (Only if type == 'Fly').
     */
    double scale() const;

    /**
     * Returns true if the area to be flown is rectangular and opaque (Only if
     * type == 'Fly').
     */
    bool isRectangular() const;

    /**
     * Sets the @p type of the transition (@ref Type).
     */
    void setType(Type type);

    /**
     * Sets the @p duration in seconds for the transition.
     */
    void setDuration(double duration);

    /**
     * Sets the @p alignment of the transition (@ref Alignment).
     */
    void setAlignment(Alignment alignment);

    /**
     * Sets the @p direction of the transition (@see Direction).
     */
    void setDirection(Direction direction);

    /**
     * Sets the moving @p angle of the transition.
     */
    void setAngle(int angle);

    /**
     * Sets the starting or ending scale of the transition (Only if type ==
     * 'Fly').
     */
    void setScale(double scale);

    /**
     * Sets whether the area to be flown is rectangular and opaque (Only if type
     * == 'Fly').
     */
    void setIsRectangular(bool rectangular);

private:
    class Private;
    Private* const d;
};

}  // namespace Okular

#endif
