/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>
    SPDX-FileCopyrightText: 2008 Harri Porten <porten@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_SCRIPT_KJS_DATA_P_H
#define OKULAR_SCRIPT_KJS_DATA_P_H

class KJSContext;
class KJSObject;

namespace Okular
{
class EmbeddedFile;

class JSData
{
public:
    static void initType(KJSContext* ctx);
    static KJSObject wrapFile(KJSContext* ctx, EmbeddedFile* f);
};

}  // namespace Okular

#endif
