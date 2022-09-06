/*
    SPDX-FileCopyrightText: 2008 Jakub Stachowski <qbast@go2.pl>

    SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef MOBI_DOCUMENT_H
#define MOBI_DOCUMENT_H

#include <QTextDocument>
#include <QUrl>
#include <QVariant>

class QFile;
namespace Mobipocket
{
class Document;
class QFileStream;
}

namespace Mobi
{
class MobiDocument : public QTextDocument
{
    Q_OBJECT

public:
    explicit MobiDocument(const QString &fileName);
    ~MobiDocument() override;

    Mobipocket::Document *mobi() const
    {
        return doc;
    }

protected:
    QVariant loadResource(int type, const QUrl &name) override;

private:
    QString fixMobiMarkup(const QString &data);
    Mobipocket::Document *doc;
    Mobipocket::QFileStream *file;
};

}
#endif
