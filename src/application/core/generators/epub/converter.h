/*
    SPDX-FileCopyrightText: 2008 Ely Levy <elylevy@cs.huji.ac.il>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef EPUB_CONVERTER_H
#define EPUB_CONVERTER_H

#include <QFont>
#include <core/document.h>
#include <core/textdocumentgenerator.h>

#include "epubdocument.h"

namespace Epub
{
class Converter : public Okular::TextDocumentConverter
{
    Q_OBJECT

public:
    Converter();
    ~Converter() override;

    QTextDocument *convert(const QString &fileName) override;

private:
    void _emitData(Okular::DocumentInfo::Key key, enum epub_metadata type);
    void _handle_anchors(const QTextBlock &start, const QString &name);
    void _insert_local_links(const QString &key, const QPair<int, int> value);
    EpubDocument *mTextDocument;
    QFont mFont;

    QHash<QString, QTextBlock> mSectionMap;
    QHash<QString, QVector<QPair<int, int>>> mLocalLinks;
};
}

#endif
