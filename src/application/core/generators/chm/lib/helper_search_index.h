/*
    Kchmviewer - a CHM and EPUB file viewer with broad language support
    SPDX-FileCopyrightText: 2004-2014 George Yunaev gyunaev@ulduzsoft.com

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef EBOOK_SEARCH_INDEX_H
#define EBOOK_SEARCH_INDEX_H

#include <QDataStream>
#include <QHash>
#include <QStringList>
#include <QUrl>
#include <QVector>
#include "helper_entitydecoder.h"

class EBook;

// This code is based on some pretty old version of Qt Assistant
namespace QtAs
{
struct Document
{
    Document(int d, int f) :
        docNumber(d),
        frequency(f)
    {
    }

    Document() :
        docNumber(-1),
        frequency(0)
    {
    }

    bool operator==(const Document doc) const
    {
        return docNumber == doc.docNumber;
    }

    bool operator<(const Document doc) const
    {
        return frequency > doc.frequency;
    }

    bool operator<=(const Document doc) const
    {
        return frequency >= doc.frequency;
    }

    bool operator>(const Document doc) const
    {
        return frequency < doc.frequency;
    }

    qint16 docNumber;
    qint16 frequency;
};

QDataStream& operator>>(QDataStream& s, Document& l);
QDataStream& operator<<(QDataStream& s, const Document l);

class Index : public QObject
{
    Q_OBJECT

public:
    Index();

    void writeDict(QDataStream& stream);
    bool readDict(QDataStream& stream);
    bool makeIndex(const QList<QUrl>& docs, EBook* chmFile);
    QList<QUrl> query(const QStringList&, const QStringList&,
                      const QStringList&, EBook* chmFile);

    QString getCharsSplit() const
    {
        return m_charssplit;
    }

    QString getCharsPartOfWord() const
    {
        return m_charsword;
    }

Q_SIGNALS:
    void indexingProgress(int, const QString&);

public Q_SLOTS:
    void setLastWinClosed();

private:
    struct Entry
    {
        explicit Entry(int d)
        {
            documents.append(Document(d, 1));
        }

        explicit Entry(const QVector<Document>& l) :
            documents(l)
        {
        }

        QVector<Document> documents;
    };

    struct PosEntry
    {
        explicit PosEntry(int p)
        {
            positions.append(p);
        }

        QList<uint> positions;
    };

    bool parseDocumentToStringlist(EBook* chmFile, const QUrl& filename,
                                   QStringList& tokenlist);
    void insertInDict(const QString&, int);

    QStringList getWildcardTerms(const QString&);
    QStringList split(const QString&);
    QList<Document> setupDummyTerm(const QStringList&);
    bool searchForPhrases(const QStringList& phrases, const QStringList& words,
                          const QUrl& filename, EBook* chmFile);

    QList<QUrl> docList;
    QHash<QString, Entry*> dict;
    QHash<QString, PosEntry*> miniDict;
    bool lastWindowClosed;
    HelperEntityDecoder entityDecoder;

    // Those characters are splitters (i.e. split the word), but added
    // themselves into dictionary too. This makes the dictionary MUCH larger,
    // but ensure that for the piece of "window->print" both search for "print"
    // and "->print" will find it.
    QString m_charssplit;

    // Those characters are parts of word - for example, '_' is here, and search
    // for _debug will find only _debug.
    QString m_charsword;
};

};  // namespace QtAs

Q_DECLARE_TYPEINFO(QtAs::Document, Q_MOVABLE_TYPE);

#endif  // EBOOK_SEARCH_INDEX_H
