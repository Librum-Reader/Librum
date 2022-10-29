/*
    SPDX-FileCopyrightText: 2008 Tobias Koenig <tokoe@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "generator_fax.h"
#include <core/document.h>
#include <core/page.h>
#include <KAboutData>
#include <KLocalizedString>
#include <QPainter>
#include <QPrinter>

OKULAR_EXPORT_PLUGIN(FaxGenerator, "libokularGenerator_fax.json")

FaxGenerator::FaxGenerator(QObject* parent, const QVariantList& args) :
    Generator(parent, args)
{
    setFeature(Threaded);
    setFeature(PrintNative);
    setFeature(PrintToFile);
}

FaxGenerator::~FaxGenerator()
{
}

bool FaxGenerator::loadDocument(const QString& fileName,
                                QVector<Okular::Page*>& pagesVector)
{
    if(fileName.endsWith(QLatin1String(".g3"), Qt::CaseInsensitive))
    {
        m_type = FaxDocument::G3;
    }
    else
    {
        m_type = FaxDocument::G4;
    }

    FaxDocument faxDocument(fileName, m_type);

    if(!faxDocument.load())
    {
        Q_EMIT error(i18n("Unable to load document"), -1);
        return false;
    }

    m_img = faxDocument.image();

    pagesVector.resize(1);

    Okular::Page* page =
        new Okular::Page(0, m_img.width(), m_img.height(), Okular::Rotation0);
    pagesVector[0] = page;

    return true;
}

bool FaxGenerator::doCloseDocument()
{
    m_img = QImage();

    return true;
}

QImage FaxGenerator::image(Okular::PixmapRequest* request)
{
    // perform a smooth scaled generation
    int width = request->width();
    int height = request->height();
    if(request->page()->rotation() % 2 == 1)
    {
        qSwap(width, height);
    }

    return m_img.scaled(width, height, Qt::IgnoreAspectRatio,
                        Qt::SmoothTransformation);
}

Okular::DocumentInfo FaxGenerator::generateDocumentInfo(
    const QSet<Okular::DocumentInfo::Key>& keys) const
{
    Okular::DocumentInfo docInfo;
    if(keys.contains(Okular::DocumentInfo::MimeType))
    {
        if(m_type == FaxDocument::G3)
        {
            docInfo.set(Okular::DocumentInfo::MimeType,
                        QStringLiteral("image/fax-g3"));
        }
        else
        {
            docInfo.set(Okular::DocumentInfo::MimeType,
                        QStringLiteral("image/fax-g4"));
        }
    }
    return docInfo;
}

Okular::Document::PrintError FaxGenerator::print(QPrinter& printer)
{
    QPainter p(&printer);

    QImage image(m_img);

    if((image.width() > printer.width()) || (image.height() > printer.height()))
    {
        image = image.scaled(printer.width(), printer.height(),
                             Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    p.drawImage(0, 0, image);

    return Okular::Document::NoPrintError;
}

#include "generator_fax.moc"
