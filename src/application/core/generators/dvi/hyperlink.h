// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
//
// Class: hyperlink
//
// Part of KDVI- A previewer for TeX DVI files.
//
// SPDX-FileCopyrightText: 2004-2005 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef _hyperlink_h_
#define _hyperlink_h_

#include <QRect>
#include <QString>

/** Represents a named, rectangular region in a rendered documentPage

    This trivial class is used in the documentPage class to represent
    a hyperlink in a rendered documentPage.

    @author Stefan Kebekus <kebekus@kde.org>
    @version 1.0.0
*/

class Hyperlink
{
public:
    /** \brief Default Constructor

    The default constructor leaves all fields uninitialized.
    */
    Hyperlink()
    {
    }

    /** \brief Constructor

    Trivial constructor leaves that initialized all members.

    @param bl value for the baseline field
    @param re value for the box
    @param lT valus for the text field
    */
    Hyperlink(quint32 bl, const QRect re, const QString &lT)
        : baseline(bl)
        , box(re)
        , linkText(lT)
    {
    }

    /** \brief Base line of a hyperlink

    This field specifies the Y-coordinate of the base line of the
    bounding box in the same coordinates that were used when the
    associated documentPage was rendered by the
    documentRenderer.drawPage() method. It is used to underline
    hyperlinks in blue. Note that this field does generally differ from
    the Y-coordinate of the bottom of the bounding box, e.g. if the text
    in the box contains characters with underlengths, such as 'y', 'j'
    or 'g'.
     */
    quint32 baseline;

    /** \brief Bounding box of the text or hyperlink

    This rectangle specifies where on the page the hyperlink is
    found. It uses the same coordinates that were used when the
    associated documentPage was rendered by the
    documentRenderer.drawPage() method. The box is used to determine if
    the mouse pointer hovers over the link.
    */
    QRect box;

    /** \brief Name of the region

    This field contains the name of the target,
    e.g. "http://www.kde.org". If the Hyperlink class is used to
    represent text, then the text is stored here.
    */
    QString linkText;
};

#endif
