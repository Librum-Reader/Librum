/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

// local includes
#include "pagesize.h"

using namespace Okular;

class Okular::PageSizePrivate : public QSharedData
{
public:
    PageSizePrivate()
        : m_width(0)
        , m_height(0)
    {
    }

    bool operator==(const PageSizePrivate &rhs) const
    {
        return m_width == rhs.m_width && m_height == rhs.m_height && m_name == rhs.m_name;
    }

    double m_width;
    double m_height;
    QString m_name;
};

PageSize::PageSize()
{
}

PageSize::PageSize(double width, double height, const QString &name)
    : d(new PageSizePrivate)
{
    d->m_width = width;
    d->m_height = height;
    d->m_name = name;
}

PageSize::PageSize(const PageSize &pageSize)
    : d(pageSize.d)
{
}

PageSize::~PageSize()
{
}

double PageSize::width() const
{
    if (!d) {
        return 0;
    }

    return d->m_width;
}

double PageSize::height() const
{
    if (!d) {
        return 0;
    }

    return d->m_height;
}

QString PageSize::name() const
{
    if (!d) {
        return QString();
    }

    return d->m_name;
}

bool PageSize::isNull() const
{
    if (!d) {
        return true;
    }

    return d->m_width == 0 && d->m_height == 0 && d->m_name.isEmpty();
}

bool PageSize::operator==(const PageSize &pageSize) const
{
    // 1st: we're null: check if the other is null too
    if (!d) {
        return !pageSize.d;
    }
    // 2nd: we're not null, return if the other is null
    if (!pageSize.d) {
        return false;
    }

    // 3rd: normal == check
    return *d == *pageSize.d;
}

bool PageSize::operator!=(const PageSize &pageSize) const
{
    return !operator==(pageSize);
}

PageSize &PageSize::operator=(const PageSize &pageSize)
{
    if (this == &pageSize) {
        return *this;
    }

    d = pageSize.d;
    return *this;
}
