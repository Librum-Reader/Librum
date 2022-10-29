/*
    SPDX-FileCopyrightText: 2008 Ely Levy <elylevy@cs.huji.ac.il>

    SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef OKULAR_GENERATOR_MOBI_H
#define OKULAR_GENERATOR_MOBI_H
#include <core/textdocumentgenerator.h>

class MobiGenerator : public Okular::TextDocumentGenerator
{
    Q_OBJECT
    Q_INTERFACES(Okular::Generator)

public:
    MobiGenerator(QObject* parent, const QVariantList& args);

    ~MobiGenerator() override
    {
    }

    // [INHERITED] reparse configuration
    void addPages(KConfigDialog* dlg) override;
};

#endif
