/*
    SPDX-FileCopyrightText: 2008 Jakub Stachowski <qbast@go2.pl>

    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "mobidocument.h"
#include <qmobipocket/mobipocket.h>
#include <qmobipocket/qfilestream.h>
#include <QApplication>  // Because of the HACK
#include <QColor>
#include <QDebug>
#include <QFile>
#include <QPalette>  // Because of the HACK
#include <QRegExp>

using namespace Mobi;

MobiDocument::MobiDocument(const QString& fileName) :
    QTextDocument()
{
    file = new Mobipocket::QFileStream(fileName);
    doc = new Mobipocket::Document(file);
    if(doc->isValid())
    {
        QString text = doc->text();
        QString header = text.left(1024);
        if(header.contains(QStringLiteral("<html>")) ||
           header.contains(QStringLiteral("<HTML>")))
        {
            // HACK BEGIN Get the links without CSS to be blue
            //            Remove if Qt ever gets fixed and the code in
            //            textdocumentgenerator.cpp works
            const QPalette orig = qApp->palette();
            QPalette p = orig;
            p.setColor(QPalette::Link, Qt::blue);
            qApp->setPalette(p);
            // HACK END

            setHtml(fixMobiMarkup(text));

            // HACK BEGIN
            qApp->setPalette(orig);
            // HACK END
        }
        else
        {
            setPlainText(text);
        }
    }
}

MobiDocument::~MobiDocument()
{
    delete doc;
    delete file;
}

QVariant MobiDocument::loadResource(int type, const QUrl& name)
{
    if(type != QTextDocument::ImageResource ||
       name.scheme() != QString(QStringLiteral("pdbrec")))
    {
        return QVariant();
    }
    bool ok;
    quint16 recnum = name.path().midRef(1).toUShort(&ok);
    if(!ok || recnum >= doc->imageCount())
    {
        return QVariant();
    }

    QVariant resource;
    resource.setValue(doc->getImage(recnum - 1));
    addResource(type, name, resource);

    return resource;
}

// starting from 'pos', find position in the string that is not inside a tag
int outsideTag(const QString& data, int pos)
{
    for(int i = pos - 1; i >= 0; i--)
    {
        if(data[i] == QLatin1Char('>'))
        {
            return pos;
        }
        if(data[i] == QLatin1Char('<'))
        {
            return i;
        }
    }
    return pos;
}

QString MobiDocument::fixMobiMarkup(const QString& data)
{
    QString ret = data;
    QMap<int, QString> anchorPositions;
    static QRegExp anchors(
        QStringLiteral(
            "<a(?: "
            "href=\"[^\"]*\"){0,1}[\\s]+filepos=['\"]{0,1}([\\d]+)[\"']{0,1}"),
        Qt::CaseInsensitive);
    int pos = 0;

    // find all link destinations
    while((pos = anchors.indexIn(data, pos)) != -1)
    {
        int filepos = anchors.cap(1).toUInt();
        if(filepos)
        {
            anchorPositions[filepos] = anchors.cap(1);
        }
        pos += anchors.matchedLength();
    }

    // put HTML anchors in all link destinations
    int offset = 0;
    QMapIterator<int, QString> it(anchorPositions);
    while(it.hasNext())
    {
        it.next();
        // link pointing outside the document, ignore
        if((it.key() + offset) >= ret.size())
        {
            continue;
        }
        int fixedpos = outsideTag(ret, it.key() + offset);
        ret.insert(fixedpos, QStringLiteral("<a name=\"") + it.value() +
                                 QStringLiteral("\">&nbsp;</a>"));
        // inserting anchor shifts all offsets after the anchor
        offset += 21 + it.value().size();
    }

    // replace links referencing filepos with normal internal links
    ret.replace(anchors, QStringLiteral("<a href=\"#\\1\""));
    // Mobipocket uses strange variang of IMG tags: <img recindex="3232"> where
    // recindex is number of record containing image
    static QRegExp imgs(QStringLiteral("<img.*recindex=\"([\\d]*)\".*>"),
                        Qt::CaseInsensitive);

    imgs.setMinimal(true);
    ret.replace(imgs, QStringLiteral("<img src=\"pdbrec:/\\1\">"));
    ret.replace(QStringLiteral("<mbp:pagebreak/>"),
                QStringLiteral("<p style=\"page-break-after:always\"></p>"));

    return ret;
}
