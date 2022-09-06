/*
    SPDX-FileCopyrightText: 2006-2008 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef POPPLEREMBEDDEDFILE_H
#define POPPLEREMBEDDEDFILE_H

#include <poppler-qt5.h>

#include <core/document.h>

class PDFEmbeddedFile : public Okular::EmbeddedFile
{
public:
    explicit PDFEmbeddedFile(Poppler::EmbeddedFile *f)
        : ef(f)
    {
    }

    QString name() const override
    {
        return ef->name();
    }

    QString description() const override
    {
        return ef->description();
    }

    QByteArray data() const override
    {
        return ef->data();
    }

    int size() const override
    {
        int s = ef->size();
        return s <= 0 ? -1 : s;
    }

    QDateTime modificationDate() const override
    {
        return ef->modDate();
    }

    QDateTime creationDate() const override
    {
        return ef->createDate();
    }

private:
    Poppler::EmbeddedFile *ef;
};

#endif
