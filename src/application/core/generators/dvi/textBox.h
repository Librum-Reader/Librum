// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
//
// Class: textBox
//
// Part of KDVI- A previewer for TeX DVI files.
//
// SPDX-FileCopyrightText: 2004-2005 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef _textbox_h_
#define _textbox_h_

#include <QRect>
#include <QString>

/** Represents a rectangular region in a RenderedDocumentPage that contains text

    This trivial class is used in the RenderedDocumentPage class to
    give a non-graphical representation of text in a rendered document
    page. This is used, e.g. by text search and the text selection
    functions that need to know the contents and the position of text
    on a page

    @author Stefan Kebekus <kebekus@kde.org>
    @version 1.0.0
*/

class TextBox
{
public:
    /** \brief Default Constructor

    The default constructor leaves all fields uninitialized.
    */
    TextBox()
    {
    }

    /** \brief Constructor

    Trivial constructor leaves that initialized all members.

    @param re value for the box
    @param lT valus for the text field
    */
    TextBox(const QRect re, const QString &lT)
        : box(re)
        , text(lT)
    {
    }

    /** \brief Bounding box of the text or hyperlink

    This rectangle specifies where on the page the text or hyperlink is
    found. It uses the same coordinates that were used when the
    associated documentPage was rendered by the
    documentRenderer.drawPage() method. The contents of the box is
    graphically inverted to indicate marked text.
    */
    QRect box;

    /** \brief Name of the region

    The text associated with the box is stored here.
    */
    QString text;
};

#endif
