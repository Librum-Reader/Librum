/*
    SPDX-FileCopyrightText: 2019 João Netto <joaonetto901@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <kjs/kjsinterpreter.h>
#include <kjs/kjsobject.h>
#include <kjs/kjsprototype.h>
#include <QAbstractItemModel>
#include <QDebug>
#include <QPair>
#include <QString>
#include "kjs_ocg_p.h"

using namespace Okular;

static KJSPrototype* g_OCGProto;

typedef QHash<QPair<int, int>*, QAbstractItemModel*> OCGCache;
Q_GLOBAL_STATIC(OCGCache, g_OCGCache)

// OCG.state (getter)
static KJSObject OCGGetState(KJSContext*, void* object)
{
    QPair<int, int>* pair = reinterpret_cast<QPair<int, int>*>(object);
    QAbstractItemModel* model = g_OCGCache->value(pair);

    const QModelIndex index = model->index(pair->first, pair->second);

    const bool state = model->data(index, Qt::CheckStateRole).toBool();

    return KJSBoolean(state);
}

// OCG.state (setter)
static void OCGSetState(KJSContext* ctx, void* object, KJSObject value)
{
    QPair<int, int>* pair = reinterpret_cast<QPair<int, int>*>(object);
    QAbstractItemModel* model = g_OCGCache->value(pair);

    const QModelIndex index = model->index(pair->first, pair->second);

    const bool state = value.toBoolean(ctx);

    model->setData(index, QVariant(state ? Qt::Checked : Qt::Unchecked),
                   Qt::CheckStateRole);
}

void JSOCG::initType(KJSContext* ctx)
{
    static bool initialized = false;
    if(initialized)
    {
        return;
    }
    initialized = true;

    g_OCGProto = new KJSPrototype();

    g_OCGProto->defineProperty(ctx, QStringLiteral("state"), OCGGetState,
                               OCGSetState);
}

KJSObject JSOCG::object(KJSContext* ctx)
{
    return g_OCGProto->constructObject(ctx, nullptr);
}

KJSObject JSOCG::wrapOCGObject(KJSContext* ctx, QAbstractItemModel* model,
                               const int i, const int j)
{
    QPair<int, int>* pair = new QPair<int, int>(i, j);
    g_OCGCache->insert(pair, model);
    return g_OCGProto->constructObject(ctx, pair);
}

void JSOCG::clearCachedFields()
{
    if(g_OCGCache.exists())
    {
        g_OCGCache->clear();
    }
}
