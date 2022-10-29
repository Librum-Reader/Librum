/*
    SPDX-FileCopyrightText: 2013 Fabio D 'Urso <fabiodurso@hotmail.it>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <core/observer.h>
#include <core/page.h>
#include <gui/pagepainter.h>
#include <KPluginLoader>
#include <QImage>
#include <QImageReader>
#include <QMimeDatabase>
#include <QPainter>
#include <QTemporaryFile>
#include <QTest>
#include "../../settings_core.h"
#include "../generator_kimgio.h"

class KIMGIOTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testExifOrientation_data();
    void testExifOrientation();
};

void KIMGIOTest::initTestCase()
{
    // Make sure we find the okularGenerator_kimgio that we build just now and
    // not the system one
    QFileInfo lib(QStringLiteral(GENERATOR_PATH));
    QVERIFY2(lib.exists(), GENERATOR_PATH);
    QStringList libPaths = QCoreApplication::libraryPaths();
    libPaths.prepend(lib.absolutePath());
    QCoreApplication::setLibraryPaths(libPaths);
    QVERIFY(!KPluginLoader::findPlugin(QStringLiteral("okularGenerator_kimgio"))
                 .isEmpty());
    // make sure we didn't break the search path for image formats:
    auto availableFormats = QImageReader::supportedImageFormats();
    QVERIFY2(availableFormats.contains("jpeg"),
             availableFormats.join(", ").constData());
}

// The following images have different Exif orientation tags, but they all
// are a 3x2 rectangle whose top-left pixel is black, and whose other pixels are
// white. Note that, due to JPEG lossy compression, some pixels are not pure
// white. In testExifOrientation, we only check the top-left and bottom-right
// corners.
void KIMGIOTest::testExifOrientation_data()
{
    QTest::addColumn<QString>("imgPath");

    // No Exif metadata at all
    QTest::newRow("No Exif metadata")
        << KDESRCDIR "tests/data/testExifOrientation-noexif.jpg";

    // No Exif orientation information
    QTest::newRow("Unspecified")
        << KDESRCDIR "tests/data/testExifOrientation-unspecified.jpg";

    // Valid Orientation values
    QTest::newRow("Horizontal (normal)")
        << KDESRCDIR "tests/data/testExifOrientation-0.jpg";
    QTest::newRow("Mirror horizontal")
        << KDESRCDIR "tests/data/testExifOrientation-0mirror.jpg";
    QTest::newRow("Rotate 90 CW")
        << KDESRCDIR "tests/data/testExifOrientation-90.jpg";
    QTest::newRow("Mirror horizontal and rotate 90 CW")
        << KDESRCDIR "tests/data/testExifOrientation-90mirror.jpg";
    QTest::newRow("Rotate 180")
        << KDESRCDIR "tests/data/testExifOrientation-180.jpg";
    QTest::newRow("Mirror vertical")
        << KDESRCDIR "tests/data/testExifOrientation-180mirror.jpg";
    QTest::newRow("Rotate 270 CW")
        << KDESRCDIR "tests/data/testExifOrientation-270.jpg";
    QTest::newRow("Mirror horizontal and rotate 270 CW")
        << KDESRCDIR "tests/data/testExifOrientation-270mirror.jpg";
}

void KIMGIOTest::testExifOrientation()
{
    QFETCH(QString, imgPath);
    QMimeDatabase db;

    Okular::SettingsCore::instance(QStringLiteral("kimgiotest"));
    Okular::Document* m_document = new Okular::Document(nullptr);
    const QMimeType mime = db.mimeTypeForFile(imgPath);

    Okular::DocumentObserver* dummyDocumentObserver =
        new Okular::DocumentObserver();
    m_document->addObserver(dummyDocumentObserver);

    // Load image
    QCOMPARE((int)m_document->openDocument(imgPath, QUrl(), mime),
             (int)Okular::Document::OpenSuccess);
    m_document->setRotation(0);  // Test the default rotation
    QCOMPARE(m_document->pages(), 1u);

    // Check size
    QCOMPARE(m_document->page(0)->width(), double(3));
    QCOMPARE(m_document->page(0)->height(), double(2));

    // Generate pixmap
    Okular::PixmapRequest* req = new Okular::PixmapRequest(
        dummyDocumentObserver, 0, 3, 2, qApp->devicePixelRatio(), 1,
        Okular::PixmapRequest::NoFeature);
    m_document->requestPixmaps({ req });
    QVERIFY(m_document->page(0)->hasPixmap(dummyDocumentObserver, 3, 2));

    // Obtain image
    QImage img(3, 2, QImage::Format_ARGB32_Premultiplied);
    QPainter p(&img);
    PagePainter::paintPageOnPainter(&p, m_document->page(0),
                                    dummyDocumentObserver, 0, 3, 2,
                                    QRect(0, 0, 3, 2));

    // Verify pixel data
    QCOMPARE(img.pixel(0, 0), qRgb(0, 0, 0));
    QCOMPARE(img.pixel(2, 1), qRgb(255, 255, 255));

    m_document->removeObserver(dummyDocumentObserver);
    delete dummyDocumentObserver;
    delete m_document;
}

QTEST_MAIN(KIMGIOTest)
#include "kimgiotest.moc"
