/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_TEXTEDITORS_P_H
#define OKULAR_TEXTEDITORS_P_H

#include <QHash>
#include <QString>
#include "settings_core.h"

namespace Okular
{
static inline QHash<int, QString> buildEditorsMap()
{
    QHash<int, QString> editors;
    editors.insert(SettingsCore::EnumExternalEditor::Kate,
                   QStringLiteral("kate --line %l --column %c"));
    editors.insert(SettingsCore::EnumExternalEditor::Kile,
                   QStringLiteral("kile --line %l"));
    editors.insert(SettingsCore::EnumExternalEditor::Scite,
                   QStringLiteral("scite %f \"-goto:%l,%c\""));
    editors.insert(SettingsCore::EnumExternalEditor::Emacsclient,
                   QStringLiteral("emacsclient -a emacs --no-wait +%l %f"));
    editors.insert(SettingsCore::EnumExternalEditor::Lyxclient,
                   QStringLiteral("lyxclient -g %f %l"));
    editors.insert(SettingsCore::EnumExternalEditor::Texstudio,
                   QStringLiteral("texstudio --line %l"));
    editors.insert(SettingsCore::EnumExternalEditor::Texifyidea,
                   QStringLiteral("idea --line %l"));
    return editors;
}

}  // namespace Okular

#endif
