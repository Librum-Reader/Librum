/*
    SPDX-FileCopyrightText: 2013 Azat Khuzhin <a3at.mail@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "generator_txt.h"
#include "converter.h"

#include <KAboutData>
#include <KConfigDialog>
#include <KLocalizedString>

OKULAR_EXPORT_PLUGIN(TxtGenerator, "libokularGenerator_txt.json")

TxtGenerator::TxtGenerator(QObject *parent, const QVariantList &args)
    : Okular::TextDocumentGenerator(new Txt::Converter, QStringLiteral("okular_txt_generator_settings"), parent, args)
{
}

void TxtGenerator::addPages(KConfigDialog *dlg)
{
    Okular::TextDocumentSettingsWidget *widget = new Okular::TextDocumentSettingsWidget();

    dlg->addPage(widget, generalSettings(), i18n("Txt"), QStringLiteral("text-plain"), i18n("Txt Backend Configuration"));
}

#include "generator_txt.moc"
