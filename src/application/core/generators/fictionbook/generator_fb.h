/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_GENERATOR_FB_H_
#define _OKULAR_GENERATOR_FB_H_

#include <core/textdocumentgenerator.h>

class FictionBookGenerator : public Okular::TextDocumentGenerator
{
    Q_OBJECT
    Q_INTERFACES(Okular::Generator)

public:
    FictionBookGenerator(QObject *parent, const QVariantList &args);

    // [INHERITED] reparse configuration
    void addPages(KConfigDialog *dlg) override;
};

#endif
