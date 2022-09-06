/*
    SPDX-FileCopyrightText: 2013 Azat Khuzhin <a3at.mail@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_TEXTDOCUMENTSETTINGS_P_H_
#define _OKULAR_TEXTDOCUMENTSETTINGS_P_H_

class KFontRequester;
class Ui_TextDocumentSettings;

namespace Okular
{
class TextDocumentSettingsWidgetPrivate
{
public:
    /**
     * @note the private class won't take ownership of the ui, so you
     *       must delete it yourself
     */
    explicit TextDocumentSettingsWidgetPrivate(Ui_TextDocumentSettings *ui)
        : mUi(ui)
    {
    }

    KFontRequester *mFont;
    Ui_TextDocumentSettings *mUi;
};

class TextDocumentSettingsPrivate : public QObject
{
    Q_OBJECT

public:
    explicit TextDocumentSettingsPrivate(QObject *parent)
        : QObject(parent)
    {
    }

    QFont mFont;
};

}

#endif
