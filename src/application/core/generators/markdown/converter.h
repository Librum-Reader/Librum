/*
    SPDX-FileCopyrightText: 2017 Julian Wolff <wolff@julianwolff.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MARKDOWN_CONVERTER_H
#define MARKDOWN_CONVERTER_H

#include <core/textdocumentgenerator.h>

#include <QDir>
#include <QTextFragment>

class QTextBlock;
class QTextFrame;

namespace Markdown
{
class Converter : public Okular::TextDocumentConverter
{
    Q_OBJECT

public:
    Converter();
    ~Converter() override;

    QTextDocument *convert(const QString &fileName) override;

    void convertAgain();

    void setFancyPantsEnabled(bool b)
    {
        m_isFancyPantsEnabled = b;
    }
    bool isFancyPantsEnabled() const
    {
        return m_isFancyPantsEnabled;
    }

    QTextDocument *convertOpenFile();

private:
    void extractLinks(QTextFrame *parent, QHash<QString, QTextFragment> &internalLinks, QHash<QString, QTextBlock> &documentAnchors);
    void extractLinks(const QTextBlock &parent, QHash<QString, QTextFragment> &internalLinks, QHash<QString, QTextBlock> &documentAnchors);
    void convertImages(QTextFrame *parent, const QDir &dir, QTextDocument *textDocument);
    void convertImages(const QTextBlock &parent, const QDir &dir, QTextDocument *textDocument);
    void setImageSize(QTextImageFormat &format, const qreal specifiedWidth, const qreal specifiedHeight, const qreal originalWidth, const qreal originalHeight);

    FILE *m_markdownFile;
    QDir m_fileDir;
    bool m_isFancyPantsEnabled;
};

}

#endif
