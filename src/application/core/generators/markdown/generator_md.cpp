/*
    SPDX-FileCopyrightText: 2017 Julian Wolff <wolff@julianwolff.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "generator_md.h"

#include "converter.h"
#include "debug_md.h"

#include <KAboutData>
#include <KConfigDialog>
#include <KLocalizedString>

#include <QCheckBox>

OKULAR_EXPORT_PLUGIN(MarkdownGenerator, "libokularGenerator_md.json")

MarkdownGenerator::MarkdownGenerator(QObject *parent, const QVariantList &args)
    : Okular::TextDocumentGenerator(new Markdown::Converter, QStringLiteral("okular_markdown_generator_settings"), parent, args)
{
    Okular::TextDocumentSettings *mdSettings = generalSettings();

    mdSettings->addItemBool(QStringLiteral("SmartyPants"), m_isFancyPantsConfigEnabled, true);
    mdSettings->load();
    m_wasFancyPantsConfigEnabled = m_isFancyPantsConfigEnabled;
    Markdown::Converter *c = static_cast<Markdown::Converter *>(converter());
    c->setFancyPantsEnabled(m_isFancyPantsConfigEnabled);
}

bool MarkdownGenerator::reparseConfig()
{
    const bool textDocumentGeneratorChangedConfig = Okular::TextDocumentGenerator::reparseConfig();

    if (m_wasFancyPantsConfigEnabled != m_isFancyPantsConfigEnabled) {
        m_wasFancyPantsConfigEnabled = m_isFancyPantsConfigEnabled;

        Markdown::Converter *c = static_cast<Markdown::Converter *>(converter());
        c->setFancyPantsEnabled(m_isFancyPantsConfigEnabled);
        c->convertAgain();
        setTextDocument(c->document());

        return true;
    }

    return textDocumentGeneratorChangedConfig;
}

void MarkdownGenerator::addPages(KConfigDialog *dlg)
{
    Okular::TextDocumentSettingsWidget *widget = new Okular::TextDocumentSettingsWidget();

    QCheckBox *enableSmartyPants = new QCheckBox(dlg);
    enableSmartyPants->setObjectName(QStringLiteral("kcfg_SmartyPants"));
    widget->addRow(i18n("Enable SmartyPants formatting"), enableSmartyPants);

    dlg->addPage(widget, generalSettings(), i18n("Markdown"), QStringLiteral("text-markdown"), i18n("Markdown Backend Configuration"));
}

Q_LOGGING_CATEGORY(OkularMdDebug, "org.kde.okular.generators.md", QtWarningMsg)

#include "generator_md.moc"
