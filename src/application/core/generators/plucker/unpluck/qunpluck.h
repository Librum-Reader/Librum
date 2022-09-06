/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

    Based on code written by Bill Janssen 2002

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef QUNPLUCK_H
#define QUNPLUCK_H

#include <QImage>
#include <QList>
#include <QMap>
#include <QTextBlock>

#include "unpluck.h"

class Context;
class RecordNode;
class QTextDocument;

namespace Okular
{
class Action;
}

class Link
{
public:
    Link()
        : link(nullptr)
    {
    }

    typedef QVector<Link> List;

    Okular::Action *link;
    QString url;
    int page;
    int start;
    int end;
};

class QUnpluck
{
public:
    QUnpluck();
    ~QUnpluck();

    QUnpluck(const QUnpluck &) = delete;
    QUnpluck &operator=(const QUnpluck &) = delete;

    bool open(const QString &fileName);

    QList<QTextDocument *> pages() const
    {
        return mPages;
    }
    Link::List links() const
    {
        return mLinks;
    }
    QMap<QString, QString> infos() const
    {
        return mInfo;
    }

private:
    int GetNextRecordNumber();
    int GetPageID(int index);
    void AddRecord(int index);
    void MarkRecordDone(int index);
    void SetPageID(int index, int page_id);
    QString MailtoURLFromBytes(unsigned char *record_data);
    void DoStyle(Context *context, int style, bool start);
    bool TranscribeRecord(int index);
    QImage TranscribeImageRecord(unsigned char *bytes);
    bool TranscribeTableRecord(plkr_Document *doc, Context *context, unsigned char *bytes);
    bool TranscribeTextRecord(plkr_Document *doc, int id, Context *context, unsigned char *bytes, plkr_DataRecordType type);
    void ParseText(plkr_Document *doc, unsigned char *ptr, int text_len, int *font, int *style, Context *context);

    plkr_Document *mDocument;
    QList<RecordNode *> mRecords;

    QList<Context *> mContext;
    QList<QTextDocument *> mPages;
    QMap<QString, QPair<int, QTextBlock>> mNamedTargets;
    QMap<int, QImage> mImages;
    QMap<QString, QString> mInfo;
    QString mErrorString;
    Link::List mLinks;
};

#endif
