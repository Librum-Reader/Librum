/*
    SPDX-FileCopyrightText: 2008 Ely Levy <elylevy@cs.huji.ac.il>

    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "generator_mobi.h"

#include "converter.h"

#include <KAboutData>
#include <KConfigDialog>
#include <KLocalizedString>

OKULAR_EXPORT_PLUGIN(MobiGenerator, "libokularGenerator_mobi.json")

MobiGenerator::MobiGenerator(QObject *parent, const QVariantList &args)
    : Okular::TextDocumentGenerator(new Mobi::Converter, QStringLiteral("okular_mobi_generator_settings"), parent, args)
{
}

void MobiGenerator::addPages(KConfigDialog *dlg)
{
    Okular::TextDocumentSettingsWidget *widget = new Okular::TextDocumentSettingsWidget();

    dlg->addPage(widget, generalSettings(), i18n("Mobipocket"), QStringLiteral("application-x-mobipocket-ebook"), i18n("Mobipocket Backend Configuration"));
}

#include "generator_mobi.moc"
