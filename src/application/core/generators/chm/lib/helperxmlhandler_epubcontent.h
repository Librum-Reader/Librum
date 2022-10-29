/*
    Kchmviewer - a CHM and EPUB file viewer with broad language support
    SPDX-FileCopyrightText: 2004-2014 George Yunaev gyunaev@ulduzsoft.com

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef HELPERXMLHANDLER_EPUBCONTENT_H
#define HELPERXMLHANDLER_EPUBCONTENT_H

#include <QMap>
#include <QString>
#include <QXmlDefaultHandler>

class HelperXmlHandler_EpubContent : public QXmlDefaultHandler
{
public:
    HelperXmlHandler_EpubContent();

    // Keep the tag-associated metadata
    QMap<QString, QString> metadata;

    // Manifest storage, id -> href
    QMap<QString, QString> manifest;

    // Spline storage
    QList<QString> spine;

    // TOC (NCX) filename
    QString tocname;

private:
    enum State
    {
        STATE_NONE,
        STATE_IN_METADATA,
        STATE_IN_MANIFEST,
        STATE_IN_SPINE
    };

    bool startElement(const QString& namespaceURI, const QString& localName,
                      const QString& qName,
                      const QXmlAttributes& atts) override;
    bool characters(const QString& ch) override;
    bool endElement(const QString& namespaceURI, const QString& localName,
                    const QString& qName) override;

    // Tracking
    State m_state;
    QString m_tagname;
};

#endif  // HELPERXMLHANDLER_EPUBCONTENT_H
