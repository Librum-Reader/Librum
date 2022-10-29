/*
    SPDX-FileCopyrightText: 2013 Azat Khuzhin <a3at.mail@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "textdocumentsettings.h"
#include <KFontRequester>
#include <KLocalizedString>
#include "textdocumentsettings_p.h"
#include "ui_textdocumentsettings.h"

using namespace Okular;

/**
 * TextDocumentSettingsWidget
 */

TextDocumentSettingsWidget::TextDocumentSettingsWidget(QWidget* parent) :
    QWidget(parent),
    d_ptr(new TextDocumentSettingsWidgetPrivate(new Ui_TextDocumentSettings()))
{
    Q_D(TextDocumentSettingsWidget);

    d->mUi->setupUi(this);

    // @notice I think this will be useful in future.
#define ADD_WIDGET(property, widget, objectName, labelName)    \
    d->property = new widget(this);                            \
    d->property->setObjectName(QString::fromUtf8(objectName)); \
    addRow(labelName, d->property);

    ADD_WIDGET(mFont, KFontRequester, "kcfg_Font", i18n("&Default Font:"));
#undef ADD_WIDGET
}

TextDocumentSettingsWidget::~TextDocumentSettingsWidget()
{
    Q_D(TextDocumentSettingsWidget);

    delete d->mUi;
    delete d;
}

void TextDocumentSettingsWidget::addRow(const QString& labelText,
                                        QWidget* widget)
{
    Q_D(TextDocumentSettingsWidget);

    d->mUi->formLayout->addRow(labelText, widget);
}

/**
 * TextDocumentSettings
 */

TextDocumentSettings::TextDocumentSettings(const QString& config,
                                           QObject* parent) :
    KConfigSkeleton(config, parent),
    d_ptr(new TextDocumentSettingsPrivate(this))
{
    Q_D(TextDocumentSettings);

    addItemFont(QStringLiteral("Font"), d->mFont);
}

QFont TextDocumentSettings::font() const
{
    Q_D(const TextDocumentSettings);
    return d->mFont;
}
