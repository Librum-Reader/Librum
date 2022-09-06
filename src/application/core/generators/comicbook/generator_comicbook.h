/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef GENERATOR_COMICBOOK_H
#define GENERATOR_COMICBOOK_H

#include <core/generator.h>

#include "document.h"

class ComicBookGenerator : public Okular::Generator
{
    Q_OBJECT
    Q_INTERFACES(Okular::Generator)

public:
    ComicBookGenerator(QObject *parent, const QVariantList &args);
    ~ComicBookGenerator() override;

    // [INHERITED] load a document and fill up the pagesVector
    bool loadDocument(const QString &fileName, QVector<Okular::Page *> &pagesVector) override;

    // [INHERITED] print document using already configured kprinter
    Okular::Document::PrintError print(QPrinter &printer) override;

protected:
    bool doCloseDocument() override;
    QImage image(Okular::PixmapRequest *request) override;

private:
    ComicBook::Document mDocument;
};

#endif
