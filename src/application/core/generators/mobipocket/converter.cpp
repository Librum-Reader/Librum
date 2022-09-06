/*
    SPDX-FileCopyrightText: 2008 Jakub Stachowski <qbast@go2.pl>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "converter.h"

#include <QAbstractTextDocumentLayout>
#include <QDebug>
#include <QFile>
#include <QTextBlock>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QTextFrame>

#include <KLocalizedString>
#include <core/action.h>

using namespace Mobi;

Converter::Converter()
{
}

Converter::~Converter()
{
}

void Converter::handleMetadata(const QMap<Mobipocket::Document::MetaKey, QString> &metadata)
{
    QMapIterator<Mobipocket::Document::MetaKey, QString> it(metadata);
    while (it.hasNext()) {
        it.next();
        switch (it.key()) {
        case Mobipocket::Document::Title:
            Q_EMIT addMetaData(Okular::DocumentInfo::Title, it.value());
            break;
        case Mobipocket::Document::Author:
            Q_EMIT addMetaData(Okular::DocumentInfo::Author, it.value());
            break;
        case Mobipocket::Document::Description:
            Q_EMIT addMetaData(Okular::DocumentInfo::Description, it.value());
            break;
        case Mobipocket::Document::Subject:
            Q_EMIT addMetaData(Okular::DocumentInfo::Subject, it.value());
            break;
        case Mobipocket::Document::Copyright:
            Q_EMIT addMetaData(Okular::DocumentInfo::Copyright, it.value());
            break;
        }
    }
}

QTextDocument *Converter::convert(const QString &fileName)
{
    MobiDocument *newDocument = new MobiDocument(fileName);
    if (!newDocument->mobi()->isValid()) {
        Q_EMIT error(i18n("Error while opening the Mobipocket document."), -1);
        delete newDocument;
        return nullptr;
    }
    if (newDocument->mobi()->hasDRM()) {
        Q_EMIT error(i18n("This book is protected by DRM and can be displayed only on designated device"), -1);
        delete newDocument;
        return nullptr;
    }

    handleMetadata(newDocument->mobi()->metadata());
    newDocument->setPageSize(QSizeF(600, 800));

    QTextFrameFormat frameFormat;
    frameFormat.setMargin(20);
    QTextFrame *rootFrame = newDocument->rootFrame();
    rootFrame->setFrameFormat(frameFormat);
    QMap<QString, QPair<int, int>> links;
    QMap<QString, QTextBlock> targets;

    // go over whole document and add all <a> tags to links or targets map
    for (QTextBlock it = newDocument->begin(); it != newDocument->end(); it = it.next()) {
        for (QTextBlock::iterator fit = it.begin(); !fit.atEnd(); ++fit) {
            QTextFragment frag = fit.fragment();
            QTextCharFormat format = frag.charFormat();
            if (!format.isAnchor()) {
                continue;
            }
            // link
            if (!format.anchorHref().isEmpty()) {
                links[format.anchorHref()] = QPair<int, int>(frag.position(), frag.position() + frag.length());
            }
            const QStringList anchors = format.anchorNames();
            if (!anchors.isEmpty()) {
                // link targets
                for (const QString &name : anchors) {
                    targets[QLatin1Char('#') + name] = it;
                }
            }
        }
    }

    // create link actions
    QMapIterator<QString, QPair<int, int>> it(links);
    while (it.hasNext()) {
        it.next();
        QUrl u(it.key());
        // external or internal link
        if (!u.isRelative()) {
            Q_EMIT addAction(new Okular::BrowseAction(QUrl(it.key())), it.value().first, it.value().second);
        } else {
            // is there valid target?
            if (!targets.contains(it.key()) || !targets[it.key()].isValid()) {
                continue;
            }
            Q_EMIT addAction(new Okular::GotoAction(QString(), calculateViewport(newDocument, targets[it.key()])), it.value().first, it.value().second);
        }
    }

    return newDocument;
}
