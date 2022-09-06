/*
    SPDX-FileCopyrightText: 2006 Luigi Toscano <luigi.toscano@tiscali.it>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _dvipageinfo_h_
#define _dvipageinfo_h_

#include "hyperlink.h"
#include "pageNumber.h"
#include "textBox.h"
#include <QPixmap>
#include <QVector>

class dviPageInfo
{
public:
    QImage img;
    int width, height;
    double resolution;
    PageNumber pageNumber;

    dviPageInfo();

    virtual ~dviPageInfo();

    dviPageInfo(const dviPageInfo &) = delete;
    dviPageInfo &operator=(const dviPageInfo &) = delete;

    virtual void clear();

    /** \brief List of source hyperlinks
     */
    QVector<Hyperlink> sourceHyperLinkList;

    /** \brief Hyperlinks on the document page
     */
    QVector<Hyperlink> hyperLinkList;
    QVector<TextBox> textBoxList;
};

/* quick&dirty hack to cheat the dviRenderer class... */
#define RenderedDviPagePixmap dviPageInfo
#define RenderedDocumentPagePixmap dviPageInfo
#endif
