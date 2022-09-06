/*
    Kchmviewer - a CHM and EPUB file viewer with broad language support
    SPDX-FileCopyrightText: 2004-2014 George Yunaev gyunaev@ulduzsoft.com

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include <QApplication>

#include "ebook.h"
#include "ebook_search.h"

// Helper class to simplicity state management and data keeping
class SearchDataKeeper
{
public:
    SearchDataKeeper()
    {
        m_inPhrase = false;
    }

    void beginPhrase()
    {
        phrase_terms.clear();
        m_inPhrase = true;
    }

    void endPhrase()
    {
        m_inPhrase = false;
        phrasewords += phrase_terms;
        phrases.push_back(phrase_terms.join(QStringLiteral(" ")));
    }

    bool isInPhrase() const
    {
        return m_inPhrase;
    }

    void addTerm(const QString &term)
    {
        if (!term.isEmpty()) {
            terms.push_back(term);

            if (m_inPhrase) {
                phrase_terms.push_back(term);
            }
        }
    }

    // Should contain all the search terms present in query, includind those from phrases. One element - one term .
    QStringList terms;

    // Should contain phrases present in query without quotes. One element - one phrase.
    QStringList phrases;

    // Should contain all the terms present in all the phrases (but not outside).
    QStringList phrasewords;

private:
    bool m_inPhrase;
    QStringList phrase_terms;
};

EBookSearch::EBookSearch()
{
    m_Index = nullptr;
}

EBookSearch::~EBookSearch()
{
    delete m_Index;
}

bool EBookSearch::loadIndex(QDataStream &stream)
{
    delete m_Index;

    m_Index = new QtAs::Index();
    return m_Index->readDict(stream);
}

bool EBookSearch::generateIndex(EBook *ebookFile, QDataStream &stream)
{
    QList<QUrl> documents;
    QList<QUrl> alldocuments;

    Q_EMIT progressStep(0, QStringLiteral("Generating the list of documents"));
    processEvents();

    // Enumerate the documents
    if (!ebookFile->enumerateFiles(alldocuments)) {
        return false;
    }

    if (m_Index) {
        delete m_Index;
    }

    m_Index = new QtAs::Index();
    connect(m_Index, &QtAs::Index::indexingProgress, this, &EBookSearch::updateProgress);

    // Process the list of files in CHM archive and keep only HTML document files from there
    for (const QUrl &allDocumentsI : qAsConst(alldocuments)) {
        const QString docpath = allDocumentsI.path();

        if (docpath.endsWith(QLatin1String(".html"), Qt::CaseInsensitive) || docpath.endsWith(QLatin1String(".htm"), Qt::CaseInsensitive) || docpath.endsWith(QLatin1String(".xhtml"), Qt::CaseInsensitive)) {
            documents.push_back(allDocumentsI);
        }
    }

    if (!m_Index->makeIndex(documents, ebookFile)) {
        delete m_Index;
        m_Index = nullptr;
        return false;
    }

    m_Index->writeDict(stream);
    m_keywordDocuments.clear();

    return true;
}

void EBookSearch::cancelIndexGeneration()
{
    m_Index->setLastWinClosed();
}

void EBookSearch::updateProgress(int value, const QString &stepName)
{
    Q_EMIT progressStep(value, stepName);
}

void EBookSearch::processEvents()
{
    // Do it up to ten times; some events generate other events
    for (int i = 0; i < 10; i++) {
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    }
}

bool EBookSearch::searchQuery(const QString &query, QList<QUrl> *results, EBook *ebookFile, unsigned int limit)
{
    // We should have index
    if (!m_Index) {
        return false;
    }

    // Characters which split the words. We need to make them separate tokens
    QString splitChars = m_Index->getCharsSplit();

    // Characters which are part of the word. We should keep them apart.
    QString partOfWordChars = m_Index->getCharsPartOfWord();

    // Variables to store current state
    SearchDataKeeper keeper;
    QString term;

    for (const QChar &iChar : query) {
        const QChar ch = iChar.toLower();

        // a quote either begins or ends the phrase
        if (ch == QLatin1Char('"')) {
            keeper.addTerm(term);

            if (keeper.isInPhrase()) {
                keeper.endPhrase();
            } else {
                keeper.beginPhrase();
            }

            continue;
        }

        // If new char does not stop the word, add ot and continue
        if (ch.isLetterOrNumber() || partOfWordChars.indexOf(ch) != -1) {
            term.append(ch);
            continue;
        }

        // If it is a split char, add this term and split char as separate term
        if (splitChars.indexOf(ch) != -1) {
            // Add existing term if present
            keeper.addTerm(term);

            // Change the term variable, so it will be added when we exit this block
            term = ch;
        }

        // Just add the word; it is most likely a space or terminated by tokenizer.
        keeper.addTerm(term);
        term = QString();
    }

    keeper.addTerm(term);

    if (keeper.isInPhrase()) {
        return false;
    }

    QList<QUrl> foundDocs = m_Index->query(keeper.terms, keeper.phrases, keeper.phrasewords, ebookFile);

    for (QList<QUrl>::iterator it = foundDocs.begin(); it != foundDocs.end() && limit > 0; ++it, limit--) {
        results->push_back(*it);
    }

    return true;
}

bool EBookSearch::hasIndex() const
{
    return m_Index != nullptr;
}
