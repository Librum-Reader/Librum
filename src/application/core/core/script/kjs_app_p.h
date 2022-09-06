/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>
    SPDX-FileCopyrightText: 2008 Harri Porten <porten@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_SCRIPT_KJS_APP_P_H
#define OKULAR_SCRIPT_KJS_APP_P_H

class KJSContext;
class KJSObject;
class QTimer;

namespace Okular
{
class DocumentPrivate;

class JSApp
{
public:
    static void initType(KJSContext *ctx);
    static KJSObject object(KJSContext *ctx, DocumentPrivate *doc);
    static KJSObject wrapTimer(KJSContext *ctx, QTimer *timer);
    static void clearCachedFields();
};

}

#endif
