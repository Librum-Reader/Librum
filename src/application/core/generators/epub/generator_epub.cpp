/*
    SPDX-FileCopyrightText: 2008 Ely Levy <elylevy@cs.huji.ac.il>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "generator_epub.h"

#include "converter.h"

#include <KAboutData>
#include <KConfigDialog>
#include <KLocalizedString>

OKULAR_EXPORT_PLUGIN(EPubGenerator, "libokularGenerator_epub.json")

EPubGenerator::EPubGenerator(QObject *parent, const QVariantList &args)
    : Okular::TextDocumentGenerator(new Epub::Converter, QStringLiteral("okular_epub_generator_settings"), parent, args)
{
}

EPubGenerator::~EPubGenerator()
{
}

void EPubGenerator::addPages(KConfigDialog *dlg)
{
    Okular::TextDocumentSettingsWidget *widget = new Okular::TextDocumentSettingsWidget();

    dlg->addPage(widget, generalSettings(), i18n("EPub"), QStringLiteral("application-epub+zip"), i18n("EPub Backend Configuration"));
}

#include "generator_epub.moc"
