/*
    SPDX-FileCopyrightText: 2019 João Netto <joaonetto901@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "../form.h"
#include "kjs_display_p.h"

#include <kjs/kjsobject.h>
#include <kjs/kjsprototype.h>

#include <QString>

using namespace Okular;

static KJSPrototype *g_displayProto;

// display.hidden
static KJSObject displayGetHidden(KJSContext *, void *)
{
    return KJSNumber(FormDisplay::FormHidden);
}

// display.visible
static KJSObject displayGetVisible(KJSContext *, void *)
{
    return KJSNumber(FormDisplay::FormVisible);
}

// display.noView
static KJSObject displayGetNoView(KJSContext *, void *)
{
    return KJSNumber(FormDisplay::FormNoView);
}

// display.noPrint
static KJSObject displayGetNoPrint(KJSContext *, void *)
{
    return KJSNumber(FormDisplay::FormNoPrint);
}

void JSDisplay::initType(KJSContext *ctx)
{
    static bool initialized = false;
    if (initialized) {
        return;
    }
    initialized = true;

    g_displayProto = new KJSPrototype();

    g_displayProto->defineProperty(ctx, QStringLiteral("hidden"), displayGetHidden);
    g_displayProto->defineProperty(ctx, QStringLiteral("visible"), displayGetVisible);
    g_displayProto->defineProperty(ctx, QStringLiteral("noView"), displayGetNoView);
    g_displayProto->defineProperty(ctx, QStringLiteral("noPrint"), displayGetNoPrint);
}

KJSObject JSDisplay::object(KJSContext *ctx)
{
    return g_displayProto->constructObject(ctx, nullptr);
}
