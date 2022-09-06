/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef FICTIONBOOK_DOCUMENT_H
#define FICTIONBOOK_DOCUMENT_H

#include <QByteArray>
#include <QDomDocument>
#include <QMap>
#include <QString>

namespace FictionBook
{
class Document
{
public:
    explicit Document(const QString &fileName);

    bool open();

    QDomDocument content() const;

    QString lastErrorString() const;

private:
    void setError(const QString &);

    QString mFileName;
    QDomDocument mDocument;
    QString mErrorString;
};

}

#endif
