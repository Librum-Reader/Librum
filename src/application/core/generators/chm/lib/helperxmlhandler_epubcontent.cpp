/*
    Kchmviewer - a CHM and EPUB file viewer with broad language support
    SPDX-FileCopyrightText: 2004-2014 George Yunaev gyunaev@ulduzsoft.com

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "helperxmlhandler_epubcontent.h"

HelperXmlHandler_EpubContent::HelperXmlHandler_EpubContent()
{
    m_state = STATE_NONE;
}

bool HelperXmlHandler_EpubContent::startElement(const QString&,
                                                const QString& localName,
                                                const QString&,
                                                const QXmlAttributes& atts)
{
    // <metadata> tag contains the medatada which goes into m_metadata
    if(localName == QLatin1String("metadata"))
    {
        m_state = STATE_IN_METADATA;
    }
    else if(localName == QLatin1String("manifest"))
    {
        m_state = STATE_IN_MANIFEST;
    }
    else if(localName == QLatin1String("spine"))
    {
        m_state = STATE_IN_SPINE;
    }
    else if(m_state == STATE_IN_METADATA)
    {  // we don't need to store the first 'metadata' here
        // Now handle the states
        m_tagname = localName;
    }
    else if(m_state == STATE_IN_MANIFEST && localName == QLatin1String("item"))
    {
        int idx_id = atts.index(QLatin1String("id"));
        int idx_href = atts.index(QLatin1String("href"));
        int idx_mtype = atts.index(QLatin1String("media-type"));

        if(idx_id == -1 || idx_href == -1 || idx_mtype == -1)
        {
            return false;
        }

        manifest[atts.value(idx_id)] = atts.value(idx_href);

        if(atts.value(idx_mtype) == QLatin1String("application/x-dtbncx+xml"))
        {
            tocname = atts.value(idx_href);
        }

        // qDebug() << "MANIFEST: " << atts.value( idx_id ) << "->" <<
        // atts.value( idx_href );
    }
    else if(m_state == STATE_IN_SPINE && localName == QLatin1String("itemref"))
    {
        int idx = atts.index(QLatin1String("idref"));

        if(idx == -1)
        {
            return false;
        }

        spine.push_back(atts.value(idx));
        // qDebug() << "SPINE: " << atts.value( idx );
    }

    return true;
}

bool HelperXmlHandler_EpubContent::characters(const QString& ch)
{
    if(m_state == STATE_IN_METADATA && !m_tagname.isEmpty() &&
       !ch.trimmed().isEmpty())
    {
        // Some metadata may be duplicated; we concantenate them with |
        if(metadata.contains(m_tagname))
        {
            metadata[m_tagname].append(QStringLiteral("|"));
            metadata[m_tagname].append(ch.trimmed());
        }
        else
        {
            metadata[m_tagname] = ch.trimmed();
        }

        // qDebug() << "METATAG: " << m_tagname << " " << metadata[ m_tagname ];
    }

    return true;
}

bool HelperXmlHandler_EpubContent::endElement(const QString&, const QString&,
                                              const QString& qName)
{
    if(qName == QLatin1String("manifest") ||
       qName == QLatin1String("metadata") || qName == QLatin1String("spine"))
    {
        m_state = STATE_NONE;
    }

    return true;
}
