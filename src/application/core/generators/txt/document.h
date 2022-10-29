/*
    SPDX-FileCopyrightText: 2013 Azat Khuzhin <a3at.mail@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _TXT_DOCUMENT_H_
#define _TXT_DOCUMENT_H_

#include <QTextDocument>

namespace Txt
{
class Document : public QTextDocument
{
    Q_OBJECT

public:
    explicit Document(const QString& fileName);
    ~Document() override;

private:
    QString toUnicode(const QByteArray& array);
};
}  // namespace Txt

#endif
