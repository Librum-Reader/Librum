/*
    SPDX-FileCopyrightText: 2008 Ely Levy <elylevy@cs.huji.ac.il>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_GENERATOR_EPUB_H_
#define _OKULAR_GENERATOR_EPUB_H_
#include <core/textdocumentgenerator.h>

class EPubGenerator : public Okular::TextDocumentGenerator
{
    Q_OBJECT
    Q_INTERFACES(Okular::Generator)

public:
    EPubGenerator(QObject* parent, const QVariantList& args);
    ~EPubGenerator() override;

    // [INHERITED] reparse configuration
    void addPages(KConfigDialog* dlg) override;
};

#endif
