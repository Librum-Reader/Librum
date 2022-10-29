/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "generator_fb.h"
#include <KAboutData>
#include <KConfigDialog>
#include <KLocalizedString>
#include "converter.h"

OKULAR_EXPORT_PLUGIN(FictionBookGenerator, "libokularGenerator_fb.json")

FictionBookGenerator::FictionBookGenerator(QObject* parent,
                                           const QVariantList& args) :
    Okular::TextDocumentGenerator(
        new FictionBook::Converter,
        QStringLiteral("okular_fictionbook_generator_settings"), parent, args)
{
}

void FictionBookGenerator::addPages(KConfigDialog* dlg)
{
    Okular::TextDocumentSettingsWidget* widget =
        new Okular::TextDocumentSettingsWidget();

    dlg->addPage(widget, generalSettings(), i18n("FictionBook"),
                 QStringLiteral("okular-fb2"),
                 i18n("FictionBook Backend Configuration"));
}

#include "generator_fb.moc"
