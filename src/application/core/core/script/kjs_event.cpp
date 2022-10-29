/*
    SPDX-FileCopyrightText: 2018 Intevation GmbH <intevation@intevation.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <kjs/kjsarguments.h>
#include <kjs/kjsinterpreter.h>
#include <kjs/kjsobject.h>
#include <kjs/kjsprototype.h>
#include "event_p.h"
#include "kjs_event_p.h"
#include "kjs_field_p.h"

using namespace Okular;

static KJSPrototype* g_eventProto;

// Event.name
static KJSObject eventGetName(KJSContext*, void* object)
{
    const Event* event = reinterpret_cast<Event*>(object);
    return KJSString(event->name());
}

// Event.type
static KJSObject eventGetType(KJSContext*, void* object)
{
    const Event* event = reinterpret_cast<Event*>(object);
    return KJSString(event->type());
}

// Event.targetName (getter)
static KJSObject eventGetTargetName(KJSContext*, void* object)
{
    const Event* event = reinterpret_cast<Event*>(object);
    return KJSString(event->targetName());
}

// Event.targetName (setter)
static void eventSetTargetName(KJSContext* ctx, void* object, KJSObject value)
{
    Event* event = reinterpret_cast<Event*>(object);
    event->setTargetName(value.toString(ctx));
}

// Event.shift
static KJSObject eventGetShift(KJSContext*, void* object)
{
    const Event* event = reinterpret_cast<Event*>(object);
    return KJSBoolean(event->shiftModifier());
}

// Event.source
static KJSObject eventGetSource(KJSContext* ctx, void* object)
{
    const Event* event = reinterpret_cast<Event*>(object);
    if(event->eventType() == Event::FieldCalculate)
    {
        FormField* src = event->source();
        if(src)
        {
            return JSField::wrapField(ctx, src, event->sourcePage());
        }
    }
    return KJSUndefined();
}

// Event.target
static KJSObject eventGetTarget(KJSContext* ctx, void* object)
{
    const Event* event = reinterpret_cast<Event*>(object);
    switch(event->eventType())
    {
    case Event::FieldCalculate:
    case Event::FieldFormat:
    case Event::FieldKeystroke:
    case Event::FieldFocus:
    case Event::FieldValidate:
    {
        FormField* target = static_cast<FormField*>(event->target());
        if(target)
        {
            return JSField::wrapField(ctx, target, event->targetPage());
        }
        break;
    }
    default:
    {
    }
    }
    return KJSUndefined();
}

// Event.value (getter)
static KJSObject eventGetValue(KJSContext*, void* object)
{
    const Event* event = reinterpret_cast<Event*>(object);
    return KJSString(event->value().toString());
}

// Event.value (setter)
static void eventSetValue(KJSContext* ctx, void* object, KJSObject value)
{
    Event* event = reinterpret_cast<Event*>(object);
    event->setValue(QVariant(value.toString(ctx)));
}

// Event.rc (getter)
static KJSObject eventGetReturnCode(KJSContext*, void* object)
{
    const Event* event = reinterpret_cast<Event*>(object);
    return KJSBoolean(event->returnCode());
}

// Event.rc (setter)
static void eventSetReturnCode(KJSContext* ctx, void* object, KJSObject value)
{
    Event* event = reinterpret_cast<Event*>(object);
    event->setReturnCode(value.toBoolean(ctx));
}

// Event.willCommit (getter)
static KJSObject eventGetWillCommit(KJSContext*, void* object)
{
    const Event* event = reinterpret_cast<Event*>(object);
    return KJSBoolean(event->willCommit());
}

void JSEvent::initType(KJSContext* ctx)
{
    static bool initialized = false;
    if(initialized)
    {
        return;
    }
    initialized = true;

    if(!g_eventProto)
    {
        g_eventProto = new KJSPrototype();
    }

    g_eventProto->defineProperty(ctx, QStringLiteral("name"), eventGetName);
    g_eventProto->defineProperty(ctx, QStringLiteral("type"), eventGetType);
    g_eventProto->defineProperty(ctx, QStringLiteral("targetName"),
                                 eventGetTargetName, eventSetTargetName);
    g_eventProto->defineProperty(ctx, QStringLiteral("shift"), eventGetShift);
    g_eventProto->defineProperty(ctx, QStringLiteral("source"), eventGetSource);
    g_eventProto->defineProperty(ctx, QStringLiteral("target"), eventGetTarget);
    g_eventProto->defineProperty(ctx, QStringLiteral("willCommit"),
                                 eventGetWillCommit);
    g_eventProto->defineProperty(ctx, QStringLiteral("value"), eventGetValue,
                                 eventSetValue);
    g_eventProto->defineProperty(ctx, QStringLiteral("rc"), eventGetReturnCode,
                                 eventSetReturnCode);
}

KJSObject JSEvent::wrapEvent(KJSContext* ctx, Event* event)
{
    return g_eventProto->constructObject(ctx, event);
}
