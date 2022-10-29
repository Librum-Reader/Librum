/*
    SPDX-FileCopyrightText: 2017 Gilbert Assaf <gassaf@gmx.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <QMimeDatabase>
#include <QTest>
#include "core/document.h"
#include "core/page.h"
#include "core/textpage.h"
#include "settings_core.h"

class ChmGeneratorTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testDocumentStructure();
    void testDocumentContent();
    void cleanupTestCase();

private:
    Okular::Document* m_document;
};

void ChmGeneratorTest::initTestCase()
{
    Okular::SettingsCore::instance(QStringLiteral("ChmGeneratorTest"));
    m_document = new Okular::Document(nullptr);
    const QString testFile =
        QStringLiteral(KDESRCDIR "autotests/data/test.chm");
    QMimeDatabase db;
    const QMimeType mime = db.mimeTypeForFile(testFile);
    QCOMPARE(m_document->openDocument(testFile, QUrl(), mime),
             Okular::Document::OpenSuccess);
}

void ChmGeneratorTest::cleanupTestCase()
{
    m_document->closeDocument();
    delete m_document;
}

void ChmGeneratorTest::testDocumentStructure()
{
    unsigned int expectedPageNr = 6;
    QCOMPARE(m_document->pages(), expectedPageNr);
    QCOMPARE(m_document->metaData(QStringLiteral("DocumentTitle")).toString(),
             QStringLiteral("okular test chm"));

    const Okular::DocumentSynopsis* docSyn = m_document->documentSynopsis();
    QDomElement heading1 = docSyn->documentElement();
    QCOMPARE(heading1.tagName(), QStringLiteral("Heading 1"));

    QDomElement topic1 = heading1.firstChildElement();
    QCOMPARE(topic1.tagName(), QStringLiteral("Topic 1"));

    QDomElement heading1_1 = topic1.nextSiblingElement();
    QCOMPARE(heading1_1.tagName(), QStringLiteral("Heading 1.1"));

    QDomElement topic1_1 = heading1_1.firstChildElement();
    QCOMPARE(topic1_1.tagName(), QStringLiteral("Topic 1.1"));

    QDomElement heading2 = heading1.nextSiblingElement();
    QCOMPARE(heading2.tagName(), QStringLiteral("Heading 2"));
}

void ChmGeneratorTest::testDocumentContent()
{
    const Okular::Page* page0 = m_document->page(0);
    QCOMPARE(page0->number(), 0);
    m_document->requestTextPage(page0->number());
    QVERIFY(page0->hasTextPage());
    QCOMPARE(page0->text(),
             QStringLiteral("Heading 1This is an example Text."));

    const Okular::Page* page1 = m_document->page(1);
    QCOMPARE(page1->number(), 1);
    m_document->requestTextPage(page1->number());
    QVERIFY(page1->hasTextPage());
    QCOMPARE(page1->text(), QStringLiteral("Topic 1This is an example Text."));

    const Okular::Page* page2 = m_document->page(2);
    QCOMPARE(page2->number(), 2);
    m_document->requestTextPage(page2->number());
    QVERIFY(page2->hasTextPage());
    QCOMPARE(page2->text(), QStringLiteral("Heading 1.1With html title."));

    // Test page, who doesn't have an TOC entry, but one in chm index
    const Okular::Page* indexPage1 = m_document->page(5);
    QCOMPARE(indexPage1->number(), 5);
    m_document->requestTextPage(indexPage1->number());
    QVERIFY(indexPage1->hasTextPage());
    QCOMPARE(indexPage1->text(),
             QStringLiteral("Index 1This is an example Text."));
}

QTEST_MAIN(ChmGeneratorTest)
#include "chmgeneratortest.moc"

/* kate: replace-tabs on; tab-width 4; */
