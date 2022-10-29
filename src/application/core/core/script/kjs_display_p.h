/*
    SPDX-FileCopyrightText: 2019 João Netto <joaonetto901@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_SCRIPT_KJS_DISPLAY_P_H
#define OKULAR_SCRIPT_KJS_DISPLAY_P_H

class KJSContext;
class KJSObject;

namespace Okular
{
/**
 * The display types of the field.
 */
enum FormDisplay
{
    FormVisible,
    FormHidden,
    FormNoPrint,
    FormNoView
};

class JSDisplay
{
public:
    static void initType(KJSContext* ctx);
    static KJSObject object(KJSContext* ctx);
};

}  // namespace Okular

#endif
