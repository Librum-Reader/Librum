/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>
    SPDX-FileCopyrightText: 2008 Harri Porten <porten@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <kjs/kjsobject.h>
#include <kjs/kjsprototype.h>
#include <QDateTime>
#include "../document.h"
#include "kjs_data_p.h"

using namespace Okular;

static KJSPrototype* g_dataProto;

static KJSObject dataGetCreationDate(KJSContext* ctx, void* object)
{
    const EmbeddedFile* file = reinterpret_cast<EmbeddedFile*>(object);

    return KJSDate(ctx, file->creationDate());
}

static KJSObject dataGetDescription(KJSContext*, void* object)
{
    const EmbeddedFile* file = reinterpret_cast<EmbeddedFile*>(object);

    return KJSString(file->description());
}

static KJSObject dataGetMIMEType(KJSContext*, void*)
{
    return KJSString("");
}

static KJSObject dataGetModDate(KJSContext* ctx, void* object)
{
    const EmbeddedFile* file = reinterpret_cast<EmbeddedFile*>(object);

    return KJSDate(ctx, file->modificationDate());
}

static KJSObject dataGetName(KJSContext*, void* object)
{
    const EmbeddedFile* file = reinterpret_cast<EmbeddedFile*>(object);

    return KJSString(file->name());
}

static KJSObject dataGetPath(KJSContext*, void*)
{
    return KJSString("");
}

static KJSObject dataGetSize(KJSContext*, void* object)
{
    const EmbeddedFile* file = reinterpret_cast<EmbeddedFile*>(object);
    return KJSNumber(file->size());
}

void JSData::initType(KJSContext* ctx)
{
    static bool initialized = false;
    if(initialized)
    {
        return;
    }
    initialized = true;

    if(!g_dataProto)
    {
        g_dataProto = new KJSPrototype();
    }

    g_dataProto->defineProperty(ctx, QStringLiteral("creationDate"),
                                dataGetCreationDate);
    g_dataProto->defineProperty(ctx, QStringLiteral("description"),
                                dataGetDescription);
    g_dataProto->defineProperty(ctx, QStringLiteral("MIMEType"),
                                dataGetMIMEType);
    g_dataProto->defineProperty(ctx, QStringLiteral("modDate"), dataGetModDate);
    g_dataProto->defineProperty(ctx, QStringLiteral("name"), dataGetName);
    g_dataProto->defineProperty(ctx, QStringLiteral("path"), dataGetPath);
    g_dataProto->defineProperty(ctx, QStringLiteral("size"), dataGetSize);
}

KJSObject JSData::wrapFile(KJSContext* ctx, EmbeddedFile* f)
{
    return g_dataProto->constructObject(ctx, f);
}
