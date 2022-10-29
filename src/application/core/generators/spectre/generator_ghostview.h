/*
    SPDX-FileCopyrightText: 2007 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_GENERATOR_GHOSTVIEW_H_
#define _OKULAR_GENERATOR_GHOSTVIEW_H_

#include <core/generator.h>
#include <interfaces/configinterface.h>
#include <libspectre/spectre.h>

class GSGenerator : public Okular::Generator,
                    public Okular::ConfigInterface
{
    Q_OBJECT
    Q_INTERFACES(Okular::Generator)
    Q_INTERFACES(Okular::ConfigInterface)

public:
    /** virtual methods to reimplement **/
    // load a document and fill up the pagesVector
    bool loadDocument(const QString& fileName,
                      QVector<Okular::Page*>& pagesVector) override;

    // Document description and Table of contents
    Okular::DocumentInfo generateDocumentInfo(
        const QSet<Okular::DocumentInfo::Key>& keys) const override;

    const Okular::DocumentSynopsis* generateDocumentSynopsis() override
    {
        return nullptr;
    }

    const Okular::DocumentFonts* generateDocumentFonts()
    {
        return nullptr;
    }

    // page contents generation
    bool canGeneratePixmap() const override;
    void generatePixmap(Okular::PixmapRequest* request) override;

    QVariant metaData(const QString& key,
                      const QVariant& option) const override;

    // print document using already configured kprinter
    Okular::Document::PrintError print(QPrinter& /*printer*/) override;
    QString fileName() const;

    bool reparseConfig() override;
    void addPages(KConfigDialog* dlg) override;

    /** constructor **/
    GSGenerator(QObject* parent, const QVariantList& args);
    ~GSGenerator() override;

public Q_SLOTS:
    void slotImageGenerated(QImage* img, Okular::PixmapRequest* request);

protected:
    bool doCloseDocument() override;

private:
    bool loadPages(QVector<Okular::Page*>& pagesVector);
    Okular::Rotation orientation(SpectreOrientation orientation) const;

    // backendish stuff
    SpectreDocument* m_internalDocument;

    Okular::PixmapRequest* m_request;

    bool cache_AAtext;
    bool cache_AAgfx;
};

#endif
