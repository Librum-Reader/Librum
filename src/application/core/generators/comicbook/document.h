/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef COMICBOOK_DOCUMENT_H
#define COMICBOOK_DOCUMENT_H

#include <QStringList>

class KArchiveDirectory;
class KArchive;
class QImage;
class Unrar;
class Directory;

namespace Okular
{
class Page;
}

namespace ComicBook
{
class Document
{
public:
    Document();
    ~Document();

    bool open(const QString &fileName);
    void close();

    void pages(QVector<Okular::Page *> *pagesVector);
    QStringList pageTitles() const;

    QImage pageImage(int page) const;

    QString lastErrorString() const;

private:
    bool processArchive();

    QStringList mPageMap;
    Directory *mDirectory;
    Unrar *mUnrar;
    KArchive *mArchive;
    const KArchiveDirectory *mArchiveDir;
    QString mLastErrorString;
    QStringList mEntries;
};

}

#endif
