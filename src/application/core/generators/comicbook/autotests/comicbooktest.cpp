/*
    SPDX-FileCopyrightText: 2020 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <QTest>
#include "../document.h"
#include "core/document.h"
#include "core/generator.h"
#include "core/observer.h"
#include "core/page.h"
#include "settings_core.h"

class ComicBookGeneratorTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testRotatedImage();
    void cleanupTestCase();
};

void ComicBookGeneratorTest::initTestCase()
{
    Okular::SettingsCore::instance(QStringLiteral("ComicBookGeneratorTest"));
}

void ComicBookGeneratorTest::cleanupTestCase()
{
}

void ComicBookGeneratorTest::testRotatedImage()
{
    ComicBook::Document document;
    const QString testFile =
        QStringLiteral(KDESRCDIR "autotests/data/rotated_cb.cbz");
    QVERIFY(document.open(testFile));

    QVector<Okular::Page*> pagesVector;
    document.pages(&pagesVector);

    const Okular::Page* p = pagesVector[0];
    QVERIFY(p->height() > p->width());

    const QImage image = document.pageImage(0);
    QVERIFY(image.height() > image.width());
}

QTEST_MAIN(ComicBookGeneratorTest)
#include "comicbooktest.moc"

/* kate: replace-tabs on; tab-width 4; */
