/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_PAGESIZE_H_
#define _OKULAR_PAGESIZE_H_

#include <QList>
#include <QSharedDataPointer>
#include <QString>

#include "okularcore_export.h"

namespace Okular
{
class PageSizePrivate;

/**
 * @short A small class that represents the size of a page.
 */
class OKULARCORE_EXPORT PageSize
{
public:
    typedef QList<PageSize> List;

    /**
     * Construct a null page size.
     * @see isNull()
     */
    PageSize();
    /**
     * Construct a page size with the specified @p width and @p height,
     * having the ID @p name.
     */
    PageSize(double width, double height, const QString &name);
    /**
     * Copy constructor.
     */
    PageSize(const PageSize &pageSize);
    ~PageSize();

    /**
     * Returns the width of the page size.
     */
    double width() const;
    /**
     * Returns the height of the page size.
     */
    double height() const;
    /**
     * Returns the ID of the page size.
     */
    QString name() const;

    /**
     * Whether the page size is null.
     */
    bool isNull() const;

    PageSize &operator=(const PageSize &pageSize);

    /**
     * Comparison operator.
     */
    bool operator==(const PageSize &pageSize) const;

    bool operator!=(const PageSize &pageSize) const;

private:
    /// @cond PRIVATE
    friend class PageSizePrivate;
    /// @endcond
    QSharedDataPointer<PageSizePrivate> d;
};

}

#endif
