/*
    SPDX-FileCopyrightText: 2008 Tobias Koenig <tokoe@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_GENERATOR_FAX_H
#define OKULAR_GENERATOR_FAX_H

#include <core/generator.h>
#include <QImage>
#include "faxdocument.h"

class FaxGenerator : public Okular::Generator
{
    Q_OBJECT
    Q_INTERFACES(Okular::Generator)

public:
    FaxGenerator(QObject* parent, const QVariantList& args);
    ~FaxGenerator() override;

    bool loadDocument(const QString& fileName,
                      QVector<Okular::Page*>& pagesVector) override;

    Okular::DocumentInfo generateDocumentInfo(
        const QSet<Okular::DocumentInfo::Key>& keys) const override;

    Okular::Document::PrintError print(QPrinter& printer) override;

protected:
    bool doCloseDocument() override;
    QImage image(Okular::PixmapRequest* request) override;

private:
    QImage m_img;
    FaxDocument::DocumentType m_type;
};

#endif
