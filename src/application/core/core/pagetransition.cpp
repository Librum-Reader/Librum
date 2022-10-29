/*
    SPDX-FileCopyrightText: 2005 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

// local includes
#include "pagetransition.h"

using namespace Okular;

/** class Okular::PageTransition **/

class PageTransition::Private
{
public:
    explicit Private(Type type) :
        m_type(type),
        m_duration(1),
        m_alignment(Horizontal),
        m_direction(Inward),
        m_angle(0),
        m_scale(1.0),
        m_rectangular(false)
    {
    }

    Type m_type;
    double m_duration;
    Alignment m_alignment;
    Direction m_direction;
    int m_angle;
    double m_scale;
    bool m_rectangular;
};

PageTransition::PageTransition(Type type) :
    d(new Private(type))
{
}

PageTransition::PageTransition(const PageTransition& other) :
    d(new Private(*other.d))
{
}

PageTransition& PageTransition::operator=(const PageTransition& other)
{
    if(this == &other)
    {
        return *this;
    }

    *d = *other.d;

    return *this;
}

PageTransition::~PageTransition()
{
    delete d;
}

PageTransition::Type PageTransition::type() const
{
    return d->m_type;
}

double PageTransition::duration() const
{
    return d->m_duration;
}

PageTransition::Alignment PageTransition::alignment() const
{
    return d->m_alignment;
}

PageTransition::Direction PageTransition::direction() const
{
    return d->m_direction;
}

int PageTransition::angle() const
{
    return d->m_angle;
}

double PageTransition::scale() const
{
    return d->m_scale;
}

bool PageTransition::isRectangular() const
{
    return d->m_rectangular;
}

void PageTransition::setType(Type type)
{
    d->m_type = type;
}

void PageTransition::setDuration(double duration)
{
    d->m_duration = duration;
}

void PageTransition::setAlignment(Alignment alignment)
{
    d->m_alignment = alignment;
}

void PageTransition::setDirection(Direction direction)
{
    d->m_direction = direction;
}

void PageTransition::setAngle(int angle)
{
    d->m_angle = angle;
}

void PageTransition::setScale(double scale)
{
    d->m_scale = scale;
}

void PageTransition::setIsRectangular(bool rectangular)
{
    d->m_rectangular = rectangular;
}
