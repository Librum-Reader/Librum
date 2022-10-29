/*
    Kchmviewer - a CHM and EPUB file viewer with broad language support
    SPDX-FileCopyrightText: 2004-2014 George Yunaev gyunaev@ulduzsoft.com

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef HELPERXMLHANDLER_EPUBTOC_H
#define HELPERXMLHANDLER_EPUBTOC_H

#include <QXmlDefaultHandler>
#include "ebook_epub.h"

class HelperXmlHandler_EpubTOC : public QXmlDefaultHandler
{
public:
    explicit HelperXmlHandler_EpubTOC(EBook_EPUB* epub);

    QList<EBookTocEntry> entries;

private:
    // Overridden members
    bool startElement(const QString& namespaceURI, const QString& localName,
                      const QString& qName,
                      const QXmlAttributes& atts) override;
    bool characters(const QString& ch) override;
    bool endElement(const QString& namespaceURI, const QString& localName,
                    const QString& qName) override;
    void checkNewTocEntry();

    bool m_inNavMap;
    bool m_inText;
    unsigned int m_indent;
    QString m_lastId;
    QString m_lastTitle;
    EBook_EPUB* m_epub;
};

#endif  // HELPERXMLHANDLER_EPUBTOC_H
