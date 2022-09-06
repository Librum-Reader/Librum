// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
//
// pageNumber.h
//
// Part of KVIEWSHELL - A framework for multipage text/gfx viewers
//
// SPDX-FileCopyrightText: 2004 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef PAGENUMBER_H
#define PAGENUMBER_H

#include <QGlobalStatic>

/** \brief Class to represent a page number

The class PageNumber is really nothing but an alias for quint16, and
can be casted to and from quint16. It is used in kviewshell to remind
the programmer of the convention that page numbers start at '1' (for
'first page'), and that the value '0' means 'illegal page number' or
'no page number'. Accordingly, the value '0' is also named
PageNumber::invalidPage, and there is a trivial method isInvalid()
that checks if the page number is 0.

@author Stefan Kebekus <kebekus@kde.org>
@version 1.0 0
*/

class PageNumber
{
public:
    enum pageNums {
        invalidPage = 0 /*! Invalid page number */
    };

    /** The default constructor sets the page number to 'invalidPage' */
    PageNumber()
    {
        pgNum = invalidPage;
    }

    /** \brief Constructor that sets the page number

    @param num page number that is set initially
    */
    explicit PageNumber(quint16 num)
    {
        pgNum = num;
    }

    /** \brief this method implements typecasts from quint16 */
    PageNumber &operator=(const quint16 p)
    {
        pgNum = p;
        return *this;
    }

    /** \brief This method implements typecasts to quint16 */
    explicit operator quint16() const
    {
        return pgNum;
    }

    bool operator>(const PageNumber other) const
    {
        return pgNum > other.pgNum;
    }

    /** \brief Checks if the page number is invalid

    @returns true, if pgNum != invalidPage, i.e., does not equal 0
    */
    bool isValid() const
    {
        return (pgNum != invalidPage);
    }

private:
    /** \brief Single number that represents the page number */
    quint16 pgNum;
};

#endif
