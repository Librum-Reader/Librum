/*
    SPDX-FileCopyrightText: 2013 Azat Khuzhin <a3at.mail@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _GENERATOR_TXT_H_
#define _GENERATOR_TXT_H_

#include <core/textdocumentgenerator.h>

class TxtGenerator : public Okular::TextDocumentGenerator
{
    Q_OBJECT
    Q_INTERFACES(Okular::Generator)

public:
    TxtGenerator(QObject* parent, const QVariantList& args);

    ~TxtGenerator() override
    {
    }

    void addPages(KConfigDialog* dlg) override;
};

#endif
