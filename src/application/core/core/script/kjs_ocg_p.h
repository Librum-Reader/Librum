/*
    SPDX-FileCopyrightText: 2019 João Netto <joaonetto901@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_SCRIPT_KJS_OCG_P_H
#define OKULAR_SCRIPT_KJS_OCG_P_H

class KJSContext;
class KJSObject;
class QAbstractItemModel;

namespace Okular
{
class JSOCG
{
public:
    static void initType(KJSContext* ctx);
    static KJSObject object(KJSContext* ctx);
    static KJSObject wrapOCGObject(KJSContext* ctx, QAbstractItemModel* model,
                                   const int i, const int j);
    static void clearCachedFields();
};

}  // namespace Okular

#endif
