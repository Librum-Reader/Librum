/*
    SPDX-FileCopyrightText: 2013 Azat Khuzhin <a3at.mail@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef TXT_CONVERTER_H
#define TXT_CONVERTER_H

#include <core/document.h>
#include <core/textdocumentgenerator.h>

namespace Txt
{
class Converter : public Okular::TextDocumentConverter
{
    Q_OBJECT

public:
    Converter();
    ~Converter() override;

    QTextDocument *convert(const QString &fileName) override;
};
}

#endif
