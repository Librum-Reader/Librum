/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>
    SPDX-FileCopyrightText: 2008 Harri Porten <porten@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kjs_spell_p.h"

#include <kjs/kjsobject.h>
#include <kjs/kjsprototype.h>

#include <QString>

using namespace Okular;

static KJSPrototype *g_spellProto;

// Spell.available
static KJSObject spellGetAvailable(KJSContext *, void *)
{
    return KJSBoolean(false);
}

void JSSpell::initType(KJSContext *ctx)
{
    static bool initialized = false;
    if (initialized) {
        return;
    }
    initialized = true;

    g_spellProto = new KJSPrototype();
    g_spellProto->defineProperty(ctx, QStringLiteral("available"), spellGetAvailable);
}

KJSObject JSSpell::object(KJSContext *ctx)
{
    return g_spellProto->constructObject(ctx, nullptr);
}
