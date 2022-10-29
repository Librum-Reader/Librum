/*
    SPDX-FileCopyrightText: 2006 Pino Toscano <toscano.pino@tiscali.it>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_GENERATOR_TIFF_H_
#define _OKULAR_GENERATOR_TIFF_H_

#include <core/generator.h>
#include <QHash>
#include <QLoggingCategory>

class TIFFGenerator : public Okular::Generator
{
    Q_OBJECT
    Q_INTERFACES(Okular::Generator)

public:
    TIFFGenerator(QObject* parent, const QVariantList& args);
    ~TIFFGenerator() override;

    bool loadDocument(const QString& fileName,
                      QVector<Okular::Page*>& pagesVector) override;
    bool loadDocumentFromData(const QByteArray& fileData,
                              QVector<Okular::Page*>& pagesVector) override;

    Okular::DocumentInfo generateDocumentInfo(
        const QSet<Okular::DocumentInfo::Key>& keys) const override;

    Okular::Document::PrintError print(QPrinter& printer) override;

protected:
    bool doCloseDocument() override;
    QImage image(Okular::PixmapRequest* request) override;

private:
    class Private;
    Private* const d;

    bool loadTiff(QVector<Okular::Page*>& pagesVector, const char* name);
    void loadPages(QVector<Okular::Page*>& pagesVector);
    int mapPage(int page) const;

    QHash<int, int> m_pageMapping;
};

Q_DECLARE_LOGGING_CATEGORY(OkularTiffDebug)

#endif
