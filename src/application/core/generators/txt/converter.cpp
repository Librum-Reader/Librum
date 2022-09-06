/*
    SPDX-FileCopyrightText: 2013 Azat Khuzhin <a3at.mail@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "converter.h"

#include <QTextFrame>

#include "document.h"

using namespace Txt;

Converter::Converter()
{
}

Converter::~Converter()
{
}

QTextDocument *Converter::convert(const QString &fileName)
{
    Document *textDocument = new Document(fileName);

    textDocument->setPageSize(QSizeF(600, 800));

    QTextFrameFormat frameFormat;
    frameFormat.setMargin(20);

    QTextFrame *rootFrame = textDocument->rootFrame();
    rootFrame->setFrameFormat(frameFormat);

    Q_EMIT addMetaData(Okular::DocumentInfo::MimeType, QStringLiteral("text/plain"));

    return textDocument;
}
