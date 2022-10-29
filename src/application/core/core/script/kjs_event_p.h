/*
    SPDX-FileCopyrightText: 2018 Intevation GmbH <intevation@intevation.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_SCRIPT_KJS_EVENT_P_H
#define OKULAR_SCRIPT_KJS_EVENT_P_H

class KJSContext;
class KJSObject;

namespace Okular
{
class Event;

class JSEvent
{
public:
    static void initType(KJSContext* ctx);
    static KJSObject wrapEvent(KJSContext* ctx, Event* event);
    static void clearCachedFields();
};

}  // namespace Okular

#endif
