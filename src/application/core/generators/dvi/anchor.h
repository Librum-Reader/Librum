// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
//
// anchor.h
//
// Part of KVIEWSHELL - A framework for multipage text/gfx viewers
//
// SPDX-FileCopyrightText: 2004-2005 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef ANCHOR_H
#define ANCHOR_H

#include "length.h"
#include "pageNumber.h"

/** \brief Page number and vertical position in physical coordinates

This very simple class contains a page number and a vertical position
in physical coordinates. The vertical position is given by the
distance from the top of the page. Anchors are completely independent
of documents, there is no need for a document to exists that contains
the given page, nor does the page number need to be valid.

@author Stefan Kebekus <kebekus@kde.org>
@version 1.0 0
*/

class Anchor
{
public:
    /** \brief Constructs an anchor that points to an invalid page */
    Anchor()
    {
        page = 0;
    }

    /** \brief Constructs an anchor that points to a given position on a
        given page

        The class contains no code to make sure in any way that the page
        number pg exists, and that page pg, if it exists, is taller than
        distance_from_top

        @param pg number of the page
        @param _distance_from_top distance from the top of the page
    */
    Anchor(quint16 pg, const Length _distance_from_top)
        : page(pg)
        , distance_from_top(_distance_from_top)
    {
    }

    /** \brief quick validity check for anchors

    @returns true if the page number is valid, and 0mm <= distance_from_top <= 2m
    */
    bool isValid() const
    {
        return page.isValid() && (0.0 <= distance_from_top.getLength_in_mm()) && (distance_from_top.getLength_in_mm() <= 2000.0);
    }

    /** \brief Page number that this anchor point to */
    PageNumber page;

    /** \brief Distance from the top of the page in inch */
    Length distance_from_top;
};

#endif
