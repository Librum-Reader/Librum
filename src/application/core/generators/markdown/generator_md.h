/*
    SPDX-FileCopyrightText: 2017 Julian Wolff <wolff@julianwolff.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_GENERATOR_MD_H_
#define _OKULAR_GENERATOR_MD_H_

#include <core/textdocumentgenerator.h>

class MarkdownGenerator : public Okular::TextDocumentGenerator
{
    Q_OBJECT
    Q_INTERFACES(Okular::Generator)

public:
    MarkdownGenerator(QObject* parent, const QVariantList& args);

    // [INHERITED] reparse configuration
    bool reparseConfig() override;
    void addPages(KConfigDialog* dlg) override;

private:
    bool m_isFancyPantsConfigEnabled = true;
    bool m_wasFancyPantsConfigEnabled = true;
};

#endif
