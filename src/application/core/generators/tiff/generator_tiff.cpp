/*
    SPDX-FileCopyrightText: 2006 Pino Toscano <toscano.pino@tiscali.it>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "generator_tiff.h"

#include <QBuffer>
#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QList>
#include <QPainter>
#include <QPrinter>

#include <KAboutData>
#include <KLocalizedString>
#include <QDebug>

#include <core/document.h>
#include <core/fileprinter.h>
#include <core/page.h>
#include <core/utils.h>

#include <tiff.h>
#include <tiffio.h>

#define TiffDebug 4714

tsize_t okular_tiffReadProc(thandle_t handle, tdata_t buf, tsize_t size)
{
    QIODevice *device = static_cast<QIODevice *>(handle);
    return device->isReadable() ? device->read(static_cast<char *>(buf), size) : -1;
}

tsize_t okular_tiffWriteProc(thandle_t handle, tdata_t buf, tsize_t size)
{
    QIODevice *device = static_cast<QIODevice *>(handle);
    return device->write(static_cast<char *>(buf), size);
}

toff_t okular_tiffSeekProc(thandle_t handle, toff_t offset, int whence)
{
    QIODevice *device = static_cast<QIODevice *>(handle);
    switch (whence) {
    case SEEK_SET:
        device->seek(offset);
        break;
    case SEEK_CUR:
        device->seek(device->pos() + offset);
        break;
    case SEEK_END:
        device->seek(device->size() + offset);
        break;
    }

    return device->pos();
}

int okular_tiffCloseProc(thandle_t handle)
{
    Q_UNUSED(handle)
    return 0;
}

toff_t okular_tiffSizeProc(thandle_t handle)
{
    QIODevice *device = static_cast<QIODevice *>(handle);
    return device->size();
}

int okular_tiffMapProc(thandle_t, tdata_t *, toff_t *)
{
    return 0;
}

void okular_tiffUnmapProc(thandle_t, tdata_t, toff_t)
{
}

class TIFFGenerator::Private
{
public:
    Private()
        : tiff(nullptr)
        , dev(nullptr)
    {
    }

    TIFF *tiff;
    QByteArray data;
    QIODevice *dev;
};

static QDateTime convertTIFFDateTime(const char *tiffdate)
{
    if (!tiffdate) {
        return QDateTime();
    }

    return QDateTime::fromString(QString::fromLatin1(tiffdate), QStringLiteral("yyyy:MM:dd HH:mm:ss"));
}

static void adaptSizeToResolution(TIFF *tiff, ttag_t whichres, double dpi, uint32_t *size)
{
    float resvalue = 1.0;
    uint16_t resunit = 0;
    if (!TIFFGetField(tiff, whichres, &resvalue) || !TIFFGetFieldDefaulted(tiff, TIFFTAG_RESOLUTIONUNIT, &resunit)) {
        return;
    }

    float newsize = *size / resvalue;
    switch (resunit) {
    case RESUNIT_INCH:
        *size = (uint32_t)(newsize * dpi);
        break;
    case RESUNIT_CENTIMETER:
        *size = (uint32_t)(newsize * 10.0 / 25.4 * dpi);
        break;
    case RESUNIT_NONE:
        break;
    }
}

static Okular::Rotation readTiffRotation(TIFF *tiff)
{
    uint32_t tiffOrientation = 0;

    if (!TIFFGetField(tiff, TIFFTAG_ORIENTATION, &tiffOrientation)) {
        return Okular::Rotation0;
    }

    Okular::Rotation ret = Okular::Rotation0;
    switch (tiffOrientation) {
    case ORIENTATION_TOPLEFT:
    case ORIENTATION_TOPRIGHT:
        ret = Okular::Rotation0;
        break;
    case ORIENTATION_BOTRIGHT:
    case ORIENTATION_BOTLEFT:
        ret = Okular::Rotation180;
        break;
    case ORIENTATION_LEFTTOP:
    case ORIENTATION_LEFTBOT:
        ret = Okular::Rotation270;
        break;
    case ORIENTATION_RIGHTTOP:
    case ORIENTATION_RIGHTBOT:
        ret = Okular::Rotation90;
        break;
    }

    return ret;
}

OKULAR_EXPORT_PLUGIN(TIFFGenerator, "libokularGenerator_tiff.json")

TIFFGenerator::TIFFGenerator(QObject *parent, const QVariantList &args)
    : Okular::Generator(parent, args)
    , d(new Private)
{
    setFeature(Threaded);
    setFeature(PrintNative);
    setFeature(PrintToFile);
    setFeature(ReadRawData);
}

TIFFGenerator::~TIFFGenerator()
{
    if (d->tiff) {
        TIFFClose(d->tiff);
        d->tiff = nullptr;
    }

    delete d;
}

bool TIFFGenerator::loadDocument(const QString &fileName, QVector<Okular::Page *> &pagesVector)
{
    QFile *qfile = new QFile(fileName);
    qfile->open(QIODevice::ReadOnly);
    d->dev = qfile;
    d->data = QFile::encodeName(QFileInfo(*qfile).fileName());
    return loadTiff(pagesVector, d->data.constData());
}

bool TIFFGenerator::loadDocumentFromData(const QByteArray &fileData, QVector<Okular::Page *> &pagesVector)
{
    d->data = fileData;
    QBuffer *qbuffer = new QBuffer(&d->data);
    qbuffer->open(QIODevice::ReadOnly);
    d->dev = qbuffer;
    return loadTiff(pagesVector, "<stdin>");
}

bool TIFFGenerator::loadTiff(QVector<Okular::Page *> &pagesVector, const char *name)
{
    d->tiff = TIFFClientOpen(name, "r", d->dev, okular_tiffReadProc, okular_tiffWriteProc, okular_tiffSeekProc, okular_tiffCloseProc, okular_tiffSizeProc, okular_tiffMapProc, okular_tiffUnmapProc);
    if (!d->tiff) {
        delete d->dev;
        d->dev = nullptr;
        d->data.clear();
        return false;
    }

    loadPages(pagesVector);

    return true;
}

bool TIFFGenerator::doCloseDocument()
{
    // closing the old document
    if (d->tiff) {
        TIFFClose(d->tiff);
        d->tiff = nullptr;
        delete d->dev;
        d->dev = nullptr;
        d->data.clear();
        m_pageMapping.clear();
    }

    return true;
}

QImage TIFFGenerator::image(Okular::PixmapRequest *request)
{
    bool generated = false;
    QImage img;

    if (TIFFSetDirectory(d->tiff, mapPage(request->page()->number()))) {
        int rotation = request->page()->rotation();
        uint32_t width = 1;
        uint32_t height = 1;
        uint32_t orientation = 0;
        TIFFGetField(d->tiff, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(d->tiff, TIFFTAG_IMAGELENGTH, &height);

        if (!TIFFGetField(d->tiff, TIFFTAG_ORIENTATION, &orientation)) {
            orientation = ORIENTATION_TOPLEFT;
        }

        QImage image(width, height, QImage::Format_RGB32);
        uint32_t *data = reinterpret_cast<uint32_t *>(image.bits());

        // read data
        if (TIFFReadRGBAImageOriented(d->tiff, width, height, data, orientation) != 0) {
            // an image read by ReadRGBAImage is ABGR, we need ARGB, so swap red and blue
            uint32_t size = width * height;
            for (uint32_t i = 0; i < size; ++i) {
                uint32_t red = (data[i] & 0x00FF0000) >> 16;
                uint32_t blue = (data[i] & 0x000000FF) << 16;
                data[i] = (data[i] & 0xFF00FF00) + red + blue;
            }

            int reqwidth = request->width();
            int reqheight = request->height();
            if (rotation % 2 == 1) {
                qSwap(reqwidth, reqheight);
            }
            img = image.scaled(reqwidth, reqheight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

            generated = true;
        }
    }

    if (!generated) {
        img = QImage(request->width(), request->height(), QImage::Format_RGB32);
        img.fill(qRgb(255, 255, 255));
    }

    return img;
}

Okular::DocumentInfo TIFFGenerator::generateDocumentInfo(const QSet<Okular::DocumentInfo::Key> &keys) const
{
    Okular::DocumentInfo docInfo;
    if (d->tiff) {
        if (keys.contains(Okular::DocumentInfo::MimeType)) {
            docInfo.set(Okular::DocumentInfo::MimeType, QStringLiteral("image/tiff"));
        }

        if (keys.contains(Okular::DocumentInfo::Description)) {
            char *buffer = nullptr;
            TIFFGetField(d->tiff, TIFFTAG_IMAGEDESCRIPTION, &buffer);
            docInfo.set(Okular::DocumentInfo::Description, buffer ? QString::fromLatin1(buffer) : QString());
        }

        if (keys.contains(Okular::DocumentInfo::Producer)) {
            char *buffer = nullptr;
            TIFFGetField(d->tiff, TIFFTAG_SOFTWARE, &buffer);
            docInfo.set(Okular::DocumentInfo::Producer, buffer ? QString::fromLatin1(buffer) : QString());
        }

        if (keys.contains(Okular::DocumentInfo::Copyright)) {
            char *buffer = nullptr;
            TIFFGetField(d->tiff, TIFFTAG_COPYRIGHT, &buffer);
            docInfo.set(Okular::DocumentInfo::Copyright, buffer ? QString::fromLatin1(buffer) : QString());
        }

        if (keys.contains(Okular::DocumentInfo::Author)) {
            char *buffer = nullptr;
            TIFFGetField(d->tiff, TIFFTAG_ARTIST, &buffer);
            docInfo.set(Okular::DocumentInfo::Author, buffer ? QString::fromLatin1(buffer) : QString());
        }

        if (keys.contains(Okular::DocumentInfo::CreationDate)) {
            char *buffer = nullptr;
            TIFFGetField(d->tiff, TIFFTAG_DATETIME, &buffer);
            QDateTime date = convertTIFFDateTime(buffer);
            docInfo.set(Okular::DocumentInfo::CreationDate, date.isValid() ? QLocale().toString(date, QLocale::LongFormat) : QString());
        }
    }

    return docInfo;
}

void TIFFGenerator::loadPages(QVector<Okular::Page *> &pagesVector)
{
    if (!d->tiff) {
        return;
    }

    tdir_t dirs = TIFFNumberOfDirectories(d->tiff);
    pagesVector.resize(dirs);
    tdir_t realdirs = 0;

    uint32_t width = 0;
    uint32_t height = 0;

    const QSizeF dpi = Okular::Utils::realDpi(nullptr);
    for (tdir_t i = 0; i < dirs; ++i) {
        if (!TIFFSetDirectory(d->tiff, i)) {
            continue;
        }

        if (TIFFGetField(d->tiff, TIFFTAG_IMAGEWIDTH, &width) != 1 || TIFFGetField(d->tiff, TIFFTAG_IMAGELENGTH, &height) != 1) {
            continue;
        }

        adaptSizeToResolution(d->tiff, TIFFTAG_XRESOLUTION, dpi.width(), &width);
        adaptSizeToResolution(d->tiff, TIFFTAG_YRESOLUTION, dpi.height(), &height);

        Okular::Page *page = new Okular::Page(realdirs, width, height, readTiffRotation(d->tiff));
        pagesVector[realdirs] = page;

        m_pageMapping[realdirs] = i;

        ++realdirs;
    }

    pagesVector.resize(realdirs);
}

Okular::Document::PrintError TIFFGenerator::print(QPrinter &printer)
{
    uint32_t width = 0;
    uint32_t height = 0;

    QPainter p(&printer);

    QList<int> pageList = Okular::FilePrinter::pageList(printer, document()->pages(), document()->currentPage() + 1, document()->bookmarkedPageList());

    for (int i = 0; i < pageList.count(); ++i) {
        if (!TIFFSetDirectory(d->tiff, mapPage(pageList[i] - 1))) {
            continue;
        }

        if (TIFFGetField(d->tiff, TIFFTAG_IMAGEWIDTH, &width) != 1 || TIFFGetField(d->tiff, TIFFTAG_IMAGELENGTH, &height) != 1) {
            continue;
        }

        QImage image(width, height, QImage::Format_RGB32);
        uint32_t *data = reinterpret_cast<uint32_t *>(image.bits());

        // read data
        if (TIFFReadRGBAImageOriented(d->tiff, width, height, data, ORIENTATION_TOPLEFT) != 0) {
            // an image read by ReadRGBAImage is ABGR, we need ARGB, so swap red and blue
            uint32_t size = width * height;
            for (uint32_t j = 0; j < size; ++j) {
                uint32_t red = (data[j] & 0x00FF0000) >> 16;
                uint32_t blue = (data[j] & 0x000000FF) << 16;
                data[j] = (data[j] & 0xFF00FF00) + red + blue;
            }
        }

        if (i != 0) {
            printer.newPage();
        }

        QSize targetSize = printer.pageRect().size();

        if ((image.width() < targetSize.width()) && (image.height() < targetSize.height())) {
            // draw small images at 100% (don't scale up)
            p.drawImage(0, 0, image);
        } else {
            // fit to page
            p.drawImage(0, 0, image.scaled(targetSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
    }

    return Okular::Document::NoPrintError;
}

int TIFFGenerator::mapPage(int page) const
{
    QHash<int, int>::const_iterator it = m_pageMapping.find(page);
    if (it == m_pageMapping.end()) {
        qCWarning(OkularTiffDebug) << "Requesting unmapped page" << page << ":" << m_pageMapping;
        return -1;
    }
    return it.value();
}

Q_LOGGING_CATEGORY(OkularTiffDebug, "org.kde.okular.generators.tiff", QtWarningMsg)

#include "generator_tiff.moc"
