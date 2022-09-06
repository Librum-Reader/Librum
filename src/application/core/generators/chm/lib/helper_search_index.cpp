/*
    Kchmviewer - a CHM and EPUB file viewer with broad language support
    SPDX-FileCopyrightText: 2004-2014 George Yunaev gyunaev@ulduzsoft.com

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include <QApplication>
#include <QTextCodec>

#include "ebook.h"
#include "ebook_search.h"
#include "helper_search_index.h"

static const int DICT_VERSION = 4;

namespace QtAs
{
// Those characters are splitters (i.e. split the word), but added themselves into dictionary too.
// This makes the dictionary MUCH larger, but ensure that for the piece of "window->print" both
// search for "print" and "->print" will find it.
#define SPLIT_CHARACTERS QStringLiteral("!()*&^%#@[]{}':;,.?/|/?<>\\-+=~`")

// Those characters are parts of word - for example, '_' is here, and search for _debug will find only _debug.
#define WORD_CHARACTERS QStringLiteral("$_")

struct Term {
    Term()
        : frequency(-1)
    {
    }
    Term(const QString &t, int f, const QVector<Document> &l)
        : term(t)
        , frequency(f)
        , documents(l)
    {
    }
    QString term;
    int frequency;
    QVector<Document> documents;
    bool operator<(const Term &i2) const
    {
        return frequency < i2.frequency;
    }
};

QDataStream &operator>>(QDataStream &s, Document &l)
{
    s >> l.docNumber;
    s >> l.frequency;
    return s;
}

QDataStream &operator<<(QDataStream &s, const Document l)
{
    s << (short)l.docNumber;
    s << (short)l.frequency;
    return s;
}

Index::Index()
    : QObject(nullptr)
{
    lastWindowClosed = false;
    connect(qApp, &QGuiApplication::lastWindowClosed, this, &Index::setLastWinClosed);
}

void Index::setLastWinClosed()
{
    lastWindowClosed = true;
}

bool Index::makeIndex(const QList<QUrl> &docs, EBook *chmFile)
{
    if (docs.isEmpty()) {
        return false;
    }

    docList = docs;

    if (chmFile->hasFeature(EBook::FEATURE_ENCODING)) {
        entityDecoder.changeEncoding(QTextCodec::codecForName(chmFile->currentEncoding().toUtf8()));
    }

    QList<QUrl>::ConstIterator it = docList.constBegin();
    int steps = docList.count() / 100;

    if (!steps) {
        steps++;
    }

    int prog = 0;

    for (int i = 0; it != docList.constEnd(); ++it, ++i) {
        if (lastWindowClosed) {
            return false;
        }

        const QUrl &filename = *it;
        QStringList terms;

        if (parseDocumentToStringlist(chmFile, filename, terms)) {
            for (QStringList::ConstIterator tit = terms.constBegin(); tit != terms.constEnd(); ++tit) {
                insertInDict(*tit, i);
            }
        }

        if (i % steps == 0) {
            prog++;
            prog = qMin(prog, 99);
            Q_EMIT indexingProgress(prog, tr("Processing document %1").arg((*it).path()));
        }
    }

    Q_EMIT indexingProgress(100, tr("Processing completed"));
    return true;
}

void Index::insertInDict(const QString &str, int docNum)
{
    Entry *e = nullptr;
    if (!dict.isEmpty()) {
        e = dict[str];
    }

    if (e) {
        if (e->documents.last().docNumber != docNum) {
            e->documents.append(Document(docNum, 1));
        } else {
            e->documents.last().frequency++;
        }
    } else {
        dict.insert(str, new Entry(docNum));
    }
}

bool Index::parseDocumentToStringlist(EBook *chmFile, const QUrl &filename, QStringList &tokenlist)
{
    QString parsedbuf, parseentity, text;

    if (!chmFile->getFileContentAsString(text, filename) || text.isEmpty()) {
        qWarning("Search index generator: could not retrieve the document content for %s", qPrintable(filename.toString()));
        return false;
    }

    m_charssplit = SPLIT_CHARACTERS;
    m_charsword = WORD_CHARACTERS;

    tokenlist.clear();

    // State machine states
    enum state_t {
        STATE_OUTSIDE_TAGS,  // outside HTML tags; parse text
        STATE_IN_HTML_TAG,   // inside HTML tags; wait for end tag
        STATE_IN_QUOTES,     // inside HTML tags and inside quotes; wait for end quote (in var QuoteChar)
        STATE_IN_HTML_ENTITY // inside HTML entity; parse the entity
    };

    state_t state = STATE_OUTSIDE_TAGS;
    QChar QuoteChar; // used in STATE_IN_QUOTES

    for (int j = 0; j < text.length(); j++) {
        QChar ch = text[j];

        if ((j % 20000) == 0) {
            qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
        }

        if (state == STATE_IN_HTML_TAG) {
            // We are inside HTML tag.
            // Ignore everything until we see '>' (end of HTML tag) or quote char (quote start)
            if (ch == QLatin1Char('"') || ch == QLatin1Char('\'')) {
                state = STATE_IN_QUOTES;
                QuoteChar = ch;
            } else if (ch == QLatin1Char('>')) {
                state = STATE_OUTSIDE_TAGS;
            }

            continue;
        } else if (state == STATE_IN_QUOTES) {
            // We are inside quoted text inside HTML tag.
            // Ignore everything until we see the quote character again
            if (ch == QuoteChar) {
                state = STATE_IN_HTML_TAG;
            }

            continue;
        } else if (state == STATE_IN_HTML_ENTITY) {
            // We are inside encoded HTML entity (like &nbsp;).
            // Collect to parsedbuf everything until we see ;
            if (ch.isLetterOrNumber()) {
                // get next character of this entity
                parseentity.append(ch);
                continue;
            }

            // The entity ended
            state = STATE_OUTSIDE_TAGS;

            // Some shitty HTML does not terminate entities correctly. Screw it.
            if (ch != QLatin1Char(';') && ch != QLatin1Char('<')) {
                if (parseentity.isEmpty()) {
                    // straight '&' symbol. Add and continue.
                    parsedbuf += QLatin1String("&");
                } else {
                    qWarning("Index::parseDocument: incorrectly terminated HTML entity '&%s%c', ignoring", qPrintable(parseentity), ch.toLatin1());
                }

                j--; // parse this character again, but in different state
                continue;
            }

            // Don't we have a space?
            if (parseentity.toLower() != QLatin1String("nbsp")) {
                QString entity = entityDecoder.decode(parseentity);

                if (entity.isNull()) {
                    // decodeEntity() already printed error message
                    // qWarning( "Index::parseDocument: failed to decode entity &%s;", parsedbuf.ascii() );
                    continue;
                }

                parsedbuf += entity;
                continue;
            } else {
                ch = QLatin1Char(' '); // We got a space, so treat it like it, and not add it to parsebuf
            }
        }

        //
        // Now process STATE_OUTSIDE_TAGS
        //

        // Check for start of HTML tag, and switch to STATE_IN_HTML_TAG if it is
        if (ch == QLatin1Char('<')) {
            state = STATE_IN_HTML_TAG;
            goto tokenize_buf;
        }

        // Check for start of HTML entity
        if (ch == QLatin1Char('&')) {
            state = STATE_IN_HTML_ENTITY;
            parseentity = QString();
            continue;
        }

        // Replace quote by ' - quotes are used in search window to set the phrase
        if (ch == QLatin1Char('"')) {
            ch = QLatin1Char('\'');
        }

        // Ok, we have a valid character outside HTML tags, and probably some in buffer already.
        // If it is char or letter, add it and continue
        if (ch.isLetterOrNumber() || m_charsword.indexOf(ch) != -1) {
            parsedbuf.append(ch);
            continue;
        }

        // If it is a split char, add the word to the dictionary, and then add the char itself.
        if (m_charssplit.indexOf(ch) != -1) {
            if (!parsedbuf.isEmpty()) {
                tokenlist.push_back(parsedbuf.toLower());
            }

            tokenlist.push_back(ch.toLower());
            parsedbuf = QString();
            continue;
        }

    tokenize_buf:
        // Just add the word; it is most likely a space or terminated by tokenizer.
        if (!parsedbuf.isEmpty()) {
            tokenlist.push_back(parsedbuf.toLower());
            parsedbuf = QString();
        }
    }

    // Add the last word if still here - for broken htmls.
    if (!parsedbuf.isEmpty()) {
        tokenlist.push_back(parsedbuf.toLower());
    }

    return true;
}

void Index::writeDict(QDataStream &stream)
{
    stream << DICT_VERSION;
    stream << m_charssplit;
    stream << m_charsword;

    // Document list
    stream << docList;

    // Dictionary
    for (QHash<QString, Entry *>::ConstIterator it = dict.constBegin(); it != dict.constEnd(); ++it) {
        stream << it.key();
        stream << (int)it.value()->documents.count();
        stream << it.value()->documents;
    }
}

bool Index::readDict(QDataStream &stream)
{
    dict.clear();
    docList.clear();

    QString key;
    int version, numOfDocs;

    stream >> version;

    if (version < 2) {
        return false;
    }

    stream >> m_charssplit;
    stream >> m_charsword;

    // Read the document list
    stream >> docList;

    while (!stream.atEnd()) {
        stream >> key;
        stream >> numOfDocs;

        QVector<Document> docs(numOfDocs);

        stream >> docs;
        dict.insert(key, new Entry(docs));
    }

    return dict.size() > 0;
}

QList<QUrl> Index::query(const QStringList &terms, const QStringList &termSeq, const QStringList &seqWords, EBook *chmFile)
{
    QList<Term> termList;

    QStringList::ConstIterator it = terms.begin();
    for (it = terms.begin(); it != terms.end(); ++it) {
        Entry *e = nullptr;

        if (dict[*it]) {
            e = dict[*it];
            termList.append(Term(*it, e->documents.count(), e->documents));
        } else {
            return QList<QUrl>();
        }
    }

    if (termList.isEmpty()) {
        return QList<QUrl>();
    }

    std::sort(termList.begin(), termList.end());

    QVector<Document> minDocs = termList.takeFirst().documents;
    for (const Term &t : qAsConst(termList)) {
        const QVector<Document> docs = t.documents;
        for (QVector<Document>::Iterator minDoc_it = minDocs.begin(); minDoc_it != minDocs.end();) {
            bool found = false;
            for (QVector<Document>::ConstIterator doc_it = docs.constBegin(); doc_it != docs.constEnd(); ++doc_it) {
                if ((*minDoc_it).docNumber == (*doc_it).docNumber) {
                    (*minDoc_it).frequency += (*doc_it).frequency;
                    found = true;
                    break;
                }
            }
            if (!found) {
                minDoc_it = minDocs.erase(minDoc_it);
            } else {
                ++minDoc_it;
            }
        }
    }

    QList<QUrl> results;
    std::sort(minDocs.begin(), minDocs.end());
    if (termSeq.isEmpty()) {
        for (const Document &doc : qAsConst(minDocs)) {
            results << docList.at((int)doc.docNumber);
        }
        return results;
    }

    QUrl fileName;
    for (const Document &doc : qAsConst(minDocs)) {
        fileName = docList[(int)doc.docNumber];
        if (searchForPhrases(termSeq, seqWords, fileName, chmFile)) {
            results << fileName;
        }
    }

    return results;
}

bool Index::searchForPhrases(const QStringList &phrases, const QStringList &words, const QUrl &filename, EBook *chmFile)
{
    QStringList parsed_document;

    if (!parseDocumentToStringlist(chmFile, filename, parsed_document)) {
        return false;
    }

    miniDict.clear();

    // Initialize the dictionary with the words in phrase(s)
    for (const QString &word : words) {
        miniDict.insert(word, new PosEntry(0));
    }

    // Fill the dictionary with the words from the document
    unsigned int word_offset = 3;
    for (QStringList::ConstIterator it = parsed_document.constBegin(); it != parsed_document.constEnd(); it++, word_offset++) {
        PosEntry *entry = miniDict[*it];

        if (entry) {
            entry->positions.append(word_offset);
        }
    }

    // Dump it
    /*
        QDictIterator<PosEntry> it( miniDict );
        for( ; it.current(); ++it )
        {
            QString text( it.currentKey() );
            QValueList<uint> pos = miniDict[text]->positions;
            for ( unsigned int i = 1; i < pos.size(); i++ )
                text += " " + QString::number( pos[i] );

            qDebug( "%s", text.ascii());
        }
    */

    QList<uint> first_word_positions;

    for (QStringList::ConstIterator phrase_it = phrases.constBegin(); phrase_it != phrases.constEnd(); phrase_it++) {
        QStringList phrasewords = phrase_it->split(QLatin1Char(' '));
        first_word_positions = miniDict[phrasewords[0]]->positions;

        for (int j = 1; j < phrasewords.count(); ++j) {
            QList<uint> next_word_it = miniDict[phrasewords[j]]->positions;
            QList<uint>::iterator dict_it = first_word_positions.begin();

            while (dict_it != first_word_positions.end()) {
                if (next_word_it.indexOf(*dict_it + 1) != -1) {
                    (*dict_it)++;
                    ++dict_it;
                } else {
                    dict_it = first_word_positions.erase(dict_it);
                }
            }
        }
    }

    return !first_word_positions.isEmpty();
}

};
