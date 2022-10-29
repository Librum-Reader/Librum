/*
    SPDX-FileCopyrightText: 2008 Jakub Stachowski <qbast@go2.pl>

    SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef MOBI_CONVERTER_H
#define MOBI_CONVERTER_H

#include <core/document.h>
#include <core/textdocumentgenerator.h>
#include <qmobipocket/mobipocket.h>
#include "mobidocument.h"

namespace Mobi
{
class Converter : public Okular::TextDocumentConverter
{
    Q_OBJECT

public:
    Converter();
    ~Converter() override;

    QTextDocument* convert(const QString& fileName) override;

private:
    void handleMetadata(
        const QMap<Mobipocket::Document::MetaKey, QString>& metadata);
};
}  // namespace Mobi

#endif
