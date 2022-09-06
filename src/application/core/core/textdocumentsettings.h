/*
    SPDX-FileCopyrightText: 2013 Azat Khuzhin <a3at.mail@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_TEXTDOCUMENTSETTINGS_H_
#define _OKULAR_TEXTDOCUMENTSETTINGS_H_

#include "okularcore_export.h"

#include <KConfigSkeleton>
#include <QFont>
#include <QObject>
#include <QWidget>

namespace Okular
{
class TextDocumentSettingsWidgetPrivate;
class TextDocumentSettingsPrivate;

/**
 * Here is example of how you can add custom settings per-backend:
 *
 * In .h header:
 * \code
 * class KIntSpinBox;
 * ...
 *
 * class YourGenerator
 * {
 * ...
 * public:
 *     bool reparseConfig();
 *     void addPages( KConfigDialog* dlg );
 * ...
 * private:
 *     QString customArgument;
 *     KIntSpinBox *customArgumentWidget;
 * ...
 * }
 * \endcode
 *
 * In .cpp module:
 * \code
 * #include <KIntSpinBox>
 * ...
 * bool YourGenerator::reparseConfig()
 * {
 *     ... Do something with customArgumentWidget and customArgument ...
 * }
 * void YourGenerator::addPages( KConfigDialog* dlg )
 * {
 *     Okular::TextDocumentSettingsWidget *widget = new Okular::TextDocumentSettingsWidget();
 *
 *     KIntSpinBox *customArgumentWidget = new KIntSpinBox( dlg );
 *     customArgumentWidget->setObjectName( QString::fromUtf8( "kcfg_CustomArgument" ) );
 *     widget->addRow( "Custom argument", customArgumentWidget );
 *
 *     Okular::TextDocumentSettings *settings = generalSettings();
 *     settings->addItemString( "CustomArgument", customArgument );
 *
 *     dlg->addPage( widget, settings, ... );
 * }
 * \endcode
 */

/**
 * TextDocumentSettingsWidget
 *
 * Contain default settings for text based documents.
 * (all generators that inherited from TextDocumentGenerator)
 * Generator can add settings to this object individually.
 *
 * @since 0.17 (KDE 4.11)
 */
class OKULARCORE_EXPORT TextDocumentSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TextDocumentSettingsWidget(QWidget *parent = nullptr);
    ~TextDocumentSettingsWidget() override;

    void addRow(const QString &labelText, QWidget *widget);

private:
    friend class TextDocumentGenerator;

    TextDocumentSettingsWidgetPrivate *d_ptr;
    Q_DECLARE_PRIVATE(TextDocumentSettingsWidget)
    Q_DISABLE_COPY(TextDocumentSettingsWidget)
};

/**
 * TextDocumentSettings
 *
 * Contain default settings/config skeleton
 * To save/restore settings.
 *
 * @since 0.17 (KDE 4.11)
 */
class OKULARCORE_EXPORT TextDocumentSettings : public KConfigSkeleton
{
    Q_OBJECT

public:
    QFont font() const;

private:
    friend class TextDocumentGenerator;

    TextDocumentSettings(const QString &config, QObject *parent);

    TextDocumentSettingsPrivate *d_ptr;
    Q_DECLARE_PRIVATE(TextDocumentSettings)
    Q_DISABLE_COPY(TextDocumentSettings)
};

}

#endif
