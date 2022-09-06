/*
    SPDX-FileCopyrightText: 2005 Albert Astals Cid <aacid@kde.org>

    Work sponsored by the LiMux project of the city of Munich:
    SPDX-FileCopyrightText: 2017 Klarälvdalens Datakonsult AB a KDAB Group company <info@kdab.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_GENERATOR_KIMGIO_H_
#define _OKULAR_GENERATOR_KIMGIO_H_

#include <core/document.h>
#include <core/generator.h>

#include <QImage>

class KIMGIOGenerator : public Okular::Generator
{
    Q_OBJECT
    Q_INTERFACES(Okular::Generator)

public:
    KIMGIOGenerator(QObject *parent, const QVariantList &args);
    ~KIMGIOGenerator() override;

    // [INHERITED] load a document and fill up the pagesVector
    bool loadDocument(const QString &fileName, QVector<Okular::Page *> &pagesVector) override;
    bool loadDocumentFromData(const QByteArray &fileData, QVector<Okular::Page *> &pagesVector) override;
    SwapBackingFileResult swapBackingFile(QString const &newFileName, QVector<Okular::Page *> &newPagesVector) override;

    // [INHERITED] print document using already configured kprinter
    Okular::Document::PrintError print(QPrinter &printer) override;

    // [INHERITED] document information
    Okular::DocumentInfo generateDocumentInfo(const QSet<Okular::DocumentInfo::Key> &keys) const override;

protected:
    bool doCloseDocument() override;
    QImage image(Okular::PixmapRequest *request) override;

private:
    bool loadDocumentInternal(const QByteArray &fileData, const QString &fileName, QVector<Okular::Page *> &pagesVector);

private:
    QImage m_img;
    Okular::DocumentInfo docInfo;
};

#endif
