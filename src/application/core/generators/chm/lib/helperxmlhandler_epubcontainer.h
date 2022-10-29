/*
    Kchmviewer - a CHM and EPUB file viewer with broad language support
    SPDX-FileCopyrightText: 2004-2014 George Yunaev gyunaev@ulduzsoft.com

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef HELPERXMLHANDLER_EPUBCONTAINER_H
#define HELPERXMLHANDLER_EPUBCONTAINER_H

#include <QXmlDefaultHandler>

class HelperXmlHandler_EpubContainer : public QXmlDefaultHandler
{
public:
    // Overridden members
    bool startElement(const QString& namespaceURI, const QString& localName,
                      const QString& qName,
                      const QXmlAttributes& atts) override;

    // The content path
    QString contentPath;
};

#endif  // HELPERXMLHANDLER_EPUBCONTAINER_H
